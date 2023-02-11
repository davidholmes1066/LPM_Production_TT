/*
 * init.h
 *
 * Created: 18/05/2022 12:29:08
 *  Author: david
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#ifndef INIT_H_
#define INIT_H_

#define dis_dac

void init_CLK(void);
void init_TIMER(void);
void init_ADC();

#ifndef dis_dac
void init_DAC(void);
#endif

void init_DELAY(void);
void init_UART(void);
void init_supply(void);

#endif /* INIT_H_ */