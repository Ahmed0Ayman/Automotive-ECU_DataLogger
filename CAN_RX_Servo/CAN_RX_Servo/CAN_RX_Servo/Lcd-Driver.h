 /*
 * Lcd_Driver.h
 *
 * Created: 4/12/2021 3:14:06 PM
 * Author : Ahmed_Ayman
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_
#define  F_CPU 16000000
#include <stdint.h>
#include <util/delay.h>
#include "HAL_GPIO.h"


/*
these are the connected pins of LCD
you can change the pin in here 
*/ 
#define LCD_RW_GPIOB (GPIO_PIN_2)
#define LCD_RS_GPIOB (GPIO_PIN_1)
#define LCD_EN_GPIOB (GPIO_PIN_3)
#define LCD_D4_GPIOA (GPIO_PIN_4)
#define LCD_D5_GPIOA (GPIO_PIN_5)
#define LCD_D6_GPIOA (GPIO_PIN_6)
#define LCD_D7_GPIOA (GPIO_PIN_7)



/*some of common commands that used with LCD*/
#define LCD_COMMANED_CLEAR_LCD						0X01U
#define LCD_COMMANED_RETURN_HOME					0X02U
#define LCD_COMMANED_SHIFT_RIGHT					0X05U
#define LCD_COMMANED_SHIFT_LEFT						0X07U
#define LCD_COMMANED_CLEAR_CURSOR_ON				0X0AU
#define LCD_COMMANED_CLEAR_CURSOR_OFF				0X0CU
#define LCD_COMMANED2LINE_4_BIT_MODE                0x28U




/*
 *brief  : this function used to initialize (GPIO &LCD) 
 * return : void 
 */  
void LCD_Initializaion(void);

/* 
 *brief  : this function used to send an character to LCD with the current location 
 * param. : Character the specific character to send to LCD 
 * return : void 
 */  
void LCD_Send_Character_CurrLoc(uint8_t character);

/*
 * brief  : this function used to send an character to LCD with the chosen location 
 * param. :  y  is specify the new Y axises
 * param. :  x  is specify the new X axises
 * param. : character the specific character to send to LCD 
 * return : void 
 */
void LCD_Send_Character_WithLoc(uint8_t y,uint8_t x,uint8_t character);

/*
 * brief  : this function used to send an string to LCD with the current location 
 * param. : Character the specific character to send to LCD 
 * return : void 
 */
void LCD_Send_String_CurrLoc(uint8_t *string);

/*
 * brief  : this function used to send command to LCD  
 * param. : command the specific command to send to LCD chose one of @ LCD_COMMANED_ 
 * return : void 
 */
void LCD_Send_Command(uint8_t command);

/*
 * brief  : this function used to modify the current cursor location 
 * param. :  y  is specify the new Y axises
 * param. :  x  is specify the new X axises
 * return : void 
 */
void LCD_Goto_Location(uint8_t y , uint8_t x);

/*
 * brief  : this function used to send an string to LCD with the chosen location 
 * param. :  y  is specify y axises location start from 1
 * param. :  x  is specify y axises location start from 1
 * param. : StringOfCharacters pointer to string that you want to display
 * return : void 
 */
void LCD_Send_String_WithLoc(uint8_t y, uint8_t x, uint8_t *StringOfCharacters);

/*
 * brief  : this function used to send an 16 bit integer to LCD with the chosen location 
 * param. :  y  is specify y axises location start from 1
 * param. :  x  is specify y axises location start from 1
 * param. : IntegerToDisplay this is the integer value that you want to display
 * param. : NumberOfDigits number of digits of the integer number that you want to display
 * return : void 
 */
void LCD_Send_Integer_WithLoc(uint8_t y, uint8_t x, uint16_t IntegerToDisplay, uint8_t NumberOfDigits);


/*
 * brief  : this function used to send an floating point number to LCD with the chosen location 
 * param. :  y  is specify y axises location start from 1 
 * param. :  x  is specify y axises location start from 1
 * param. : IntegerToDisplay this is the integer value that you want to display
 * return : void 
 */
void LCD_Send_Float_Withloc(uint8_t y, uint8_t x ,  float number );

/* 
 * brief  : this function used to send an 16 bit integer to LCD with the current location 
 * param. : IntegerToDisplay this is the integer value that you want to display
 * param. : NumberOfDigits number of digits of the integer number that you want to display
 * return : void 
 */
void LCD_Send_Integer_CurrLoc(uint16_t IntegerToDisplay, uint8_t NumberOfDigits);




#endif /* LCD-DRIVER_H_ */