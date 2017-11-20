#include <xinu.h>

extern sid32 adc_sem;

void adc_intr_handler(uint32 num)
{
	struct	dentry	*devptr;            
	struct	adc_reg_set *csrptr;          
	volatile uint32	local_intr_status = 0;      
	
	
	devptr = (struct dentry *) &devtab[ADC];
	csrptr = (struct adc_reg_set *) devptr->dvcsr;
	
	/* Copying the Intr status to locally */
	local_intr_status = csrptr->irqStatus;      
	csrptr->irqStatus = local_intr_status;      

	ADC_Step_Disable(csrptr, 0);
	
	/* unblock adc_read */
	signal(adc_sem);                    
	return;
}