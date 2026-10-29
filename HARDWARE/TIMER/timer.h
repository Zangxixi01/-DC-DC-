#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void BUCK_PWM_Init(void);//定时器设置
void BOOST_PWM_Init(void);//定时器设置
void set_pwm(TIM_TypeDef* TIMx,u8 chx,u16 prec,u16 up);
#endif

