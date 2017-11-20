#ifndef ADC_H
#define ADC_H

struct adc_reg_set
{
	/* dummy registers to fetch the addresses properly */
	volatile uint32 rev;
	
	/* These reserve values are to padd so that addresses 
		align with the base and matches reg offset address */
	volatile uint32 reserve1[3];
	volatile uint32 sys_config;
	volatile uint32 reserve2[4];
	volatile uint32 irqStatus_raw;
	
	/* To handle intr status */
	volatile uint32 irqStatus;
	volatile uint32 irq_en_set;	
	
	volatile uint32 irq_en_clr;
	volatile uint32 irq_wakeup;
	volatile uint32 dma_enable_set;
	volatile uint32 dma_enable_clr;
	
	
	/* check what's this for */
	volatile uint32 ctrl;
	
	
	volatile uint32 adc_stat;
	volatile uint32 adc_range;
	
	/* To handle clock configuration */
	volatile uint32 clk_value;
	
	volatile uint32 adc_misc;
	
	
	/* To configure ADC step */
	volatile uint32 step_val;
	
	
	volatile uint32 idle_config;
	volatile uint32 ts_charge_stepconfig;
	volatile uint32 ts_charge_delay;
	
	struct 
	{
		volatile uint32 stepConfig;
		volatile uint32 stepDelay;
	}step[16];
	
		
	struct 
	{
		volatile uint32 fifo_count;
		volatile uint32 fifoThreshold;
		volatile uint32 dmaReq;
	}fifoInfo[2];
	
	
	volatile uint32 reserve3;
	volatile uint32 fifo_data;
	
	volatile uint32 reserve4[63];
	volatile uint32 fifo_data1;
	
	
};


devcall ledinit(struct dentry *devptr);
devcall ledwrite(struct dentry	*devptr, char *buff, int32 val);
devcall	adcread(struct dentry* devptr, char* buff, int32 count);
devcall	adcinit (struct	dentry *devptr);
void adc_intr_handler(uint32 num);

#define ADC_BASE_REG_ADDR					(0x44E0D000)

#define ADC_IRQ_STATUS_REG_ADDR  			(ADC_BASE_REG_ADDR + 0x28)
#define ADC_CTRL_REG_ADDR  					(ADC_BASE_REG_ADDR + 0x40)
#define ADC_CLK_REG_ADDR 					(ADC_BASE_REG_ADDR + 0x4C)
#define ADC_STEP_VALUE_REG_ADDR 			(ADC_BASE_REG_ADDR + 0x54)
#define ADC_STEP_CONFIG0_REG_ADDR			(ADC_BASE_REG_ADDR + 0x64)
#define ADC_STEP_DELAY0_REG_ADDR 			(ADC_BASE_REG_ADDR + 0x68)


#define ADC_STEPCONFIG_DIFF_CNTRL   (0x01<<25)

/* Macros to choose ADC step mode operation */
#define ADC_SINGLE_ENDED_OPER_MODE  (0)
#define ADC_DIFFERENTIAL_OPER_MODE	(1)

/* Corresponding ADC Channels AIN0, AIN1 .. */
#define ADC_CHANNEL1            	(0)			
#define ADC_CHANNEL2            	(1) 		
#define ADC_CHANNEL3            	(2)   		
#define ADC_CHANNEL4            	(3)   		
#define ADC_CHANNEL5            	(4)   		
#define ADC_CHANNEL6            	(5) 		
#define ADC_CHANNEL7            	(6) 		
#define ADC_CHANNEL8            	(7)			


//SEL_RFM_SWC field in step config register
#define ADC_NEGATIVE_REF_VSSA		(0)			//SEL_RFM pins SW configuration (23:24)
#define ADC_NEGATIVE_REF_XNUR		(1)
#define ADC_NEGATIVE_REF_YNLR		(2)
#define ADC_NEGATIVE_REF_VREFN		(3)

#define ADC_POSITIVE_REF_VDDA		(0)			//SEL_RFP pins SW configuration (12:14)
#define ADC_POSITIVE_REF_XPUL		(1)
#define ADC_POSITIVE_REF_YPLL		(2)
#define ADC_POSITIVE_REF_VREFP		(3)

#define SEL_RFM_SWC_CLEAR			(0X03<<23)	//& ~SEL_RFM_SWC_CLEAR will clear the SEL_RFM_SWC field
#define SEL_RFM_SWC_SHIFT			(23)		//SEL_RFM_SWC is at 23:24 bit

//SEL_INP_SWC field in step config register
#define SEL_INP_SWC_CLEAR			(0XF<<19)	//& ~SEL_INP_SWC_CLEAR will clear the SEL_INP_SWC field
#define SEL_INP_SWC_SHIFT			(19)		//SEL_INP_SWC is at 19:22 bit

