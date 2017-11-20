#include<xinu.h>


#define GPIO_BASE		0x4804c000
#define GPIO_ENABLE		0x134 
#define GPIO_DATA_IN 		0x138
#define GPIO_DATA_OUT 		0x13C
#define GPIO_CLEARDATA		0x190
#define GPIO_SETDATA		0x194

devcall ledinit(struct dentry *devptr) {

	int32 pin21 = ((1 << 21));
	int32 pin22 = ((1 << 22));
	int32 pin23 = ((1 << 23));
	int32 pin24 = ((1 << 24));

	int32 pin13 = ((1 << 13));

	*((unsigned int *)((void *)(GPIO_BASE)+ GPIO_ENABLE)) &= ~pin21;
	*((unsigned int *)((void *)(GPIO_BASE)+ GPIO_ENABLE)) &= ~pin22;
	*((unsigned int *)((void *)(GPIO_BASE)+ GPIO_ENABLE)) &= ~pin23;
	*((unsigned int *)((void *)(GPIO_BASE)+ GPIO_ENABLE)) &= ~pin24;

	
	*((unsigned int *)((void *)(GPIO_BASE)+ GPIO_ENABLE)) &= ~pin13;
	
}



