/*
 * CAN_RX_Servo.c
 *
 * Created: 7/14/2021 4:19:16 AM
 * Author : Ahmed_Ayman
 */ 

#define  F_CPU 8000000
#include <avr/io.h>
#include "HAL_SPI.h"
#include "MCP2515.h"
#include <util/delay.h>
#include "Lcd-Driver.h"
#include "stdbool.h"
#include "8BIT_TIM.h"

CANMesg_t handler_messageTX ,handler_messageRX;
bool DataIsReady  ;

int Res  ,ServoStop  ;
float Speed ;
PWMInit_typedef Timer0_Init ;       /* global variable for initializing timer0 */

int main(void)
{

	
	
			MCP2515_init();   /* initialize the MCP2515 chip */
			LCD_Initializaion(); /* initialize LCD */
	
			handler_messageTX.ID = 0x124 ;
			handler_messageTX.CANControl.LEN_Field = 8 ;

		Timer0_Init.Instance = TIM0 ;
		Timer0_Init.TIM_Intrupt_State = TIM0_INIT_DISABLE ;
		Timer0_Init.TIMMode = TIM_MODE_PWM_PHASE ;
		Timer0_Init.TimPreScaler = TIM0_Prescaler_1024 ;
		Timer0_Init.COMPAction  = TIM_COMP_OUT_PWM_PHASE_OC_SET;					
		DDRD= 0xe0 ;
		
    /* Replace with your application code */
    while (1) 
    {			

									PORTD = 0x80;
									TIM_CTCMode_DelayUs(TIM0, handler_messageRX.MesgData[0]/2.8 * 500+1);
									PORTD = 0x0;
									_delay_ms(20);
	}

}

ISR(INT0_vect)
{

	MCP2515_receiveMesg(&handler_messageRX); /* receive message with ID 0x124 */

}