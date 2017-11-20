#include <xinu.h>

/* Clock registers */
/* The values will be configured according to the manual and references */

#define CM_WKUP                               (0x44E00400)		
#define CM_WKUP_CLKSTCTRL                     (0x44E00400)
#define CM_WKUP_CONTROL_CLKCTRL               (0x44E00404)
#define CM_WKUP_L4WKUP_CLKCTRL                (0x44E0040C)
#define CM_L3_AON_CLKSTCTRL                   (0x44E00418)
#define CM_WKUP_ADC_TSC_CLKCTRL               (0x44E004BC)
#define CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL      (0x44E004CC)


#define VALUE_OF_REG(n)	(*((volatile unsigned int *)(n)))

/* Semaphore to synchronize b/w interrupt handler and read func call 
 * i.e to synchronize low level and high level driver 
 */ 
sid32 adc_sem;


/* Initialize ADC clk as per the register update sequence */
void ADC_Clk_Init()
{
	VALUE_OF_REG(CM_WKUP_CONTROL_CLKCTRL) |= 0x02;                             
	while(( VALUE_OF_REG(CM_WKUP_CONTROL_CLKCTRL) & 0x03 ) != 0x02 );          

	VALUE_OF_REG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   
	while(( VALUE_OF_REG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                

	VALUE_OF_REG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   
	while(( VALUE_OF_REG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                

	VALUE_OF_REG(CM_L3_AON_CLKSTCTRL) |= 0x02;                                 
	while(( VALUE_OF_REG(CM_L3_AON_CLKSTCTRL) & 0x03 ) != 0x02 );              

	VALUE_OF_REG(CM_WKUP_ADC_TSC_CLKCTRL) |= 0x02;                             
	while(( VALUE_OF_REG(CM_WKUP_ADC_TSC_CLKCTRL) & 0x03 ) != 0x02 );          

	while(( VALUE_OF_REG(CM_WKUP_CONTROL_CLKCTRL) & (0x03<<16) ) != 0x00 );           
	while(( VALUE_OF_REG(CM_L3_AON_CLKSTCTRL) & (0x01<<3) ) == 0x00 );                 
	while(( VALUE_OF_REG(CM_WKUP_L4WKUP_CLKCTRL) & (0x03<<16) ) != 0x00 );             
	while(( VALUE_OF_REG(CM_WKUP_CLKSTCTRL) & (0x01<<2) ) == 0x00 );                   
	while(( VALUE_OF_REG(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & (0x01<<2) ) == 0x00 );    
	while(( VALUE_OF_REG(CM_WKUP_CLKSTCTRL) & (0x01<<14) ) == 0x00 );                  
	while(( VALUE_OF_REG(CM_WKUP_ADC_TSC_CLKCTRL) & (0x03<<16) ) != 0x00 );            
}



void ADC_Intr_Init(struct	dentry *devptr)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;   
	csrptr->threshold = 0X0FF;                                      
	
	/* reg. intr handler and set prio */
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                
	
	/* set ADC intr and prio to 10*/
	csrptr->ilr[devptr->dvirq] &= ~(0x01);                          
	csrptr->ilr[devptr->dvirq] |= (0x0A<<2);                        

}			

void ADC_Config(struct adc_reg_set *adc_reg)
{
	/* Config clock to 3 MHZ */
	ADC_Clock_Config(adc_reg, CLK_3MHZ);
	
	
	/* Make step config register writable */
	ADC_Config_Stepreg(adc_reg, 1);
	
	/* Setup step config register */
	Config_Step(adc_reg);
	
	/* Make ADC module works as general purpose */
	ADC_config_gen_purpose(adc_reg,ADC_GENERAL_PURPOSE_MODE);
	
	
	/* Clear all the interrupts */
	ADC_Clear_Intr(adc_reg);
	
	/* Set ADC INTR Bit */
	ADC_Intr_Set(adc_reg, ADC_INTR_MASK);
	
	
	//Enable ADC module
	adc_reg->ctrl |= 0x01;
}


void ADC_Clock_Config(struct adc_reg_set *adc_reg, unsigned int adc_clk_val)
{
	unsigned int clk_value = 24000000/adc_clk_val - 1;
	adc_reg->clk_value &= ~(0xFFFF);
	adc_reg->clk_value |= clk_value;
}

void ADC_Config_Stepreg(struct adc_reg_set *adc_reg, unsigned int val)
{
	adc_reg->ctrl &= ~ (0x01<<2);
	/*Modifying as writable */
	adc_reg->ctrl |= (val<<2);
}

void ADC_config_gen_purpose(struct adc_reg_set *adc_reg,unsigned int mode_val)
{
	adc_reg->ctrl &= ~ ADC_WORK_MODE_CLEAR;
	adc_reg->ctrl |= (mode_val<<ADC_WORK_MODE_SHIFT);
}


/* ADC steps enable and clear */
void ADC_Step_Enable(struct adc_reg_set *adc_reg, unsigned int step_val)
{
	adc_reg->step_val |= (0x01<<step_val);
}

void ADC_Step_Disable(struct adc_reg_set *adc_reg, unsigned int step_val)
{
	adc_reg->step_val &= ~(0x01<<step_val);
}

void ADC_Clear_Intr(struct adc_reg_set *adc_reg)
{
	adc_reg->irqStatus |= 0x7FFF;
}


void Config_Step(struct adc_reg_set *adc_reg)
{
	/* configure ADC steps */
	
	/* singel ended mode */
	ADC_Config_Step_OpMode(adc_reg,0,ADC_SINGLE_ENDED_OPER_MODE);	
	
	
	/* Apply channel and reference voltage */
	ADC_Step_Config_Chan_and_Voltage(adc_reg, 0, ADC_CHANNEL1, ADC_POSITIVE_REF_VDDA, ADC_CHANNEL1, ADC_NEGATIVE_REF_VSSA);
	
	/* Analog supplies */
	ADC_Config_Analog_Supplies(adc_reg, 0, 0, 0, 0);
	
	/* ANA ground */
	ADC_Config_Analog_Ground(adc_reg, 0, 0, 0, 0, 0);
	
	/* To config which ADC FIFO to use */
	ADC_Choose_FIFO(adc_reg, 0, ADC_FIFO_0);
	
	/* SW continuous mode */
	ADC_Config_Mode(adc_reg, 0, ADC_MODE_SW_CONTINUOUS);
	
	/* Take average on samples */
	ADC_Config_Avg(adc_reg,0,16);
}


void ADC_Config_Avg(struct adc_reg_set *adc_reg, int step_val, int sampleNum)
{
	if(sampleNum == 2)
		adc_reg->step[step_val].stepConfig |= 0x01<<2;
	else if(sampleNum == 4)
		adc_reg->step[step_val].stepConfig |= 0x02<<2;
	else if(sampleNum == 8)
		adc_reg->step[step_val].stepConfig |= 0x03<<2;	
	else if(sampleNum == 16)
		adc_reg->step[step_val].stepConfig |= 0x04<<2;
	else
		return;
}


/* Choose differntial or single ended mode operation */
void ADC_Config_Step_OpMode(struct adc_reg_set *adc_reg, unsigned int step_val, unsigned int mode)
{
	if(mode)
	{
		adc_reg->step[step_val].stepConfig |= ADC_STEPCONFIG_DIFF_CNTRL;
	}
	else
	{
		adc_reg->step[step_val].stepConfig &= ~ADC_STEPCONFIG_DIFF_CNTRL;
	}
}

/* For a given step, set up the reference voltage and input channel */

void ADC_Step_Config_Chan_and_Voltage(struct adc_reg_set *adc_reg, unsigned int step_val, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef)
{
	/* negative voltage reference */
	adc_reg->step[step_val].stepConfig &= ~ SEL_RFM_SWC_CLEAR;                
	adc_reg->step[step_val].stepConfig |= negativeRef<<SEL_RFM_SWC_SHIFT;     

	/* positive input channel */
	adc_reg->step[step_val].stepConfig &= ~ SEL_INP_SWC_CLEAR;                
	adc_reg->step[step_val].stepConfig |= positiveChannel<<SEL_INP_SWC_SHIFT; 

	/* negative input channel */
	adc_reg->step[step_val].stepConfig &= ~ SEL_INM_SWC_CLEAR;                
	adc_reg->step[step_val].stepConfig |= negativeChannel<<SEL_INM_SWC_SHIFT; 

	/* positive reference voltage */
	adc_reg->step[step_val].stepConfig &= ~ SEL_RFP_SWC_CLEAR;                
	adc_reg->step[step_val].stepConfig |= positiveRef<<SEL_RFP_SWC_SHIFT;     
}


void ADC_Config_Analog_Supplies(struct adc_reg_set *adc_reg, unsigned int step_val,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw)
{
	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_XPPSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |= xppsw<<ADC_STEPCONFIG_XPPSW_SWC_SHIFT;

	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_XNPSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |= xnpsw<<ADC_STEPCONFIG_XNPSW_SWC_SHIFT;

	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_YPPSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |= yppsw<<ADC_STEPCONFIG_YPPSW_SWC_SHIFT;
}

/* To configure pull down voltage */
void ADC_Config_Analog_Ground(struct adc_reg_set *adc_reg, unsigned int step_val,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw)
{
	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_XNNSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |=  xnnsw<<ADC_STEPCONFIG_XNNSW_SWC_SHIFT;

	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_YPNSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |=  ypnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_YNNSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |=  ynnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	adc_reg->step[step_val].stepConfig &= ~ ADC_STEPCONFIG_WPNSW_SWC_CLEAR;
	adc_reg->step[step_val].stepConfig |=  wpnsw<<ADC_STEPCONFIG_WPNSW_SWC_SHIFT;	
}

/* Control function to choose FIFO */
void ADC_Choose_FIFO(struct adc_reg_set *adc_reg, unsigned int step_val,unsigned int fifoNO)
{
	adc_reg->step[step_val].stepConfig &= ~ ADC_FIFO_SELECTION_CLEAR;
	adc_reg->step[step_val].stepConfig |= fifoNO<<ADC_FIFO_SELECTION_SHIFT;
}


void ADC_Config_Mode(struct adc_reg_set *adc_reg, unsigned int step_val,unsigned int mode)
{
	adc_reg->step[step_val].stepConfig &= ~ ADC_MODE_CLEAR;
	adc_reg->step[step_val].stepConfig |= mode<<ADC_MODE_SHIFT;
}


void ADC_Intr_Set(struct adc_reg_set *adc_reg, unsigned int irq_en_set)
{
	adc_reg->irq_en_set |= irq_en_set;
}


devcall	adcinit (struct	dentry *devptr)
{
	
	/* Initialize register addresses */
	struct adc_reg_set* adc_reg = (struct adc_reg_set*)devptr->dvcsr;
		
	adc_sem = semcreate(0);										
	
	/* Initialize ADC clk */
	ADC_Clk_Init();
	
	/* Init ADC interrupt configuration */
	ADC_Intr_Init(devptr);
	
	/* Configuration of ADC block */
	ADC_Config(adc_reg);
	
	return 1;
}
