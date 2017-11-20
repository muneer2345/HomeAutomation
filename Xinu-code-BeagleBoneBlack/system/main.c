#include <xinu.h>
#include <stdio.h>
#include <string.h>


/* Temperature sensor auxilary code */



char* read_temp(char* centi) {
	
	char val[4];
        
        unsigned int voltage_int = 0;
        float voltage;
        
        device_read(1, ADC,(char*)val, 4);
        unsigned int vol = *(unsigned int*)val;
        voltage = (vol*1.8/4095)*1000;
        voltage_int = (unsigned int)voltage;
                
        unsigned int c = voltage_int-500;
        c/=10;
        
	sprintf(centi, "%d", c);
		
	return centi;
}


/* Client to send Temperature or other analog sensor values to edge */

process client(){
	
	int	i;			/* index into buffer		*/
	int	retval;			/* return value			*/
	char	msg[] = "Xinu testing UDP echo"; /* message to send	*/
	char	inbuf[1500];		/* buffer for incoming reply	*/
	int32	slot;			/* UDP slot to use		*/
	int32	msglen;			/* length of outgoing message	*/
	uint32	remoteip;		/* remote IP address to use	*/
	
	uint16	echoport= 10000;		/* port number for UDP echo	*/
	uint16	locport	= 52743;	/* local port to use		*/
	int32	retries	= 3;		/* number of retries		*/
	int32	delay	= 2000;		/* reception delay in ms	*/
	
	
	char *args ="192.168.1.100";    /* # Edge IP has to be updated before booting*/


	if (dot2ip(args, &remoteip) == SYSERR) {
		return 1;
	}

	
	slot = udp_register(remoteip, echoport, locport);

	if (slot == SYSERR) {
		return 1;
	}

	/* Retry sending outgoing datagram and getting response */
	kprintf("Temperature sensor available\n");
	msglen = strnlen(msg, 1200);
	char *tempChanged = (char *)getmem(100);
	
	while(1){
		char *send_val;
		sleepms(300);
		/* Read each  sensor  and assign to send_val*/
		char *centi =(char*)getmem(100);
		centi = read_temp(centi);
		msglen = strnlen(centi, 1200);
		if(!strncmp(centi,tempChanged, strlen(centi))){
			continue;
		}
		
		kprintf(" Temperature: %s \n", centi);
		strncpy(tempChanged, centi, sizeof(centi));
		retval = udp_send(slot, centi, msglen);
		if (retval == SYSERR) {
			return 1;
		}

		retval = udp_recv(slot, inbuf, sizeof(inbuf), delay);
		if (retval == TIMEOUT) {
					
		} else if (retval == SYSERR) {
		
			udp_release(slot);
			return 1;
		}
		
	}

	udp_release(slot);
	if (retval == TIMEOUT) {
		return 1;
	}

	/* Response received - check contents */

	if (retval != msglen) {
		return 1;
	}
	for (i = 0; i < msglen; i++) {
		if (msg[i] != inbuf[i]) {
			return 1;
		}
	}

	
	return OK;


}



/* Server listening to the commands from edge 
 * expected msgs are LED ON (1), LED OFF(0)  	
 */

 process server(){

	int32	retval;			/* return value from sys calls	*/
	uint32	localip;		/* local IP address		*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	msglen;			/* length of outgoing message	*/
	int32	slot;			/* slot in UDP table 		*/
	uint16	echoserverport= 7;	/* port number for UDP echo	*/
	int32 loop=1;

	/* register local UDP port */
	
	slot = udp_register(0, 0, echoserverport);
	if (slot == SYSERR) {
		return 1;
	}

	/* Do forever: read an incoming datagram and send it back */
	kprintf("LED Service available\n");
	char *ledLocal = (char*)getmem(100);
	while (1) {
		
		

		retval = udp_recvaddr(slot, &remip, &remport, buff,
						sizeof(buff), 600000);
		if(!strncmp(buff, ledLocal, strlen(buff))){
			continue;		
		}
		strcpy(ledLocal, buff);
		if (retval == TIMEOUT) {
			continue;
		} else if (retval == SYSERR) {
			return 1;
		}
		msglen = retval;
		retval = udp_sendto(slot, remip, remport, buff, msglen);
		

		
		
		kprintf("LED status %s \n",  buff);	
		device_write(0, LED, buff, 1);
		
		if (retval == SYSERR) {
			return 1;
		}
		loop++;
	}
	return OK;


}
	

int main(int argc, const char *argv[])
{


	recvclr();
	
	/* This is the main application that runs client and server 
	 * to send the sensors data and to receive actuator commands
	 */	

	
	resume(create(server, 8192, 50, "server", 0));
	resume(create(client, 8192, 50, "client", 0));


	return OK;
}

