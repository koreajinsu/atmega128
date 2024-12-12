/*
 * led.h
 *
 * Created: 2024-11-08 오후 12:20:36
 *  Author: user
 */ 
//정의
//함수 원형

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define LED_DDR		DDRD
#define LED_PORT	PORTB


void GPIO_LEDInit();
void GPIO_output(uint8_t data);
void ledleftshift(uint8_t *data);
void ledrightshift(uint8_t *data);

#endif /* LED_H_ */