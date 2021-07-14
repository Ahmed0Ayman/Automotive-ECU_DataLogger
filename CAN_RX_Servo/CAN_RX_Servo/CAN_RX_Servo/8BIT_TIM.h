
/*
 * _8BIT_TIM.h
 *
 * Created: 5/13/2021 10:10:19 PM
 *  Author: Ahmed_Ayman
 */ 
#ifndef __8BIT_TIM__
#define __8BIT_TIM__





#include <avr/io.h>
#include <stdbool.h>



#define  F_CPU 16000000 
 

#define  TIM0_IN_USE     /* if you wish to use timer2 just make it as #undef TIM0_IN_USE you don't need to change any thing else */


#define  TIM0                  0x00u
#define  TIM2                  0x01u


#define	TIM0_Prescaler_0                 0x01u
#define TIM0_Prescaler_8                 0x02u
#define TIM0_Prescaler_64                0x03u
#define TIM0_Prescaler_256               0x04u
#define TIM0_Prescaler_1024              0x05u



#define TIM2_Prescaler_0                 0x01u
#define TIM2_Prescaler_8                 0x02u
#define TIM2_Prescaler_32                0x03u
#define TIM2_Prescaler_64                0x04u
#define TIM2_Prescaler_128               0x05u
#define TIM2_Prescaler_256               0x06u
#define TIM2_Prescaler_1024              0x07u





#define TIM0_PWM_START()					DDRB=(1<<3)
#define TIM0_PWM_STOP()						DDRB &= ~(1<<3)

#define TIM2_PWM_START()					DDRD=|(1<<7)     /* disable time at GPIO level */
#define TIM2_PWM_STOP()						DDRD &=~(1<<7)  


/* define operation modes in timer0 | timer1 */
#define  TIM_MODE_NORMAL					0x00u 
#define  TIM_MODE_PWM_PHASE					0x40u
#define  TIM_MODE_CTC						0x08u
#define  TIM_MODE_PWM_FAST_MODE				0x48u


#define  TIM_COMP_OUT_NormalGPIO					0x00u
#define  TIM_COMP_OUT_FAST_PWM_OC_CLR				0x20u
#define  TIM_COMP_OUT_FAST_PWM_OC_SET				0x30u

#define  TIM_COMP_OUT_PWM_PHASE_OC_CLR				0x20u
#define  TIM_COMP_OUT_PWM_PHASE_OC_SET				0x30u

#define  TIM_COMP_OUT_NormalMode_OC_TOG			0x10u
#define  TIM_COMP_OUT_NormalMode_OC_CLR			0x20u
#define  TIM_COMP_OUT_NormalMode_OC_SET			0x30u


#define  TIM0_INIT_DISABLE           0x00u
#define  TIM0_INIT_COMP_EN           0x02u
#define  TIM0_INIT_OVER_EN           0x01u
#define  TIM2_INIT_COMP_EN           0x07u
#define  TIM2_INIT_OVER_EN           0x06u


typedef void (*CaLL_BackFun)(void);  /* typedef for function pointer */ 

typedef struct
{
	
	uint8_t TIMMode   ;			 /* must be one of TIM_MODE_    */
	uint8_t COMPAction ;		/* must be one of TIM_COMP_OUT_*/
	uint8_t TimPreScaler ;     /* must be one of  TIM_Prescaler_ */
	uint8_t Instance ;        /* peripheral select TIM0 or TIM1 */
	uint8_t TIM_Intrupt_State ;   /* one of TIM0_INIT_ to enable specific interrupt  */
	
	
}PWMInit_typedef;





/* 
 * brief : this function used to initialize Timer To work on the predetermined mode in InitMode struct 
 * parameter : Instance TIM0 or TIM1
 * parameter : InitMode pointer to struct hold all timer configurations 
 * return : void 
 */
void TIM_PWMMode_Init(PWMInit_typedef * InitMode);



/* 
 * brief : this function used to set Timer duty
 * parameter : Instance TIM0 or TIM1
 * parameter : Duty duty from 0-->100
 * return : void 
 */
void TIM_PWMMode_SetDuty(uint8_t Instance,uint8_t Duty);



/* timer0 normal mode APIs */


/* 
 * brief : this function used to start timer0 operation 
 * parameter : Instance TIM0 or TIM1
 * parameter : Prescaler used to divide the derived clock to timer one of TIM_Prescaler_ 
 * return : void 
 */
void TIM_NormalMode_Start(uint8_t Instance,uint8_t  Prescaler );


/* 
 * brief : this function used to stop timer0 operation by clearing timer clock 
 * parameter : Instance TIM0 or TIM1 
 * return : void 
 */
void TIM_NormalMode_Stop(uint8_t Instance);



/* 
 * brief : this function used to set Timer value
 * parameter : Instance TIM0 or TIM1
 * parameter : ContVal used as start timer count value 
 * return : void 
 */
void TIM_NormalMode_Set(uint8_t Instance,uint8_t ContVal);



/* 
 * brief : this function used to set Timer duty
 * parameter : Instance TIM0 or TIM1
 * return : current timer value 
 */
uint8_t TIM_NormalMode_Get(uint8_t Instance);






/* 
 * brief : this function used to generate delay in Ms blocking mode 
 * parameter : Instance TIM0 or TIM1
 * parameter : MsDelay delay in Ms
 * return : current timer value 
 */
void TIM_CTCMode_DelayMs(uint8_t Instance,uint16_t MsDelay);




/* 
 * brief : this function used to generate delay in Ms blocking mode 
 * parameter : Instance TIM0 or TIM1 
 * parameter : UsDelay Delay in Us 
 * return : current timer value 
*/
void TIM_CTCMode_DelayUs(uint8_t Instance, uint16_t UsDelay);


/* all call back function must be registered by user before enable specific TIMER */

void TIM0_CallBack_OverFlow_Fuction(CaLL_BackFun  callbackfumction);

void TIM0_CallBack_CompMatch_Fuction(CaLL_BackFun  callbackfumction);

void TIM2_CallBack_OverFlow_Fuction(CaLL_BackFun  callbackfumction);

void TIM2_CallBack_CompMatch_Fuction(CaLL_BackFun  callbackfumction);







#endif