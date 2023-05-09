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

#ifndef DISABLE_IR_DO
	void enable_D0();
	void enable_D1();
	void disable_D0();
	void disable_D1();
	uint16_t get_D0();
	uint16_t get_D1();
#endif

void read_voltages();
void read_IT();
double calc_DACDATA(double dac_output);

#ifndef dis_dac
void supply_bat();
void supply_ext();
#endif

void start_DELAY();
void stop_DELAY();
void TIMEOUT();
uint8_t read_UART();
void write_UART(uint16_t data);
void write8_UART(uint8_t data);
uint16_t read_supply_ext(void);
uint16_t read_supply_bat(void);
void write_ext(void);
void write_bat(void);
void write_volt(void);
void fread_DO(void);
void LPM_P_OFF(void);
void writeF_UART(float data);



#endif /* HANDLE_H_ */