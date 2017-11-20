#include <xinu.h>


extern sid32 adc_sem;

/* Function to read ADC values, o/p value of voltage updated in the 
	buffer address passed by the caller */

devcall	adcread(struct dentry* devptr, char* buff, int32 count)
{
	
	struct adc_reg_set* adc_reg = (struct adc_reg_set*)devptr->dvcsr;

	/* Enable the step module of ADC before reading the FIFO registers */
	ADC_Step_Enable(adc_reg,1);

	/* wait on adc_sem, this will be signalled by interrupt handler */
	wait(adc_sem);

	unsigned int fifo_val = 0;
	
	int fifo_count = adc_reg->fifoInfo[0].fifo_count;
	
	int i;
	
	for(i = 0; i < fifo_count; i++)
	{
		fifo_val = adc_reg->fifo_data &(0xFFF) ;
	}
	
	buff[3] = (fifo_val&(0xff000000))>>24;
	buff[2] = (fifo_val&(0x00ff0000))>>16;	
	buff[1] = (fifo_val&(0x0000ff00))>>8;
	buff[0] = (fifo_val&(0x000000ff));
	
	return 1;
}
