#ifndef _HX711_H
#define _HX711_H
#include "stm32f0xx.h"
//²Î¿¼ https://blog.csdn.net/qq_21078557/article/details/80496495
void Init_Hx711(void);
void Get_Maopi(void);
unsigned long HX711_Read(void);
long Get_Weight(void);
void delay_us(uint16_t time);
void delay_ms(uint16_t time);

#endif
