#include<xinu.h>

#define GPIO_BASE		0x4804c000


#define GPIO_ENABLE		0x134 
#define GPIO_DATA_IN 		0x138
#define GPIO_DATA_OUT 		0x13C
#define GPIO_CLEARDATA		0x190
#define GPIO_SETDATA		0x194


static void offboard_led_on()
{	
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_SETDATA)) |= (1 << 13);	

}

static void offboard_led_off()
{
	
	
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) |= (1 << 13);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) &= ~(1 << 13);

}





static void led_on()
{	

	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_SETDATA)) |= (1 << 21);	
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_SETDATA)) |= (1 << 22);	
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_SETDATA)) |= (1 << 23);	
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_SETDATA)) |= (1 << 24);
	
}

static void led_off()
{

	

	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) |= (1 << 21);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) |= (1 << 22);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) |= (1 << 23);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) |= (1 << 24);

	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) &= ~(1 << 21);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) &= ~(1 << 22);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) &= ~(1 << 23);
	*((volatile unsigned int *)((void *)(GPIO_BASE) + GPIO_CLEARDATA)) &= ~(1 << 24);

}

devcall ledwrite(struct dentry	*devptr,	/* Entry in device switch table	*/
				char	*buff,			/* Buffer containing a block	*/
				int32	val			/* Block number to write	*/
				)
{
	
	if (!strncmp(buff,"LED0", strlen(buff))) {
		
		led_off();
	} else if (!strncmp(buff,"LED1",strlen(buff))) {
		
		led_on();

	} else if (!strncmp(buff,"USRLED0",strlen(buff))) {
		
		offboard_led_off();

	} else if (!strncmp(buff,"USRLED1",strlen(buff))) {
		
		offboard_led_on();

	} else {
		kprintf("Nothing \n");
		/* Do Nothing */
	}	
	
}