//SEL_INM_SWC field in step config register
#define SEL_INM_SWC_CLEAR			(0X0F<<15)	//& ~SEL_INM_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_INM_SWC_SHIFT			(15)		//SEL_INM_SWC is at 15:18 bit

//SEL_RFP_SWC field in step config register
#define SEL_RFP_SWC_CLEAR			(0x07<<12)	//& ~SEL_RFP_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_RFP_SWC_SHIFT			(12)		//SEL_RFP_SWC is at 15:18 bit


#define ADC_STEPCONFIG_WPNSW_SWC_CLEAR   		(0x01<<11)
#define ADC_STEPCONFIG_WPNSW_SWC_SHIFT  		(0x0000000Bu)

#define ADC_STEPCONFIG_XNNSW_SWC_CLEAR   		(0x01<<6)
#define ADC_STEPCONFIG_XNNSW_SWC_SHIFT  		(0x00000006u)

#define ADC_STEPCONFIG_XNPSW_SWC_CLEAR   		(0x01<<9)
#define ADC_STEPCONFIG_XNPSW_SWC_SHIFT  		(0x00000009u)

#define ADC_STEPCONFIG_XPPSW_SWC_CLEAR   		(0x01<<5)
#define ADC_STEPCONFIG_XPPSW_SWC_SHIFT  		(0x00000005u)

#define ADC_STEPCONFIG_YNNSW_SWC_CLEAR   		(0x01<<8)
#define ADC_STEPCONFIG_YNNSW_SWC_SHIFT  		(0x00000008u)

#define ADC_STEPCONFIG_YPNSW_SWC_CLEAR   		(0x01<<10)
#define ADC_STEPCONFIG_YPNSW_SWC_SHIFT  		(0x0000000Au)

#define ADC_STEPCONFIG_YPPSW_SWC_CLEAR   		(0x01<<7)
#define ADC_STEPCONFIG_YPPSW_SWC_SHIFT  		(0x00000007u)



// FIFO field
#define ADC_FIFO_0                           	(0)
#define ADC_FIFO_1                           	(1)
#define ADC_FIFO_SELECTION_CLEAR			 	(0X01<<26)
#define ADC_FIFO_SELECTION_SHIFT			 	(26)

//MODE field
#define ADC_MODE_CLEAR						 	(0X03)
#define ADC_MODE_SW_ONE_SHOT					(0X00)
#define ADC_MODE_SW_CONTINUOUS					(0x01)
#define ADC_MODE_HW_ONE_SHOT					(0X10)
#define ADC_MODE_HW_CONTINUOUS					(0x11)
#define ADC_MODE_SHIFT							(0x00)

//ADC work mode
#define ADC_FOUR_WIRE_MODE                   	(1)		//for touch screen
#define ADC_FIVE_WIRE_MODE                   	(2)		//for touch screen
#define ADC_GENERAL_PURPOSE_MODE             	(0)		//for general purpose
#define ADC_WORK_MODE_CLEAR						(0x03<<5)
#define ADC_WORK_MODE_SHIFT						(5)		


/* ADC interrupt bit set */
#define ADC_INTR_MASK				(0x01<<1)	
#define CLK_3MHZ 					3000000


void ADC_Config_Avg(struct adc_reg_set *adc_reg, int step_val, int sampleNum);

void ADC_Config_Mode(struct adc_reg_set *adc_reg, unsigned int step_val,
					unsigned int mode);

void ADC_Choose_FIFO(struct adc_reg_set *adc_reg, unsigned int step_val,
					unsigned int fifoNO);

void ADC_Config_Analog_Ground(struct adc_reg_set *adc_reg, unsigned int step_val,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw);


void ADC_Config_Analog_Supplies(struct adc_reg_set *adc_reg, unsigned int step_val,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw);

									
void ADC_Step_Config_Chan_and_Voltage(struct adc_reg_set *adc_reg, unsigned int step_val, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef);

		
void ADC_Config_Step_OpMode(struct adc_reg_set *adc_reg, unsigned int step_val, unsigned int mode);

void ADC_config_gen_purpose(struct adc_reg_set *adc_reg,unsigned int mode_val);

void Config_Step(struct adc_reg_set *adc_reg);

void ADC_Config_Stepreg(struct adc_reg_set *adc_reg, unsigned int val);

void ADC_Clock_Config(struct adc_reg_set *adc_reg, unsigned int adc_clk_val);

void ADC_Clear_Intr(struct adc_reg_set *adc_reg);
void ADC_Intr_Set(struct adc_reg_set *adc_reg, unsigned int irq_en_set);


void ADC_Config(struct adc_reg_set *adc_reg);
void ADC_Intr_Init(struct	dentry *devptr);
void ADC_Clk_Init();

/* To configure ADC step */
void ADC_Step_Enable(struct adc_reg_set *adc_reg, unsigned int step_val);
void ADC_Step_Disable(struct adc_reg_set *adc_reg, unsigned int step_val);
#endif
