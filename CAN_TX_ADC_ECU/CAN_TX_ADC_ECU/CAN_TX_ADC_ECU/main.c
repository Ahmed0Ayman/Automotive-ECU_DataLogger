/*
 * CAN_TX_ADC_ECU.c
 *
 * Created: 7/13/2021 10:58:13 PM
 * Author : Ahmed_Ayman
 */ 

#define  F_CPU 16000000
#include <avr/io.h>
#include "HAL_SPI.h"
#include "MCP2515.h"
#include <util/delay.h>
#include "Lcd-Driver.h"
#include "ADC.h"
#include "stdbool.h"

bool DataIsRead ;
CANMesg_t handler_messageTX ,handler_messageRX;
ADC_Handler_t hadc =	{.ADC_TRIG_SOURCE = ADC_TRIG_FREE_RUNNING ,.ADC_Adjust_Select = ADC_Adjust_Right ,	\
						 .ADC_Intrrupet_Select = ADC_INIT_DISABLE , .ADC_PreScaler = ADC_Presaler_2 ,		\
						 .ADC_Vref_Select = ADC_Vref_AVCC } ;


	int Res ,Speed  ;

int main(void)
{
		ADC_INIT(&hadc);
		MCP2515_init();   /* initialize the MCP2515 chip */
		LCD_Initializaion(); /* initialize LCD */
		DDRD &=~(0x01);
		handler_messageTX.ID = 0x123 ;
		handler_messageTX.CANControl.LEN_Field = 8 ;		
		
    /* Replace with your application code */
    while (1) 
    {
		
						ADC_GET_VALUE(&hadc,ADC_CH_0);
						Res =  (uint8_t)hadc.ADC_Value ;
						handler_messageTX.MesgData[0]=Res ;
						MCP2515_SendCANmsg(&handler_messageTX,0);
						_delay_ms(100);

						LCD_Send_String_WithLoc(1,1,"ADC_Value =");
						LCD_Send_Integer_WithLoc(1,13,Res,2);
						
						if (DataIsRead == true)
						{
						DataIsRead = false ;
						Speed = handler_messageRX.MesgData[0];	
						}
						LCD_Send_String_WithLoc(2,1,"Speed_Value=");
						LCD_Send_Integer_WithLoc(2,13,Speed,2);	
						
    }
}


ISR(INT0_vect)
{
	DataIsRead = true ;
	MCP2515_receiveMesg(&handler_messageRX); // receive message with ID 0x124 

}
