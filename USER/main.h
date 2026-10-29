/*-----------------------------------------------------*/
/*              超纬电子STM32系列开发板                */
/*-----------------------------------------------------*/
/*                     程序结构                        */
/*-----------------------------------------------------*/
/*USER     ：包含程序的main函数，是整个程序的入口      */
/*HARDWARE ：包含开发板各种功能外设的驱动程序          */
/*CORE     ：包含STM32的核心程序，官方提供，我们不修改 */
/*STLIB    ：官方提供的库文件，我们不修改              */
/*-----------------------------------------------------*/
/*                                                     */
/*           程序main函数，入口函数头文件              */
/*                                                     */
/*-----------------------------------------------------*/

#ifndef __MAIN_H
#define __MAIN_H
#include "sys.h"
extern u8 work_mode;
extern u8 show_mode;        //对应不同的界面和模式
extern u8 work_status;      //=0为电池充电状态，=1为电池放电状态

extern u8 protect_status;   //继电器异常保护状态
extern u16 dc_pwm;          //DC电路的PWM
  
extern float DCIN_V;     //输入电压数据
extern float DCIN_I;     //输入电流数据
extern float DCIN_P;     //输入功率
  
extern float DCOUT_V;    //输出电压数据
extern float DCOUT_I;    //输出电流数据
extern float DCOUT_P;    //输出功率
  
extern float Target_I;  //追踪的充电电流数据
extern float Target_V; //追踪的放电电压数据
  
extern float V_yuzhi;  //保护的电压阈值
extern float I_yuzhi;   //保护的电流阈值
#define VDCIN_Ratio 20.10
#define IDCIN_Ratio 2.0000
#define VDCOUT_Ratio 20.17
#define IDCOUT_Ratio 1.9600
#endif











