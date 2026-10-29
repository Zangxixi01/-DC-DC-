#include "timer.h"
#include "adc.h"
#include "main.h"
#include "sys.h"
#include "pid.h"
#include "encoder.h"
/*buck同步整流电路配置*/
#define BUCK_TIMx TIM1
#define BUCK_Plus 0  
#define BUCK_ARR 3599//重装载值3600
#define BUCK_PSC 0//分频系数1
void BUCK_PWM_Init(void)//buck电路输出，拟定充电
{   
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE); //使能PORTA,B时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	//初始化GPIO,PA8	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//端口复用
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); //PA8
	
	//初始化时具单元
	TIM_DeInit(BUCK_TIMx);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=BUCK_ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler=BUCK_PSC;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(BUCK_TIMx,&TIM_TimeBaseInitStruct);	
	//将输出通道2初始化为PWM模式1
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_Pulse=BUCK_Plus;
	TIM_OC1Init(BUCK_TIMx,&TIM_OCInitStruct);
	//使能预装载寄存器
	TIM_OC1PreloadConfig(BUCK_TIMx,TIM_OCPreload_Enable);
	//死区和刹车功能配置
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Disable;
	TIM_BDTRInitStruct.TIM_OSSRState=TIM_OSSRState_Disable;
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_1;
	TIM_BDTRInitStruct.TIM_DeadTime=3; //40.92ns
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_Low;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(BUCK_TIMx,&TIM_BDTRInitStruct);
  //使能定时器中断
	TIM_ITConfig(BUCK_TIMx,TIM_IT_Update,ENABLE ); //允许更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	//使能自动重装载
	TIM_ARRPreloadConfig(BUCK_TIMx,ENABLE);
	//开启定时器
	TIM_Cmd(BUCK_TIMx,ENABLE);
	//主输出使能
	TIM_CtrlPWMOutputs(BUCK_TIMx,ENABLE);
}

void BOOST_PWM_Init(void)//boost电路输出，拟定放电
{   
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE); //使能PORTA,B时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	//初始化GPIO,PA8	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//端口复用
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); //PA8
	
	//初始化时具单元
	TIM_DeInit(BUCK_TIMx);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=BUCK_ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler=BUCK_PSC;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(BUCK_TIMx,&TIM_TimeBaseInitStruct);	
	//将输出通道2初始化为PWM模式1
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_Pulse=BUCK_Plus;
	TIM_OC1Init(BUCK_TIMx,&TIM_OCInitStruct);
	//使能预装载寄存器
	TIM_OC1PreloadConfig(BUCK_TIMx,TIM_OCPreload_Enable);
	//死区和刹车功能配置
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Disable;
	TIM_BDTRInitStruct.TIM_OSSRState=TIM_OSSRState_Disable;
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_1;
	TIM_BDTRInitStruct.TIM_DeadTime=3; //40.92ns
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_Low;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(BUCK_TIMx,&TIM_BDTRInitStruct);
  //使能定时器中断
	TIM_ITConfig(BUCK_TIMx,TIM_IT_Update,ENABLE ); //允许更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	//使能自动重装载
	TIM_ARRPreloadConfig(BUCK_TIMx,ENABLE);
	//开启定时器
	TIM_Cmd(BUCK_TIMx,ENABLE);
	//主输出使能
	TIM_CtrlPWMOutputs(BUCK_TIMx,ENABLE);
}

//中断服务函数
void TIM1_UP_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查更新中断发生与否
	{

		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}
/*选择通道函数*/
void set_pwm(TIM_TypeDef* TIMx,u8 chx,u16 prec,u16 up)
{
	//判断输入参数是否正确
	if(chx<1||chx>4)
		return;
	if(prec>up)
		prec=up;
	//根据输入的通道设置PWM占空比
	switch(chx)
	{
		case 1:TIM_SetCompare1(TIMx,prec);break;
		case 2:TIM_SetCompare2(TIMx,prec);break;     
		case 3:TIM_SetCompare3(TIMx,prec);break;
		case 4:TIM_SetCompare4(TIMx,prec);break;           
	}
}
















