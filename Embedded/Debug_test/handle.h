/*
 * handle.h
 *
 * Created: 18/05/2022 14:50:13
 *  Author: david
 */ 
#include <stdio.h>
#include <string.h>

#ifndef HANDLE_H_
#define HANDLE_H_

#define DISABLE_IR_DO
#define dis_dac
#define SAMPLE_WINDOW 5000

void start_timer();
void stop_timer();

void read_voltages();
void read_IT();

void start_DELAY();
void stop_DELAY();
void TIMEOUT();
uint8_t read_UART();
void write_UART(uint16_t data);
void write8_UART(uint8_t data);
void read_supply_ext(void);
void read_supply_bat(void);
void write_volt(void);
void fread_DO(void);
void LPM_P_OFF(void);
void writeF_UART(float data);
void supply_bat(void);
void supply_ext(void);


#endif /* HANDLE_H_ */