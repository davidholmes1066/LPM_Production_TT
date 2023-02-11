/*
 * handle.c
 *
 * Created: 18/05/2022 14:33:50
 *  Author: david
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "handle.h"
#include "serialF0.h"

#define DAC_VREF 1
#define ADC_VREF 1

extern int PTimeOut;
extern int TC_samples;
extern uint16_t PTime[6];
extern int16_t VOLT[7];
extern int16_t IT[3];
extern char INPUTBUF[3];
extern int16_t InpVar;
extern char OUTPUTBUF[5];
extern int16_t OutVar;
extern uint16_t D0_raw[SAMPLE_WINDOW];

uint8_t read_UART(void)
{
	int test = 0;
	uint8_t garbage;
	uint8_t data;
	
	while(test==0)
	{
		test = USARTE0.STATUS&(0x80);													//keep polling until RXCIF bit is high
	}
	
	data = USARTE0.DATA;																//read the data from the data RX/TX register
	
	while(test!=0)
	{
		test = USARTE0.STATUS&(0x80);													//keep polling until RXCIF bit is low
		garbage = USARTE0.DATA;															//read the data until register is automatically cleared
	}
	
	return data;																		//returns 8bit received data
}

void write8_UART(uint8_t data)
{
	int test = 0;
	while(test==0)
	{
		test = USARTE0.STATUS&(0x20);													//keep polling until the data register is ready to receive new data
	}
	
	USARTE0.DATA = data;																//write the 8bits to MATLAB
	
	test = 0;
	while(test==0)
	{
		test = USARTE0.STATUS&(0x20);													//keep polling until the data register is ready to receive new data
	}
}

void write_UART(uint16_t data)
{
	uint16_t high;
	uint8_t datah, datal;
	high = data&(0xF0);																	//find what high bits are set
	datal = data&(0x0F);																//find what low bits are set
	datah = (high>>8);
	
	
	int test = 0;
	while(test==0)
	{
		test = USARTE0.STATUS&(0x20);													//keep polling until the data register is ready to receive new data
	}
	
	test = 0;
	USARTE0.DATA = datal;																//write the 8 low bits
	while(test==0)
	{
		test = USARTE0.STATUS&(0x20);													//keep polling until the data register is ready to receive new data
	}
	
	USARTE0.DATA = datah;																//write the 8 high bits
	
	test = 0;
	while(test==0)
	{
		test = USARTE0.STATUS&(0x20);													//keep polling until the data register is ready to receive new data
	}
	
}

void stop_timer(void)
{
	TCE0.CTRLA = TC_CLKSEL_OFF_gc;														//Turn timer off
	TCE0.INTCTRLA = (TCE0.INTCTRLA & ~TC0_OVFINTLVL_gm) | TC_OVFINTLVL_OFF_gc;			//Trun interrupt off
	TCE0.CNT = 0;
}

void start_timer(void)
{
	TCE0.CNT = 0;
	TCE0.INTCTRLA = (TCE0.INTCTRLA & ~TC0_OVFINTLVL_gm) | TC_OVFINTLVL_LO_gc;			//Turn on overflow interrupt
	TCE0.CTRLA = TC_CLKSEL_DIV256_gc;													//turn on TC
}

void start_DELAY(void)
{
	TCD1.CNT = 0;																		//set count to 0
	TCD1.CTRLA = TC_CLKSEL_DIV64_gc;													//set clock frequency to 32Mhz/64 = 500kHz
}

void stop_DELAY(void)
{
	TCD1.CTRLA = TC_CLKSEL_OFF_gc;														//stop counter
	TCD1.CNT = 0;																		//set count to 0
}


#ifndef DISABLE_IR_DO
void enable_D0(void)
{
																						//D0
	PORTD_INT0MASK = PIN2_bm;															//Set port D interrupt mask 0 to PD2
	PORTD.PIN2CTRL = PORT_ISC_FALLING_gc;												//Set interrupt to falling edge
	PORTD.INTCTRL = PORT_INT0LVL_LO_gc;													//Set port D interrupt mask 0 to low level
}

void disable_D0(void)
{
	PORTD.INTCTRL = PORT_INT0LVL_OFF_gc;												//Set port D interrupt mask 0 OFF
}

void enable_D1(void)
{
																						//D1
	PORTD_INT1MASK = PIN3_bm;															//Set port D interrupt mask 1 to PD3
	PORTD.PIN3CTRL = PORT_ISC_FALLING_gc;												//Set interrupt to falling edge
	PORTD.INTCTRL = PORT_INT1LVL_LO_gc;													//Set port D interrupt mask 1 to low level
}

void disable_D1(void)
{
	PORTD.INTCTRL = PORT_INT1LVL_OFF_gc;												//Set port D interrupt mask 1 to low level
}

uint16_t get_D0(void)
{
	uint16_t PAVR = 0;
	start_timer();
	enable_D0();
	while((TC_samples < 6)&(PTimeOut == 0))	
	{											
																						//Do nothing until 6 samples are collected or ovf occurs
	}
	disable_D0();
	stop_timer();																		//Reset timeout
	TC_samples = 0;
	
	if(PTimeOut == 0)
	{
		for(int n = 0; n < 5; n++)
		{
			PAVR = PAVR + (uint16_t)PTime[n+1] - (uint16_t)PTime[n];
		}
		
		PAVR = PAVR/5;
		
		PTimeOut = 0;
		
		return PAVR;
	}
	
	else
	{
		PTimeOut = 0;
		
		return 0;
	}
	
	
}

uint16_t get_D1(void)
{
	uint16_t PAVR = 0;
	start_timer();
	enable_D1();
	while((PTimeOut == 0)&(TC_samples<6))	
	{
																						//Do nothing until 6 samples are collected or ovf occurs
	}											
	disable_D1();
	stop_timer();																		//Reset timeout
	TC_samples = 0;
	
	if(PTimeOut == 0)
	{
		for(int n = 0; n < 5; n++)
		{
			PAVR = PAVR + (uint16_t)PTime[n+1] - (uint16_t)PTime[n];
		}
		
		PAVR = PAVR/5;
		
		PTimeOut = 0;
		
		return PAVR;
	}
	
	else
	{
		PTimeOut = 0;
		
		return 0;
	}
	
	
}

#endif

void read_voltages(void)
{
	
	uint8_t INTFA = (0xF), INTFB = (0xF);
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCB clock speed to 62.5kHz
	
	ADCA.CTRLA = ADC_FLUSH_bm;														//Flushes ADCA
	ADCB.CTRLA = ADC_FLUSH_bm;														//Flushes ADCB
	
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;										//Set ADCA CH1 to pin A1 as input for CORE_VDD
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;										//Set ADCA CH0 to pin A0 as input for VCC_main
	ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;										//Set ADCA CH2 to pin A2 as input for +1.8_VDD
	ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;										//Set ADCA CH3 to pin A3 as input for +3.0VIO
	
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;										//Set ADCB CH0 to pin B0 as input for VPP_2V5
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;										//Set ADCB CH1 to pin B1 as input for +1.8VIO
	ADCB.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;										//Set ADCB CH2 to pin B4 as input for BLE_VDD
	
	ADCA.CTRLA = ADC_ENABLE_bm;														//Enables ADCA
	ADCB.CTRLA = ADC_ENABLE_bm;														//Enables ADCB
	
	ADCA.CTRLA |= ADC_DMASEL_CH0123_gc;							 					//Allow DMA request all channels
	ADCB.CTRLA |= ADC_DMASEL_CH012_gc;												//Allow DMA request channel 0,1,2
	
	ADCA.CTRLA |= (0x0F << 2);							 							//Start ADCA conversion on all 4 channels
	ADCB.CTRLA |= (0x07 << 2);														//Start ADCB conversion on first three channels
	
	INTFA = (ADCA.INTFLAGS & (0x0F));
	INTFB = (ADCB.INTFLAGS & (0x0F));
	
	//write8_UART(INTFA);															//Debug print interrupt flags ADCB
	//write8_UART(INTFB);															//Debug print interrupt flags ADCA
	
	//while( !(ADCA.INTFLAGS & ADCB.INTFLAGS & (0x0F)))								//Wait for ADC conversion
	//while( ((ADCA.INTFLAGS != 0xF) && (ADCB.INTFLAGS != 0xF)))					//Wait for ADC conversion
	while( ((INTFA != (0xF)) && (INTFB != (0x7))))									//Wait for ADC conversion
	{
		INTFA = (ADCA.INTFLAGS & (0x0F));
		INTFB = (ADCB.INTFLAGS & (0x0F));
	}
	
	//INTFA = (ADCA.INTFLAGS & (0x0F));
	//INTFB = (ADCB.INTFLAGS & (0x0F));
	
	//write8_UART(INTFA);															//Debug print interrupt flags ADCB
	//write8_UART(INTFB);															//Debug print interrupt flags ADCA
	
	//TIMEOUT();
	
	VOLT[0] = (int16_t)ADCA.CH0.RES;												//Read VCC_main
	VOLT[1] = (int16_t)ADCA.CH1.RES;												//Read CORE_VDD
	VOLT[2] = (int16_t)ADCA.CH2.RES;												//Read +1.8_VDD
	VOLT[3] = (int16_t)ADCA.CH3.RES;												//Read +3.0VIO	
	
	VOLT[4] = (int16_t)ADCB.CH0.RES;												//Read VPP_2V5
	VOLT[5] = (int16_t)ADCB.CH1.RES;												//Read +1.8VIO
	VOLT[6] = (int16_t)ADCB.CH2.RES;												//Read BLE_VDD
	
	ADCA.INTFLAGS |= (0x0F);														//clear interrupt flags ADCA
	ADCB.INTFLAGS |= (0x0F);														//clear interrupt flags ADCB
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV8_gc;											//Get clock ADC ready for digital measurements
	ADCB.PRESCALER = ADC_PRESCALER_DIV8_gc;											//Get clock ADC ready for digital measurements
	
	ADCA.CTRLA = ADC_FLUSH_bm;														//Flushes ADCA
	ADCB.CTRLA = ADC_FLUSH_bm;														//Flushes ADCB
}

void fread_D0(void)
{
	uint8_t hmemory = 1;															//keeps track if high signal is already counted
	uint32_t SamplesRisingEdge[11];													//keeps track of samples at rising edge
	uint32_t cnt = 0;																//counter
	uint8_t Npulses = 0;															//keeps track of detected pulses
	uint32_t sum = 0;																//used for calculating average amount of samples
	float avrageT;																	//average pulse frequency in samples
	uint8_t *sendarray;																//creates an array with 4*8bit values from a float
			
	ADCA.PRESCALER = ADC_PRESCALER_DIV8_gc;											//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV8_gc;											//2MHz system clock use: ADCB clock speed to 62.5kHz		
		
	ADCB.CTRLA = ADC_FLUSH_bm;														//Flushes ADCB
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;										//Set ADCB CH2 to pin B5 as input for D0
	ADCB.CTRLA = ADC_ENABLE_bm;														//Enables ADCB
	
	//TIMEOUT();
	
	for(uint32_t i = 0; i < SAMPLE_WINDOW; i++)
	{
		ADCB.CTRLA |= (0x07 << 2);													//Start ADCB conversion on first channel
		while (!(ADCB.INTFLAGS & 0x1))												//wait for conversion
		{
			//do nothing
		}
		
		D0_raw[i] = (int16_t)ADCB.CH0.RES;											//fill array with raw data
		ADCB.INTFLAGS |= (0x0F);													//clear interrupt flags ADCB
	}
	
	for(uint32_t i = 0; i < SAMPLE_WINDOW; i++)
	{
		write8_UART((uint8_t)D0_raw[i]);											//debug print low byte
		write8_UART((uint8_t)(D0_raw[i]>>8));										//debug print high byte
	}
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCB clock speed to 62.5kHz
	
	ADCA.CTRLA = ADC_FLUSH_bm;														//Flushes ADCA
	ADCB.CTRLA = ADC_FLUSH_bm;														//Flushes ADCB
	
	while ((cnt<SAMPLE_WINDOW) && (Npulses < 11))
	{
		if((D0_raw[cnt] > 1000) && (hmemory > 20))
		{
			SamplesRisingEdge[Npulses] = cnt;										//note sample rising edge
			hmemory = 1;															//prevent second trigger on same pulse
			PORTE_OUTSET = PIN4_bm;													//debug pin
			Npulses++;
			hmemory = 0;
		}
		
		else if ((D0_raw[cnt]<1000))
		{
			hmemory++;																//Reset latch
			PORTE_OUTCLR = PIN4_bm;													//debug pin
		}
		
		cnt++;																		//add cycle
	}
	
	if (Npulses == 11)
	{
		for(int i = 0; i < 9; i++)
		{
			sum = sum + (SamplesRisingEdge[i+2] - SamplesRisingEdge[i+1]);			//calculates sum of trigger intervals
			//write8_UART((uint8_t)(SamplesRisingEdge[i+1]));						//debug print
			//write8_UART((uint8_t)(SamplesRisingEdge[i+1]>>8));					//debug print
		}
		avrageT = (float)sum/8;														//calculates average time in samples
// 		write8_UART((uint8_t)(sum/8));
// 		write8_UART((uint8_t)((sum/8)>>8));
		sendarray = (uint8_t*)(&avrageT);											//converts float to 4 seperate bytes in send array
		write8_UART(sendarray[0]);
		write8_UART(sendarray[1]);
		write8_UART(sendarray[2]);
		write8_UART(sendarray[3]);
	}
	
	else
	{
		write8_UART((0xFF));														//default error code
		write8_UART((0xFF));
		write8_UART((0xFF));														//default error code
		write8_UART((0xFF));
	}	
}

void read_IT(void)
{
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCB clock speed to 62.5kHz
	
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;										//Set ADCA CH0 to Pin A4 as input for I_external
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;										//Set ADCA CH1 to Pin A5 as input for I_battery
	
	//ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;										//Set ADCB CH0 to Pin B5 as input for Temp_ambient

	ADCA.CTRLA |= ADC_FLUSH_bm;														//Enables ADCA
	ADCB.CTRLA |= ADC_FLUSH_bm;														//Enables ADCB
	
	ADCA.CTRLA |= ADC_ENABLE_bm;													//Enables ADCA
	ADCB.CTRLA |= ADC_ENABLE_bm;													//Enables ADCB
	
	//TIMEOUT();
	
	
	ADCA.CTRLA |= (0x03 << 2);														//Start ADCA Conversion first two channels
	ADCB.CTRLA |= (0x01 << 2);														//Start ADCB conversion CH0
	
	while( !((ADCA.INTFLAGS & 0x3) & (ADCB.INTFLAGS & 0x01)))                       //Wait for ADC conversion
	
	TIMEOUT();
	
	IT[0] = (int16_t)ADCA.CH0.RES;													//Read I_external
	IT[1] = (int16_t)ADCA.CH1.RES;													//Read I_battery
	
	IT[2] = (int16_t)ADCB.CH0.RES;													//Read Temp_ambient
	
	ADCA.INTFLAGS |= (0x03);														//clear interrupt flags ADCA
	ADCB.INTFLAGS |= (0x01);														//clear interrupt flags ADCB
}

double calc_DACDATA(double dac_output)
{
	double dac_data;
	dac_data = (dac_output*4095)/DAC_VREF;											//calculate dac_channel_data for desired DAC output voltage
	return dac_data;
}

uint16_t read_supply_ext(void)
{
	uint16_t current[10];
	uint16_t average_I;
	uint16_t sum = 0;																//Big enough sum of 10 12bit uints;
	
//	supply_ext();	
	PORTB_OUTSET = PIN2_bm;															//supply bat
	TIMEOUT();
	PORTB_OUTCLR = PIN3_bm;															//disable ext
	TIMEOUT();
	
	for(int i = 0; i < 10; i++)														//Get 10 samples over 1 second
	{
		read_IT();
		current[i] = IT[0];
		TIMEOUT();
	}
	
	for(int i = 0; i < 10; i++)
	{
		sum += current[i];
	}
	
	average_I = sum/10;
	
	return average_I;
}

void write_bat(void)
{
	uint16_t I_bat;
	uint16_t bit_test;
	uint8_t hbyte;																	//High 8 bits I_bat
	uint8_t lbyte;																	//Low 8 bits I_bat
	
	I_bat = read_supply_bat();														//Enables battery supply and reads the average current
	lbyte = I_bat&(0xFF);															//Check for set bits lower byte
	
	write8_UART(lbyte);																//Send lower byte
	
	bit_test = I_bat&(0xFF00);														//test 8 set bits high byte
	bit_test >>= 8;																	//Bit shift high byte to low byte register
	hbyte = (uint8_t)bit_test;														//typecast to 1 register
	
	write8_UART(hbyte);																//Send high byte											
}

void write_ext(void)
{
	uint16_t I_ext;
	uint16_t bit_test;
	uint8_t hbyte;																	//High 8 bits I_bat
	uint8_t lbyte;																	//Low 8 bits I_bat
	
	I_ext = read_supply_ext();														//Enables battery supply and reads the average current
	lbyte = I_ext&(0xFF);															//Check for set bits lower byte
	
	write8_UART(lbyte);																//Send lower byte
	
	bit_test = I_ext&(0xFF00);														//test 8 set bits high byte
	bit_test >>= 8;																	//Bit shift high byte to low byte register
	hbyte = (uint8_t)bit_test;														//typecast to 1 register
	
	write8_UART(hbyte);																//Send high byte
}

void write_volt(void)
{
	uint16_t bit_test;
	uint8_t hbyte;																	//High 8 bits I_bat
	uint8_t lbyte;
	
	read_voltages();
	
	for(int i = 0; i < 7; i ++)
	{
		lbyte = VOLT[i]&(0xFF);														//Check for set bits lower byte
		
		write8_UART(lbyte);															//Send lower byte
		
		bit_test = VOLT[i]&(0xFF00);												//test 8 set bits high byte
		bit_test >>= 8;																//Bit shift high byte to low byte register
		hbyte = (uint8_t)bit_test;													//typecast to 1 register
		
		write8_UART(hbyte);															//Send high byte
		
		bit_test = 0;																//Reset just to be safe
		hbyte = 0;
		lbyte = 0;
	}
	
}

#ifndef DISABLE_IR_DO
void write_D0(void)
{
	uint16_t f_time;
	uint16_t bit_test;
	uint8_t hbyte;																	//High 8 bits I_bat
	uint8_t lbyte;																	//Low 8 bits I_bat
	
	f_time = get_D0();																//Enables battery supply and reads the average current
	lbyte = f_time&(0xFF);															//Check for set bits lower byte
	
	write8_UART(lbyte);																//Send lower byte
	
	bit_test = f_time&(0xFF00);														//test 8 set bits high byte
	bit_test >>= 8;																	//Bit shift high byte to low byte register
	hbyte = (uint8_t)bit_test;														//typecast to 1 register
	
	write8_UART(hbyte);																//Send high byte
}

void write_D1(void)
{
	uint16_t f_time;
	uint16_t bit_test;
	uint8_t hbyte;																	//High 8 bits I_bat
	uint8_t lbyte;																	//Low 8 bits I_bat
	
	f_time = get_D1();																//Enables battery supply and reads the average current
	lbyte = f_time&(0xFF);															//Check for set bits lower byte
	
	write8_UART(lbyte);																//Send lower byte
	
	bit_test = f_time&(0xFF00);														//test 8 set bits high byte
	bit_test >>= 8;																	//Bit shift high byte to low byte register
	hbyte = (uint8_t)bit_test;														//typecast to 1 register
	
	write8_UART(hbyte);																//Send high byte
}
#endif

uint16_t read_supply_bat(void)
{
	uint16_t current[10];
	uint16_t average_I;
	uint16_t sum = 0;																//Big enough sum of 10 12bit uints;
	
	//supply_bat();
	PORTB_OUTSET = PIN3_bm;															//enable ext
	TIMEOUT();
	PORTB_OUTCLR = PIN2_bm;															//disable bat
	TIMEOUT();
	
	for(int i = 0; i < 10; i++)														//Get 10 samples over 1 second
	{
		read_IT();
		current[i] = IT[1];
		TIMEOUT();
	}
	
	for(int i = 0; i < 10; i++)
	{
		sum += current[i];
	}
	
	average_I = sum/10;
	
	return average_I;
}

void TIMEOUT(void)
{
	start_DELAY();
	while(TCD1.CNT<50000)															//Do nothing for 0.1sec
	{
		//Do nothing
	}
	stop_DELAY();
}

#ifndef dis_dac
void supply_bat(void)
{
	DACB.CH0DATA = calc_DACDATA(1.00);												//Set DAC channel 0 to 1V
	TIMEOUT();
	DACB.CH1DATA = calc_DACDATA(0.00);												//Set DAC channel 1 to 0V
}

void supply_ext(void)
{
	DACB.CH1DATA = calc_DACDATA(1.00);												//Set DAC channel 1 to 1V
	TIMEOUT();
	DACB.CH0DATA = calc_DACDATA(0.00);												//Set DAC channel 0 to 0V
}
#endif

void LPM_P_OFF(void)
{
	PORTB_OUTCLR = PIN2_bm;															//turn battery supply off
	PORTB_OUTCLR = PIN3_bm;															//turn external supply off 
}
