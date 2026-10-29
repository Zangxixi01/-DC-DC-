#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define SET          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键SW
#define SET_PRES 	   1	 
#define CHOICE       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键SW
#define CHOICE_PRES  2	 
#define ADD          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键SW
#define ADD_PRES 	   3	 
#define REDUCE       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键SW
#define REDUCE_PRES  4	 
void KEY_Init(void);      //IO初始化
u8 KEY_Scan(u8);  	      //按键扫描函数					    
#endif
