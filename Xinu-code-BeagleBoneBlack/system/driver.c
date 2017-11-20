#include<xinu.h>

char* device_read(int dev_id,  char* dev_name, char *buf, int val){
	
	switch(dev_id){
		case 1 : 
		read(dev_name,  buf, val );
		break; 
		default: 
			break;		
	}

	return buf;
}


int device_write(int dev_id, char* dev_name, char *buf, int val){	
	switch(dev_id){
		case 0 : 
		write(dev_name,  buf, val );
		break; 
		default:
			break;
		}

	return 0;
}
