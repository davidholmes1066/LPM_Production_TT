/*
 * init.c
 *
 * Created: 18/05/2022 12:26:41
 *  Author: david
 */ 

#include <stdio.h>
#include <stdio.h>
#include "start.h"

void init_UART(void)
{
	PORTE_DIRCLR = PIN2_bm;												//Set RX pin USARTE0 E2 as input
	PORTE_DIRSET = PIN3_bm;												//Set TX pin USARTE0 E3 as output
	PORTE_OUTSET = PIN3_bm;												//Set TX pin to high
	
	USARTE0.BAUDCTRLB |= (0x09<<4);										//Set BSCALE to -7 for baud 115200
	USARTE0.BAUDCTRLA |= (0x2E);										//Set BSEL to 2094 for baud 115200
	USARTE0.BAUDCTRLB |= (0x08);										//Set BSel to 2094 for baud 115200
	
	USARTE0.CTRLC |= (0x03);											//Sets amount of data bits to 8
	USARTE0.CTRLB |= (0x03<<3);											//Sets the Rx and TX enable bits.
}

void init_CLK(void)
{
	OSC.CTRL |= OSC_RC32MEN_bm;											// Enable internal 32 MHz oscillator
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));								// Wait for oscillator is ready
	CCP = CCP_IOREG_gc;													// Security signature to modify clock
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;									// Select 32 MHz oscillator
	CLK.PSCTRL = CLK_PSADIV_1_gc;										// Set PSA to 1
	CLK.PSCTRL = CLK_PSBCDIV_1_1_gc;									// Set PSB&PSC to 1
}

void init_TIMER(void)
{
	TCE0.CTRLB = TC_WGMODE_NORMAL_gc;									//Set timer E0 to normal mode
	TCE0.CTRLA = TC_CLKSEL_OFF_gc;										//Timer is off until clksel is changed
	TCE0.INTCTRLA = TC_OVFINTLVL_LO_gc;									//Set overflow interrupt TCE0 to low level
	TCE0.PERL = 0xFF;
	TCE0.PERH = 0xFF;													//t = N*(PER + 1)/F_CPU = 0.25
}

void init_DELAY(void)													//Uses two 16Bit TC's to form 32 Bit one
{
	TCD1.CTRLB = TC_WGMODE_NORMAL_gc;									//Set timer E0 to normal mode
	TCD1.CTRLA = TC_CLKSEL_OFF_gc;										//Timer is off until clksel is changed
	TCD1.PER = 0xFFFF;													//t = N*(PER + 1)/F_CPU = 0.25
}

void init_ADC(void)
{
	PORTA_DIRCLR = PIN0_bm;												//VCC_main
	PORTA_DIRCLR = PIN1_bm;												//CORE_VDD
	PORTA_DIRCLR = PIN2_bm;												//+1.8VDD
	PORTA_DIRCLR = PIN3_bm;												//+3.0VIO
	PORTA_DIRCLR = PIN4_bm;												//I_external
	PORTA_DIRCLR = PIN5_bm;												//I_battery
	
	PORTB_DIRCLR = PIN0_bm;												//VPP_2V5
	PORTB_DIRCLR = PIN1_bm;												//+1.8VIO
	PORTB_DIRCLR = PIN4_bm;												//BLE_VDD
	PORTB_DIRCLR = PIN5_bm;												//D0 samples
	PORTB_DIRCLR = PIN6_bm;												//D1 samples
	
	//ADCA.PRESCALER |= ADC_PRESCALER_DIV8_gc;							//Set ADCA clock speed to 62.5kHz
	//ADCB.PRESCALER |= ADC_PRESCALER_DIV8_gc;							//Set ADCB clock speed to 62.5kHz
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;							//2MHz system clock use: ADCA clock speed to 62.5kHz
	ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;							//2MHz system clock use: ADCB clock speed to 62.5kHz
	
	ADCA.REFCTRL |= ADC_RESOLUTION_12BIT_gc;							//Set ADCA to 12bit resolution
	ADCB.REFCTRL |= ADC_RESOLUTION_12BIT_gc;							//Set ADCB to 12bit resolution
	
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;								//Set ADCA reference voltage to internal VCC
	ADCB.REFCTRL |= ADC_REFSEL_INTVCC_gc;								//Set ADCB reference voltage to internal VCC
	
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCA channel 0 differential input
	ADCA.CH1.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCA channel 1 differential input
	ADCA.CH2.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCA channel 2 differential input
	ADCA.CH3.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCA channel 3 differential input
	
	ADCB.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCB channel 0 differential input
	ADCB.CH1.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCB channel 1 differential input
	ADCB.CH2.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCB channel 2 differential input
	ADCB.CH3.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;					//Set ADCB channel 3 differential input
	
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCA channel 0 reference voltage to internal GND
	ADCA.CH1.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCA channel 1 reference voltage to internal GND
	ADCA.CH2.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCA channel 2 reference voltage to internal GND
	ADCA.CH3.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCA channel 3 reference voltage to internal GND
	
	ADCB.CH0.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCB channel 0 reference voltage to internal GND
	ADCB.CH1.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCB channel 1 reference voltage to internal GND
	ADCB.CH2.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCB channel 2 reference voltage to internal GND
	ADCB.CH3.MUXCTRL |= ADC_CH_MUXNEG_INTGND_MODE3_gc;					//Set ADCB channel 3 reference voltage to internal GND
}

#ifndef dis_dac
void init_DAC(void)
{
	DACB.CTRLC = DAC_REFSEL_INT1V_gc;									//set DAC reference to internal 1V
	DACB.CTRLB = DAC_CHSEL_DUAL_gc;										//set DAC to DUAL channel mode
	DACB.CTRLA = DAC_CH1EN_bm | DAC_CH0EN_bm | DAC_ENABLE_bm;			//Enable both DAC channels
	// PINS: PB2 = DACCH0 & PB3 = DACCH1
	// Write: to register DACB.CH0DATA and DACB.CH1DATA
}
#endif

void init_supply(void)
{
	PORTB_DIRSET = PIN2_bm;												//Set pin as output to enable ... supply
	PORTB_DIRSET = PIN3_bm;												//Set pin as output to enable ... supply
}