/*
 * Debug_test.c
 *
 * Created: 20/04/2022 09:15:21
 * Author : david
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "start.h"
#include "handle.h"

#ifndef F_CPU
#define F_CPU 32000000
#endif

int PTimeOut = 0;
int TC_samples = 0;
uint16_t PTime[6];
float VOLT[7];
int16_t IT[3];
char INPUTBUF[3];
int InpVar;
char OUTPUTBUF[5];
int16_t OutVar;
uint16_t D0_raw[SAMPLE_WINDOW];

ISR(TCE0_OVF_vect)														//Do something every timer E0 overflow
{
	PTimeOut = 1;														//If TC overflow occurs before collecting pulses stop while loop
}

ISR(PORTD_INT0_vect)													//Do something when PD2 encounters falling edge
{
	PTime[TC_samples] = (((TCE0.CNTH+1)<<8)+TCE0.CNTL+1);
	TC_samples++;
}

ISR(PORTD_INT1_vect)													//Do something when PD1 encounters falling edge
{
	//Do something
}

int main(void)
{
	init_CLK();
	init_TIMER();
	init_DELAY();
	init_ADC();
	init_UART();
	init_supply();
	sei();
	PMIC.CTRL |= PMIC_LOLVLEN_bm;										//Set low level interrupts
	
	PORTA_DIRSET = PIN7_bm;												//Set I'm alive pin as output
	PORTA_OUTSET = PIN7_bm;												//Enable I'm alive LED
	
	PORTE_DIRSET = PIN4_bm;												//Timing debug pin

	enum commands
	{
		SupBat,
		SupExt,
		SupOff,
		IBat,
		IExt,
		RVolt,
		RFrequency
	};
   
   
   uint8_t uart_data;

    while (1) 
    {
		uart_data = read_UART();
		switch(uart_data)
		{
			case(SupBat):
				supply_bat();											//Enables battery power supply and disables ext
				break;
				
			case(SupExt):
				supply_ext();											//Enables ext power supply and disables battery
				break;

			case(SupOff):
				LPM_P_OFF();											//turn of both external and bat supply to LPM
				break;
				
			case(IBat):
				read_supply_bat();										//measures Ibat in uA	
				break;
				
			case(IExt):
				read_supply_ext();										//measures IExt in uA
				break;
				
			case(RVolt):
				write_volt();											//Reads voltages and converts them to two uint8_t data packages (14 packages total)
				break;
				
			case(RFrequency):
				fread_DO();												//read the frequency of Digital outputs
				break;
				
			default:
				break;	
		}
	}
		
}

