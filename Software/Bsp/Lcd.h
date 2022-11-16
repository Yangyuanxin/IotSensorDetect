#ifndef _LCD_H_
#define _LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32l4xx_hal.h"

extern uint16_t	POINT_COLOR;		//Default brush color
extern uint16_t	BACK_COLOR;		//Default background color

//Width and height definitions of LCD
#define LCD_Width 	240
#define LCD_Height 	240

//Brush color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0 //��ɫ
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



/*
	LCD_PWR:	PB15
	LCD_RST:	PC7
	LCD_DC:		PC6
*/
#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET))
#define	LCD_DC(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))


void LCD_Init(void);																	//Init
void LCD_DisplayOn(void);																//Open display
void LCD_DisplayOff(void);																//Close display
void LCD_Write_HalfWord(const uint16_t da);												//Write half a byte of data to LCD
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);				//Setting up the data display area
void LCD_Clear(uint16_t color);															//Clean screen
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				//Filled monochrome
void LCD_Draw_Point(uint16_t x, uint16_t y);														//Draw points
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color);										//Painting with color dots
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);										//Draw line
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//Draw rectangle
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);												//Circle drawing
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size);										//Display a character
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);									//Display a number
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);							//Display number
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p);					//display string
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);					//display picture
void PutChinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint8_t mode);
void PutChinese_strings(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint8_t mode);

	
#ifdef __cplusplus
}
#endif
#endif  //ifndef _LCD_H_
