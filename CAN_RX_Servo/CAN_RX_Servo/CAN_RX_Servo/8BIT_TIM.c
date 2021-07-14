
/*
 * _8BIT_TIM.c
 *
 * Created: 5/13/2021 10:10:09 PM
 *  Author: Ahmed_Ayman
 */ 
#include "8BIT_TIM.h"

/* vectors for timer0 & timer1 as described in data sheet */

void __vector_10 (void) __attribute__ ((signal,used));
void __vector_11 (void) __attribute__ ((signal,used));

void __vector_4 (void)  __attribute__ ((signal,used));
void __vector_5 (void)  __attribute__ ((signal,used));


/* for timer0 */
static CaLL_BackFun COMPMatch0_Fun ;
static CaLL_BackFun OverFlow0_Fun ;

/* for timer2 */
static CaLL_BackFun COMPMatch2_Fun ;
static CaLL_BackFun OverFlow2_Fun ;

/* 
 * brief : this function used to initialize Timer To work on the predetermined mode in InitMode struct 
 * parameter : InitMode pointer to struct hold all timer configurations 
 * return : void 
 */
void TIM_PWMMode_Init(PWMInit_typedef * InitMode)
{
	if(InitMode->Instance == TIM0)
{
	TCCR0 = InitMode->COMPAction | InitMode->TIMMode | InitMode->TimPreScaler ;
	TIM0_PWM_STOP() ;  /* disable at GPIO level till enabled by user */
}
	else
{
	TCCR2 = InitMode->COMPAction | InitMode->TIMMode | InitMode->TimPreScaler ;	
	TIM2_PWM_STOP() ;  /* disable at GPIO level till enabled by user */
}
	TIMSK |=(1<<InitMode->TIM_Intrupt_State);
	
	
}/* END_FUN  TIM_PWMMode_Init() */



/* 
 * brief : this function used to set Timer duty
 * parameter : Duty duty from 0-->100
 * return : void 
 */
void TIM_PWMMode_SetDuty(uint8_t Instance,uint8_t Duty)
{
	if(Instance == TIM0)
	OCR0 = (255*Duty)/100;
	else
	OCR2 = (255*Duty)/100;
	
}/* END_FUN  TIM_PWMMode_SetDuty() */





/* 
 * brief : this function used to start timer0 operation 
 * parameter : Prescaler used to divide the derived clock to timer 
 * return : void 
 */
void TIM_NormalMode_Start(uint8_t Instance,uint8_t  Prescaler )
{
	if(Instance == TIM0)	
	TCCR0 = Prescaler; /* set clock source seclect */
	else
	TCCR2 = Prescaler; /* set clock source seclect */

}/* END_FUN  TIM_NormalMode_Start() */




/* 
 * brief : this function used to stop timer0 operation by clearing timer clock 
 * return : void 
*/
void TIM_NormalMode_Stop(uint8_t Instance)
{
	if(Instance == TIM0)		
	TCCR0 &= 0xf1; /* clear clock source seclect */
	else
	TCCR2 &= 0xf1; /* clear clock source seclect */
	
}/* END_FUN  TIM_NormalMode_Stop() */




/* 
 * brief : this function used to set Timer value
 * parameter : ContVal used as start timer count value 
 * return : void 
 */
void TIM_NormalMode_Set(uint8_t Instance,uint8_t ContVal)
{
	if(Instance == TIM0)			
	TCNT0 = ContVal ;
	else
	TCNT2 = ContVal ;
		
	
}/* END_FUN  TIM_NormalMode_Set() */



/* 
 * brief : this function used to set Timer duty
 * parameter : void
 * return : current timer value 
 */
uint8_t TIM_NormalMode_Get(uint8_t Instance )
{
	if(Instance == TIM0)				
	return TCNT0;
	else
	return TCNT2;
	
}/* END_FUN  TIM_NormalMode_Get() */




/* 
 * brief : this function used to generate delay in Ms blocking mode 
 * parameter : MsDelay delay in Ms
 * return : current timer value 
*/
void TIM_CTCMode_DelayMs(uint8_t Instance,uint16_t MsDelay)
{
	if(Instance == TIM0)				
	{
	TCCR0 = TIM0_Prescaler_64 ;
	OCR0 =(uint8_t)(F_CPU/((uint32_t)64*1000));	/* SET ocr regiset to required value */

	for(uint16_t ietrator=0;ietrator<MsDelay;ietrator++)
	{
		
		while(!(TIFR &(1<<OCF0)));
		TIFR |= 1<<OCF0;
	}
	}
	else
	{
	TCCR2 = TIM2_Prescaler_64 ;
	OCR2 =(uint8_t)(F_CPU/((uint32_t)64*1000));	/* SET ocr regiset to required value */

	for(uint16_t ietrator=0;ietrator<MsDelay;ietrator++)
	{
		
		while(!(TIFR &(1<<OCF2)));
		TIFR |= 1<<OCF2;
	}		
	}
}/* END_FUN  TIM_CTCMode_DelayMs() */




/* 
 * brief : this function used to generate delay in Ms blocking mode 
 * parameter : UsDelay Delay in Us 
 * return : current timer value 
*/
void TIM_CTCMode_DelayUs(uint8_t Instance, uint16_t UsDelay)
{
	if(Instance == TIM0)				
	{	
	TCCR0 = TIM0_Prescaler_8 ;
	OCR0 = (uint8_t)((F_CPU)/(8000000));	/* SET ocr register to required value */
	for(uint16_t ietrator=2;ietrator<UsDelay;ietrator++)
	{
		TCNT0 =0;
		while(!(TIFR &(1<<OCF0)));
		TIFR |= 1<<OCF0;
	}
	}
	else
	{
	TCCR2 = TIM2_Prescaler_8 ;
	OCR2 = (uint8_t)((F_CPU)/(8000000));	/* SET ocr register to required value */
	for(uint16_t ietrator=2;ietrator<UsDelay;ietrator++)
	{
		TCNT2 =0;
		while(!(TIFR &(1<<OCF2)));
		TIFR |= 1<<OCF2 ;
	}		
	}
		
	
}/* END_FUN  TIM_CTCMode_DelayUs() */


void TIM0_CallBack_OverFlow_Fuction(CaLL_BackFun  callbackfumction)
{
	
    OverFlow0_Fun = callbackfumction ;	
	
}


void TIM0_CallBack_CompMatch_Fuction(CaLL_BackFun  callbackfumction)
{
	
	
	COMPMatch0_Fun = callbackfumction ;
	
}
void TIM2_CallBack_OverFlow_Fuction(CaLL_BackFun  callbackfumction)
{
	
	OverFlow2_Fun = callbackfumction ;
	
}


void TIM2_CallBack_CompMatch_Fuction(CaLL_BackFun  callbackfumction)
{
	
	
	COMPMatch2_Fun = callbackfumction ;
	
}





/* all vectors specific for timer2 and timer0 */
void __vector_10(void)
{
	COMPMatch0_Fun();
}
void __vector_11(void)
{
	OverFlow0_Fun();
}


void __vector_4(void)
{
	COMPMatch2_Fun();
}
void __vector_5(void)
{
	OverFlow2_Fun();
}