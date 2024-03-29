#ifndef __STM32L476G_DISCOVERY_LCD_H
#define __STM32L476G_DISCOVERY_LCD_H

#include <stdint.h>
#include <stdlib.h>

#define bool _Bool

void LCD_Initialization(void);
void LCD_bar(void);
void LCD_Clock_Init(void);
void LCD_PIN_Init(void);
void LCD_Configure(void);
void LCD_Clear(void);
void LCD_DisplayString(uint8_t* ptr);
void LCD_WriteChar(uint8_t* ch, bool point, bool colon, uint8_t position);
static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool colon, uint8_t* digit);
void LCD_Display_Name(void);
void LCD_Display_Time(uint32_t ms);
void LCD_DispGame(uint32_t num, char* str);

#endif /* __STM32L476G_DISCOVERY_LCD_H */
