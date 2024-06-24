/**
 * @file : lcd.h
 * @brief : LCD driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.1
 * @date : 2024-06-24
 */
#ifndef __LCD_H
#define __LCD_H

#include "stdlib.h"
#include "stm32f1xx_hal.h"

// LCD important parameters
typedef struct
{
  uint16_t width;   // LCD width
  uint16_t height;  // LCD height
  uint16_t id;      // LCD ID
  uint8_t dir;      // LCD direction, 0: vertical, 1: horizontal
  uint16_t wramcmd; // start write GRAM register command
  uint16_t setxcmd; // set x coordinate command
  uint16_t setycmd; // set y coordinate command
} _lcd_dev;

// LCD parameters
extern _lcd_dev lcddev; // management LCD parameters
// LCD color
extern uint16_t POINT_COLOR; // dot color. Default is red
extern uint16_t BACK_COLOR;  // background color. Default is white

// LCD address
#define LCD_LED *((volatile unsigned long *)((((GPIOC_BASE + 12) & 0xF0000000) + 0x2000000 + (((GPIOC_BASE + 12) & 0xFFFFF) << 5) + (10 << 2)))) // LCD backlight control, PC10

#define LCD_CS_SET GPIOC->BSRR = 1 << 9 // chip select    PC9
#define LCD_RS_SET GPIOC->BSRR = 1 << 8 // data/command   PC8
#define LCD_WR_SET GPIOC->BSRR = 1 << 7 // write data     PC7
#define LCD_RD_SET GPIOC->BSRR = 1 << 6 // read data      PC6

#define LCD_CS_CLR GPIOC->BRR = 1 << 9 // chip select    PC9
#define LCD_RS_CLR GPIOC->BRR = 1 << 8 // data/command   PC8
#define LCD_WR_CLR GPIOC->BRR = 1 << 7 // write data     PC7
#define LCD_RD_CLR GPIOC->BRR = 1 << 6 // read data      PC6

// PB0~15,set as data line
#define DATAOUT(x) GPIOB->ODR = x; // data output
#define DATAIN GPIOB->IDR;         // data input

// scan direction definition
#define L2R_U2D 0 // from left to right, from up to down
#define L2R_D2U 1 // from left to right, from down to up
#define R2L_U2D 2 // from right to left, from up to down
#define R2L_D2U 3 // from right to left, from down to up

#define U2D_L2R 4 // from up to down, from left to right
#define U2D_R2L 5 // from up to down, from right to left
#define D2U_L2R 6 // from down to up, from left to right
#define D2U_R2L 7 // from down to up, from right to left

#define DFT_SCAN_DIR L2R_U2D // default scan direction

// scan direction definition
#define L2R_U2D 0 // from left to right, from up to down
#define L2R_D2U 1 // from left to right, from down to up
#define R2L_U2D 2 // from right to left, from up to down
#define R2L_D2U 3 // from right to left, from down to up

#define U2D_L2R 4 // from up to down, from left to right
#define U2D_R2L 5 // from up to down, from right to left
#define D2U_L2R 6 // from down to up, from left to right
#define D2U_R2L 7 // from down to up, from right to left

#define DFT_SCAN_DIR L2R_U2D // default scan direction

// LCD color
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
// GUI color

#define DARKBLUE 0X01CF  // dark blue
#define LIGHTBLUE 0X7D7C // light blue
#define GRAYBLUE 0X5458  // gray blue
// PANNEL color

#define LIGHTGREEN 0X841F // light green
#define LGRAY 0XC618      // light gray(PANNEL)

#define LGRAYBLUE 0XA651 // light gray blue(middle layer color)
#define LBBLUE 0X2B12    // light brown blue(selcet entry color)

void LCD_Init(void);                                                                                    // initailize LCD
void LCD_DisplayOn(void);                                                                               // open display
void LCD_DisplayOff(void);                                                                              // close display
void LCD_Clear(uint16_t Color);                                                                         // clear screen
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);                                                       // set cursor
void LCD_DrawPoint(uint16_t x, uint16_t y);                                                             // draw point
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color);                                        // draw point fast
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);                                                         // read point
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);                                              // draw circle
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                                  // draw line
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                             // draw rectangle
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);                      // fill color
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);               // fill color with selected color
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);                     // show a character
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);                      // show a number
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);       // show x-digit numbers
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p); // show a string with 12x16 font

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);                     // write register
uint16_t LCD_ReadReg(uint16_t LCD_Reg);                                         // read register
void LCD_WriteRAM_Prepare(void);                                                // write GRAM
void LCD_WriteRAM(uint16_t RGB_Code);                                           // write GRAM
void LCD_Scan_Dir(uint8_t dir);                                                 // set scan direction
void LCD_Display_Dir(uint8_t dir);                                              // set display direction
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height); // set window

// write data
#define LCD_WR_DATA(data) \
  {                       \
    LCD_RS_SET;           \
    LCD_CS_CLR;           \
    DATAOUT(data);        \
    LCD_WR_CLR;           \
    LCD_WR_SET;           \
    LCD_CS_SET;           \
  }

#endif // !__LCD_H
