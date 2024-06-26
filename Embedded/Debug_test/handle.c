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

#define DAC_VREF 1

#define ADC_VREF 3.0
#define ADC_OFFSET 0.3 
#define ADC_LIN 1

extern int PTimeOut;
extern int TC_samples;
extern uint16_t PTime[6];
extern float VOLT[7];
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
	
	while( ((INTFA != (0xF)) && (INTFB != (0x7))))									//Wait for ADC conversion
	{
		INTFA = (ADCA.INTFLAGS & (0x0F));
		INTFB = (ADCB.INTFLAGS & (0x0F));
	}
	
	VOLT[0] = (float)ADCA.CH0.RES;													//Read VCC_main
	VOLT[1] = (float)ADCA.CH1.RES;													//Read CORE_VDD
	VOLT[2] = (float)ADCA.CH2.RES;													//Read +1.8_VDD
	VOLT[3] = (float)ADCA.CH3.RES;													//Read +3.0VIO	
	
	VOLT[4] = (float)ADCB.CH0.RES;													//Read VPP_2V5
	VOLT[5] = (float)ADCB.CH1.RES;													//Read +1.8VIO
	VOLT[6] = (float)ADCB.CH2.RES;													//Read BLE_VDD

	for(uint8_t i = 0; i < 8; i++)
	{
		VOLT[i] = ((VOLT[i]*ADC_VREF*ADC_LIN*4)/4095)-ADC_OFFSET;
	}
	
	ADCA.INTFLAGS |= (0x0F);														//clear interrupt flags ADCA
	ADCB.INTFLAGS |= (0x0F);														//clear interrupt flags ADCB
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV8_gc;											//Get clock ADC ready for digital measurements
	ADCB.PRESCALER = ADC_PRESCALER_DIV8_gc;											//Get clock ADC ready for digital measurements
	
	ADCA.CTRLA = ADC_FLUSH_bm;														//Flushes ADCA
	ADCB.CTRLA = ADC_FLUSH_bm;														//Flushes ADCB
}

void fread_DO(void)
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
	
//	for(uint32_t i = 0; i < SAMPLE_WINDOW; i++)
//	{
//		write8_UART((uint8_t)D0_raw[i]);											//debug print low byte
//		write8_UART((uint8_t)(D0_raw[i]>>8));										//debug print high byte
//	}
	
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
		}

		avrageT = (float)sum/8;														//calculates average time in samples
		sendarray = (uint8_t*)(&avrageT);											//converts float to 4 seperate bytes in send array

		write8_UART(sendarray[0]);
		write8_UART(sendarray[1]);
		write8_UART(sendarray[2]);
		write8_UART(sendarray[3]);
	}
	
	else
	{
		writeF_UART(0.99999);														//Send error message 
	}	
}

void read_IT(void)
{
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;										//2MHz system clock use: ADCB clock speed to 62.5kHz
	
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;										//Set ADCA CH0 to Pin A4 as input for I_external
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;										//Set ADCA CH1 to Pin A5 as input for I_battery

	ADCA.CTRLA |= ADC_FLUSH_bm;														//Enables ADCA
	ADCB.CTRLA |= ADC_FLUSH_bm;														//Enables ADCB
	
	ADCA.CTRLA |= ADC_ENABLE_bm;													//Enables ADCA
	ADCB.CTRLA |= ADC_ENABLE_bm;													//Enables ADCB
	
	ADCA.CTRLA |= (0x03 << 2);														//Start ADCA Conversion first two channels
	ADCB.CTRLA |= (0x01 << 2);														//Start ADCB conversion CH0
	
	while( !((ADCA.INTFLAGS & 0x3) & (ADCB.INTFLAGS & 0x01)))                       //Wait for ADC conversion
	
	TIMEOUT();
	
	IT[0] = (float)ADCA.CH0.RES;													//Read I_external
	IT[1] = (float)ADCA.CH1.RES;													//Read I_battery
	
	IT[2] = (float)ADCB.CH0.RES;													//Read Temp_ambient
	
	ADCA.INTFLAGS |= (0x03);														//clear interrupt flags ADCA
	ADCB.INTFLAGS |= (0x01);														//clear interrupt flags ADCB
}


void read_supply_ext(void)
{
	float current[10];
	float average_I;
	float sum = 0;																	//Big enough sum of 10 12bit uints;
	
	PORTB_OUTSET = PIN2_bm;															//supply ext 
	TIMEOUT();
	PORTB_OUTCLR = PIN3_bm;															//disable bat 
	TIMEOUT();
	
	for(int i = 0; i < 10; i++)														//Get 10 samples over 1 second
	{
		read_IT();
		current[i] = IT[0];
		TIMEOUT();
	}
	
	for(int i = 0; i < 10; i++)
	{
		sum += current[i];															//Calculate sum of ADC res
	}
	
	average_I = sum/10;																//Calculate average ADC res
	average_I = (average_I*ADC_VREF*ADC_LIN)-ADC_OFFSET;							//Calculate ADC voltage
	average_I = (average_I/(165))*1000000;											//Convert voltage to current in uA
	
	writeF_UART(average_I);															//Return current in uA
}


void write_volt(void)
{
	read_voltages();
	
	for(int i = 0; i < 7; i ++)
	{
		writeF_UART(VOLT[i]);														//Write voltages to MATLAB app
	}
}

void read_supply_bat(void)
{
	float current[10];
	float average_I;
	float sum = 0;																	//Big enough sum of 10 12bit uints;
	
	PORTB_OUTSET = PIN3_bm;															//enable bat
	TIMEOUT();
	PORTB_OUTCLR = PIN2_bm;															//disable ext 
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
	
	average_I = sum/10;																//Calculate average ADC res
	average_I = (average_I*ADC_VREF*ADC_LIN)-ADC_OFFSET;							//Calculate ADC voltage
	average_I = (average_I/(500))*1000000;										//Convert voltage to current in uA
	
	writeF_UART(average_I);															//Return current in uA
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


void LPM_P_OFF(void)
{
	PORTB_OUTCLR = PIN2_bm;															//turn external supply off
	PORTB_OUTCLR = PIN3_bm;															//turn battery supply off 
}

void writeF_UART(float data)
{
	uint8_t *sendarray;
	sendarray = (uint8_t*)(&data);													//converts float to 4 seperate bytes in send array
	write8_UART(sendarray[0]);
	write8_UART(sendarray[1]);
	write8_UART(sendarray[2]);
	write8_UART(sendarray[3]);
}


void supply_ext(void)
{
	PORTB_OUTSET = PIN2_bm;															//supply ext
	TIMEOUT();
	PORTB_OUTCLR = PIN3_bm;															//disable bat
	TIMEOUT();
}

void supply_bat(void)
{
	PORTB_OUTSET = PIN3_bm;															//disable bat
	TIMEOUT();
	PORTB_OUTCLR = PIN2_bm;															//supply ext
	TIMEOUT();
}
