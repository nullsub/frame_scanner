#include "scanner.h"
#include "tprintf.h"

#define STEPS_PER_REV	200

void do_half_step(int direction);
void do_full_step(int direction);

void scan_hw_init()
{



	ADC_InitTypeDef  ADC_InitStructure;
	/* PCLK2 is the APB2 clock */
	/* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	/* Enable ADC1 clock so that we can talk to it */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Put everything back to power-on defaults */
	ADC_DeInit(ADC1);

	/* ADC1 Configuration ------------------------------------------------------*/
	/* ADC1 and ADC2 operate independently */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/* Disable the scan conversion so we do one at a time */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	/* Don't do contimuous conversions - do them on demand */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	/* Start conversin by software, not an external trigger */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/* Conversions are 12 bit - put them in the lower 12 bits of the result */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/* Say how many channels would be used by the sequencer */
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	/* Now do the setup */
	ADC_Init(ADC1, &ADC_InitStructure);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));


	MOTOR_PORT->BRR = MOTOR_PIN_0; //clear
	MOTOR_PORT->BRR = MOTOR_PIN_1; //clear
	MOTOR_PORT->BRR = MOTOR_PIN_2; //clear
	MOTOR_PORT->BRR = MOTOR_PIN_3; //clear
	do_full_step(1);
}

void step_motor(int steps)
{
	for(int i = 0; i < steps; i++) {
		do_full_step(1);
	}
}

int get_distance()
{

	ADC_RegularChannelConfig(ADC1, 1, 1, ADC_SampleTime_1Cycles5);
	// Start the conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// Wait until conversion completion
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// Get the conversion value
	int adc_val =  ADC_GetConversionValue(ADC1);
	return adc_val;
}

void scan()
{
	for(int i = 0; i < STEPS_PER_REV; i++) {
		tprintf("%i\n", get_distance());
		step_motor(1);
	}
}

const int motor_pins[4] = {
	MOTOR_PIN_0, MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3
};

void do_half_step(int direction)
{
	static int crrnt_step = 0;

	if(direction == 0) {
		crrnt_step ++;
		if (crrnt_step >= 8)
			crrnt_step = 0;
	}
	else{
		if(crrnt_step == 0)
			crrnt_step = 8;
		crrnt_step --;
	}

	//BRR is clear and BSRR is set half step is probably broken.. I dont use it.
	switch(crrnt_step) {
		case 0:
			MOTOR_PORT->BRR = motor_pins[0];
			MOTOR_PORT->BSRR = motor_pins[3];
			break;
		case 1:
			MOTOR_PORT->BRR = motor_pins[0] | motor_pins[2];
			;
			break;
		case 2:
			MOTOR_PORT->BRR = motor_pins[2];
			MOTOR_PORT->BSRR = motor_pins[0];
			break;
		case 3:
			MOTOR_PORT->BRR = motor_pins[1] | motor_pins[2];
			break;
		case 4:
			MOTOR_PORT->BRR = motor_pins[1];
			MOTOR_PORT->BSRR = motor_pins[2];
			break;
		case 5:
			MOTOR_PORT->BRR = motor_pins[1] | motor_pins[3];
			break;
		case 6:
			MOTOR_PORT->BRR = motor_pins[3];
			MOTOR_PORT->BSRR = motor_pins[1];
			break;
		case 7:
			MOTOR_PORT->BRR = motor_pins[0] | motor_pins[3];
			break;
	}
}

void do_full_step(int direction)
{
	static unsigned int crrnt_step = 0;

	if(direction == 0) {
		crrnt_step ++;
		if (crrnt_step >= 4)
			crrnt_step = 0;
	}
	else{
		if(crrnt_step == 0)
			crrnt_step = 4;
		crrnt_step --;
	}

	switch(crrnt_step) {
		case 0:
			MOTOR_PORT->BRR = motor_pins[0] | motor_pins[2];
			MOTOR_PORT->BSRR = motor_pins[3] | motor_pins[1];
			break;
		case 1:
			MOTOR_PORT->BRR = motor_pins[1] | motor_pins[2];
			MOTOR_PORT->BSRR = motor_pins[0] | motor_pins[3];
			break;
		case 2:
			MOTOR_PORT->BRR = motor_pins[1] | motor_pins[3];
			MOTOR_PORT->BSRR = motor_pins[2] | motor_pins[0];
			break;
		case 3:
			MOTOR_PORT->BRR = motor_pins[0] | motor_pins[3];
			MOTOR_PORT->BSRR = motor_pins[1] | motor_pins[2];
			break;
	}
}
