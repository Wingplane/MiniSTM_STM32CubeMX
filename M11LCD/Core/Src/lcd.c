/**
 * @file : lcd.c
 * @brief : LCD driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.1
 * @date : 2024-06-24
 */
#include "lcd.h"
#include "gpio.h"
#include "stdlib.h"
#include "font.h"
#include "usart.h"
#include "uart.h"

// LCD color
uint16_t POINT_COLOR = 0x0000; // Foreground color
uint16_t BACK_COLOR = 0xFFFF;  // Background color

_lcd_dev lcddev; // LCD backlight  0: off, 1: on

/**
 * @brief - LCD write register
 * @param data data to write
 */
void LCD_WR_REG(uint16_t data)
{
  LCD_RS_CLR;    // write register
  LCD_CS_CLR;    // chip select
  DATAOUT(data); // write data
  LCD_WR_CLR;    // write enable
  LCD_WR_SET;    // write disable
  LCD_CS_SET;    // chip deselect
}
/**
 * @brief - LCD write data
 * @param data data to write
 */
void LCD_WR_DATAX(uint16_t data)
{
  LCD_RS_SET;    // write data
  LCD_CS_CLR;    // chip select
  DATAOUT(data); // write data
  LCD_WR_CLR;    // write enable
  LCD_WR_SET;    // write disable
  LCD_CS_SET;    // chip deselect
}

/**
 * @brief - LCD read data
 * @retval - t : data read
 */
uint16_t LCD_RD_DATA(void)
{
  uint16_t t;              // data read
  GPIOB->CRL = 0X88888888; // PB0-7 pull-up input
  GPIOB->CRH = 0X88888888; // PB8-15 pull-up input
  GPIOB->ODR = 0X0000;     // all output low

  LCD_RS_SET; // read data
  LCD_CS_CLR; // chip select
  LCD_RD_CLR; // read enable
  t = DATAIN; // read data
  LCD_RD_SET; // read disable
  LCD_CS_SET; // chip deselect

  GPIOB->CRL = 0X33333333; // PB0-7 pull-up output
  GPIOB->CRH = 0X33333333; // PB8-15 pull-up output
  GPIOB->ODR = 0XFFFF;     // all output high
  return t;                // return data
}

/**
 * @brief - LCD write data
 * @param LCD_Reg register number
 * @param LCD_RegValue register value
 */
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);       // write register
  LCD_WR_DATA(LCD_RegValue); // write data
}

/**
 * @brief - LCD read register
 * @param LCD_Reg register number
 * @retval - register : value
 */
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
  LCD_WR_REG(LCD_Reg);  // write register
  return LCD_RD_DATA(); // read data
}

/**
 * @brief - LCD write GRAM prepare
 * @param data data to write
 */
void LCD_WriteRAM_Prepare(void)
{
  LCD_WR_REG(lcddev.wramcmd); // write GRAM
}

/**
 * @brief - LCD write GRAM
 * @param RGB_Code RGB color code
 */
void LCD_WriteRAM(uint16_t RGB_Code)
{
  LCD_WR_DATA(RGB_Code); // write RGB color code
}

/**
 * @brief - LCD color conversion GBR to RGB
 * @param c GBR color
 * @retval - RGB color
 */
uint16_t LCD_BGR2RGB(uint16_t c)
{
  uint16_t r, g, b, rgb;
  b = (c >> 0) & 0x1f;
  g = (c >> 5) & 0x3f;
  r = (c >> 11) & 0x1f;
  rgb = (b << 11) + (g << 5) + (r << 0);
  return (rgb);
}

/**
 * @brief - delay
 * @param i delay time
 */
void opt_delay(uint8_t i)
{
  while (i--)
    ;
}

/**
 * @brief - LCD read point color
 * @param x x coordinate
 * @param y y coordinate
 * @retval - color
 */
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
  uint16_t r, g, b;
  if (x >= lcddev.width || y >= lcddev.height) // out of range, return 0
    return 0;
  LCD_SetCursor(x, y);     // set cursor
  LCD_WR_REG(0X2E);        // ILI9341
  GPIOB->CRL = 0X88888888; // PB0-7 pull-up input
  GPIOB->CRH = 0X88888888; // PB8-15 pull-up input
  GPIOB->ODR = 0XFFFF;     // all output high
  LCD_RS_SET;              // read data
  LCD_CS_CLR;              // chip select
  // dummy read
  LCD_RD_CLR;   // read enable
  opt_delay(2); // delay
  r = DATAIN;   // read red and green color
  LCD_RD_SET;   // read disable
  // real read
  LCD_RD_CLR;                                                // read enable
  opt_delay(2);                                              // delay
  r = DATAIN;                                                // read red and green color
  LCD_RD_SET;                                                // read disable
  LCD_RD_CLR;                                                // read enable
  opt_delay(2);                                              //  delay
  b = DATAIN;                                                // read blue color
  LCD_RD_SET;                                                // read disable
  g = r & 0XFF;                                              // for ILI9341, the first read is the value of RG, with R in the front and G in the back, each occupying 8 bits
  g <<= 8;                                                   // shift 8 bits
  LCD_CS_SET;                                                // chip deselect
  GPIOB->CRL = 0X33333333;                                   // PB0-7 pull-up output
  GPIOB->CRH = 0X33333333;                                   // PB8-15 pull-up output
  GPIOB->ODR = 0XFFFF;                                       // all output high
  return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); // ILI9341 is GBR color, so need to convert to RGB color
}

/**
 * @brief - LCD display on
 */
void LCD_DisplayOn(void)
{
  LCD_WR_REG(0X29); // display on
}

/**
 * @brief - LCD display off
 */
void LCD_DisplayOff(void)
{
  LCD_WR_REG(0X28); // display off
}

/**
 * @brief - LCD set cursor
 * @param Xpos x coordinate
 * @param Ypos y coordinate
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  LCD_WR_REG(lcddev.setxcmd); // set x coordinate
  LCD_WR_DATA(Xpos >> 8);     // high 8 bits
  LCD_WR_DATA(Xpos & 0XFF);   // low 8 bits
  LCD_WR_REG(lcddev.setycmd); // set y coordinate
  LCD_WR_DATA(Ypos >> 8);     // high 8 bits
  LCD_WR_DATA(Ypos & 0XFF);   // low 8 bits
}
// 设置LCD的自动扫描方向
// 注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
// 所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//  dir:0~7,代表8个方向(具体定义见lcd.h)
//  9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963等IC已经实际测试

/**
 * @brief - LCD scan direction
 * @brief - warning 9341/6804/5310/5510/1963, special processing
 * @param dir:0~7, 8 directions
 */
void LCD_Scan_Dir(uint8_t dir)
{
  uint16_t regval = 0; // register value
  uint16_t dirreg = 0; // direction register
  if (lcddev.dir == 1) // current direction is horizontal
  {
    switch (dir) // convert to vertical direction
    {
    case 0:
      dir = 6;
      break;
    case 1:
      dir = 7;
      break;
    case 2:
      dir = 4;
      break;
    case 3:
      dir = 5;
      break;
    case 4:
      dir = 1;
      break;
    case 5:
      dir = 0;
      break;
    case 6:
      dir = 3;
      break;
    case 7:
      dir = 2;
      break;
    }
  }
  else // 9341/6804/5310/5510/1963, special processing
  {
    switch (dir) // convert to vertical direction
    {
    case L2R_U2D: // from left to right, from up to down
      regval |= (0 << 7) | (0 << 6) | (0 << 5);
      break;
    case L2R_D2U: // from left to right, from down to up
      regval |= (1 << 7) | (0 << 6) | (0 << 5);
      break;
    case R2L_U2D: // from right to left, from up to down
      regval |= (0 << 7) | (1 << 6) | (0 << 5);
      break;
    case R2L_D2U: // from right to left, from down to up
      regval |= (1 << 7) | (1 << 6) | (0 << 5);
      break;
    case U2D_L2R: // from up to down, from left to right
      regval |= (0 << 7) | (0 << 6) | (1 << 5);
      break;
    case U2D_R2L: // from up to down, from right to left
      regval |= (0 << 7) | (1 << 6) | (1 << 5);
      break;
    case D2U_L2R: // from down to up, from left to right
      regval |= (1 << 7) | (0 << 6) | (1 << 5);
      break;
    case D2U_R2L: // from down to up, from right to left
      regval |= (1 << 7) | (1 << 6) | (1 << 5);
      break;
    }

    dirreg = 0X36;
    regval |= 0X08;               // 5310/5510/1963 do not need RGB
    LCD_WriteReg(dirreg, regval); // write register
  }
}

/**
 * @brief - LCD draw point
 * @param x x coordinate
 * @param y y coordinate
 */
void LCD_DrawPoint(uint16_t x, uint16_t y)
{
  LCD_SetCursor(x, y);      // set cursor
  LCD_WriteRAM_Prepare();   // write GRAM prepare
  LCD_WR_DATA(POINT_COLOR); // write color
}

/**
 * @brief - LCD fast draw point
 * @param x x coordinate
 * @param y y coordinate
 * @param color color
 */
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_WR_REG(lcddev.setxcmd); // set x coordinate
  LCD_WR_DATA(x >> 8);        // high 8 bits
  LCD_WR_DATA(x & 0XFF);      // low 8 bits
  LCD_WR_REG(lcddev.setycmd); // set y coordinate
  LCD_WR_DATA(y >> 8);        // high 8 bits
  LCD_WR_DATA(y & 0XFF);      // low 8 bits
  LCD_RS_CLR;                 // write register
  LCD_CS_CLR;                 // chip select
  DATAOUT(lcddev.wramcmd);    // write cmd
  LCD_WR_CLR;                 // write enable
  LCD_WR_SET;                 // write disable
  LCD_CS_SET;                 // chip deselect
  LCD_WR_DATA(color);         // write color
}

/**
 * @brief - LCD display direction
 * @param dir direction
 */
void LCD_Display_Dir(uint8_t dir)
{
  if (dir == 0) // portrait
  {
    lcddev.dir = 0;        // portrait
    lcddev.width = 240;    // width
    lcddev.height = 320;   // height
    lcddev.wramcmd = 0X2C; // write GRAM command
    lcddev.setxcmd = 0X2A; // set x coordinate command
    lcddev.setycmd = 0X2B; // set y coordinate command
  }
  else // landscape
  {
    lcddev.dir = 1;        // landscape
    lcddev.width = 320;    // width
    lcddev.height = 240;   // height
    lcddev.wramcmd = 0X2C; // write GRAM command
    lcddev.setxcmd = 0X2A; // set x coordinate command
    lcddev.setycmd = 0X2B; // set y coordinate command
  }
  LCD_Scan_Dir(DFT_SCAN_DIR); // set scan direction
}

/**
 * @brief - LCD set window
 * @param sx start x coordinate
 * @param sy start y coordinate
 * @param width width
 * @param height height
 */
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
  uint16_t twidth, theight;
  twidth = sx + width - 1;     // total width
  theight = sy + height - 1;   // total height
  LCD_WR_REG(lcddev.setxcmd);  // set x coordinate
  LCD_WR_DATA(sx >> 8);        // high 8 bits
  LCD_WR_DATA(sx & 0XFF);      // low 8 bits
  LCD_WR_DATA(twidth >> 8);    // high 8 bits
  LCD_WR_DATA(twidth & 0XFF);  // low 8 bits
  LCD_WR_REG(lcddev.setycmd);  // set y coordinate
  LCD_WR_DATA(sy >> 8);        // high 8 bits
  LCD_WR_DATA(sy & 0XFF);      // low 8 bits
  LCD_WR_DATA(theight >> 8);   // high 8 bits
  LCD_WR_DATA(theight & 0XFF); // low 8 bits
}

/**
 * @brief - LCD init
 */
void LCD_Init(void)
{
  __HAL_AFIO_REMAP_SWJ_DISABLE();                                     // disable JTAG
  HAL_Delay(50);                                                      // delay 50 ms
  LCD_WriteReg(0x0000, 0x0001);                                       // start internal oscillator
  HAL_Delay(50);                                                      // delay 50 ms
  lcddev.id = LCD_ReadReg(0x0000);                                    // read ID
  if (lcddev.id < 0XFF || lcddev.id == 0XFFFF || lcddev.id == 0X9300) // sometimes the ID is not correct, try to read again
  {
    // try to read 9341 ID
    LCD_WR_REG(0XD3);           // read ID
    LCD_RD_DATA();              // dummy read
    LCD_RD_DATA();              //  read 00
    lcddev.id = LCD_RD_DATA();  // read 93
    lcddev.id <<= 8;            // shift 8 bits
    lcddev.id |= LCD_RD_DATA(); // read 41
  }
  printf(" LCD ID:%x\r\n", lcddev.id); // print ID
  LCD_WR_REG(0xCF);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0xC1);
  LCD_WR_DATAX(0X30);
  LCD_WR_REG(0xED);
  LCD_WR_DATAX(0x64);
  LCD_WR_DATAX(0x03);
  LCD_WR_DATAX(0X12);
  LCD_WR_DATAX(0X81);
  LCD_WR_REG(0xE8);
  LCD_WR_DATAX(0x85);
  LCD_WR_DATAX(0x10);
  LCD_WR_DATAX(0x7A);
  LCD_WR_REG(0xCB);
  LCD_WR_DATAX(0x39);
  LCD_WR_DATAX(0x2C);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x34);
  LCD_WR_DATAX(0x02);
  LCD_WR_REG(0xF7);
  LCD_WR_DATAX(0x20);
  LCD_WR_REG(0xEA);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_REG(0xC0);   // Power control
  LCD_WR_DATAX(0x1B); // VRH[5:0]
  LCD_WR_REG(0xC1);   // Power control
  LCD_WR_DATAX(0x01); // SAP[2:0];BT[3:0]
  LCD_WR_REG(0xC5);   // VCM control
  LCD_WR_DATAX(0x30); // 3F
  LCD_WR_DATAX(0x30); // 3C
  LCD_WR_REG(0xC7);   // VCM control2
  LCD_WR_DATAX(0XB7);
  LCD_WR_REG(0x36); // Memory Access Control
  LCD_WR_DATAX(0x48);
  LCD_WR_REG(0x3A);
  LCD_WR_DATAX(0x55);
  LCD_WR_REG(0xB1);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x1A);
  LCD_WR_REG(0xB6); // Display Function Control
  LCD_WR_DATAX(0x0A);
  LCD_WR_DATAX(0xA2);
  LCD_WR_REG(0xF2); // 3Gamma Function Disable
  LCD_WR_DATAX(0x00);
  LCD_WR_REG(0x26); // Gamma curve selected
  LCD_WR_DATAX(0x01);
  LCD_WR_REG(0xE0); // Set Gamma
  LCD_WR_DATAX(0x0F);
  LCD_WR_DATAX(0x2A);
  LCD_WR_DATAX(0x28);
  LCD_WR_DATAX(0x08);
  LCD_WR_DATAX(0x0E);
  LCD_WR_DATAX(0x08);
  LCD_WR_DATAX(0x54);
  LCD_WR_DATAX(0XA9);
  LCD_WR_DATAX(0x43);
  LCD_WR_DATAX(0x0A);
  LCD_WR_DATAX(0x0F);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_REG(0XE1); // Set Gamma
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x15);
  LCD_WR_DATAX(0x17);
  LCD_WR_DATAX(0x07);
  LCD_WR_DATAX(0x11);
  LCD_WR_DATAX(0x06);
  LCD_WR_DATAX(0x2B);
  LCD_WR_DATAX(0x56);
  LCD_WR_DATAX(0x3C);
  LCD_WR_DATAX(0x05);
  LCD_WR_DATAX(0x10);
  LCD_WR_DATAX(0x0F);
  LCD_WR_DATAX(0x3F);
  LCD_WR_DATAX(0x3F);
  LCD_WR_DATAX(0x0F);
  LCD_WR_REG(0x2B);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x01);
  LCD_WR_DATAX(0x3f);
  LCD_WR_REG(0x2A);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0x00);
  LCD_WR_DATAX(0xef);
  LCD_WR_REG(0x11); // Exit Sleep
  HAL_Delay(120);
  LCD_WR_REG(0x29);   // display on
  LCD_Display_Dir(0); // 默认为竖屏
  LCD_LED = 1;        // 点亮背光
  LCD_Clear(WHITE);
}

/**
 * @brief - LCD clear screen
 * @param color color to fill
 */
void LCD_Clear(uint16_t color)
{
  uint32_t index = 0; // index
  uint32_t totalpoint = lcddev.width;
  totalpoint *= lcddev.height;                 // get total points
  LCD_SetCursor(0x00, 0x0000);                 // set cursor
  LCD_WriteRAM_Prepare();                      // write GRAM prepare
  for (index = 0; index < totalpoint; index++) // fill color
    LCD_WR_DATA(color);
}

/**
 * @brief - LCD fill color
 * @param sx start x coordinate
 * @param sy start y coordinate
 * @param ex end x coordinate
 * @param ey end y coordinate
 * @param color color to fill
 */
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
  uint16_t i, j;
  uint16_t xlen = 0;         // x length
  xlen = ex - sx + 1;        // get x length
  for (i = sy; i <= ey; i++) // fill color
  {
    LCD_SetCursor(sx, i);   // set cursor
    LCD_WriteRAM_Prepare(); // write GRAM prepare
    for (j = 0; j < xlen; j++)
      LCD_WR_DATA(color); // write color
  }
}

/**
 * @brief - LCD color fill in specified area
 * @param sx start x coordinate
 * @param sy start y coordinate
 * @param ex end x coordinate
 * @param ey end y coordinate
 * @param color color to fill
 */
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
  uint16_t height, width;
  uint16_t i, j;
  width = ex - sx + 1;         // width
  height = ey - sy + 1;        // height
  for (i = 0; i < height; i++) // fill color
  {
    LCD_SetCursor(sx, sy + i); // set cursor
    LCD_WriteRAM_Prepare();    // write GRAM prepare
    for (j = 0; j < width; j++)
      LCD_WR_DATA(color[i * width + j]); // write color
  }
}

/**
 * @brief - draw line
 * @param x1 start x coordinate
 * @param y1 start y coordinate
 * @param x2 end x coordinate
 * @param y2 end y coordinate
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  delta_x = x2 - x1; // calculate delta x
  delta_y = y2 - y1; // calculate delta y
  uRow = x1;         // set start x coordinate
  uCol = y1;         // set start y coordinate
  if (delta_x > 0)   // set increase x
    incx = 1;
  else if (delta_x == 0) // vertical line
    incx = 0;
  else // decrease x
  {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0) // set increase y
    incy = 1;
  else if (delta_y == 0) // horizontal line
    incy = 0;
  else // decrease y
  {
    incy = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y) // set distance
    distance = delta_x;
  else // set distance
    distance = delta_y;
  for (t = 0; t <= distance + 1; t++) // draw line
  {
    LCD_DrawPoint(uRow, uCol); // draw point
    xerr += delta_x;           // calculate x error
    yerr += delta_y;           // calculate y error
    if (xerr > distance)       // calculate x error
    {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) // calculate y error
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}

/**
 * @brief - draw rectangle
 * @param x1 start x coordinate
 * @param y1 start y coordinate
 * @param x2 end x coordinate
 * @param y2 end y coordinate
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  LCD_DrawLine(x1, y1, x2, y1);
  LCD_DrawLine(x1, y1, x1, y2);
  LCD_DrawLine(x1, y2, x2, y2);
  LCD_DrawLine(x2, y1, x2, y2);
}

/**
 * @brief - draw circle
 * @param x0 center x coordinate
 * @param y0 center y coordinate
 * @param r radius
 */
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
  int a, b;
  int di;
  a = 0;
  b = r;
  di = 3 - (r << 1); // calculate di
  while (a <= b)
  {
    LCD_DrawPoint(x0 + a, y0 - b); // 5
    LCD_DrawPoint(x0 + b, y0 - a); // 0
    LCD_DrawPoint(x0 + b, y0 + a); // 4
    LCD_DrawPoint(x0 + a, y0 + b); // 6
    LCD_DrawPoint(x0 - a, y0 + b); // 1
    LCD_DrawPoint(x0 - b, y0 + a); // 3
    LCD_DrawPoint(x0 - a, y0 - b); // 2
    LCD_DrawPoint(x0 - b, y0 - a); // 7
    a++;
    // use Bresenham algorithm to calculate next point
    if (di < 0)
      di += 4 * a + 6;
    else
    {
      di += 10 + 4 * (a - b);
      b--;
    }
  }
}

/**
 * @brief - show character
 * @param x x coordinate
 * @param y y coordinate
 * @param num character
 * @param size font size
 * @param mode overlay mode
 */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp, t1, t;
  uint16_t y0 = y;
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // get font size in bytes
  num = num - ' ';                                                // Obtain the offset value (ASCII font library starts from spaces, so - '' is the corresponding character's font library)
  for (t = 0; t < csize; t++)                                     // draw font
  {
    if (size == 12) // call 1206 font
      temp = asc2_1206[num][t];
    else if (size == 16) // call 1608 font
      temp = asc2_1608[num][t];
    else if (size == 24) // call 2412 font
      temp = asc2_2412[num][t];
    else // no font
      return;
    for (t1 = 0; t1 < 8; t1++)
    {
      if (temp & 0x80) // draw point
        LCD_Fast_DrawPoint(x, y, POINT_COLOR);
      else if (mode == 0) // draw background
        LCD_Fast_DrawPoint(x, y, BACK_COLOR);
      temp <<= 1; // shift left 1 bit
      y++;
      if (y >= lcddev.height) // out of range
        return;
      if ((y - y0) == size) // next column
      {
        y = y0;
        x++;
        if (x >= lcddev.width) // out of range
          return;
        break;
      }
    }
  }
}

/**
 * @brief - power calculation
 * @param m base
 * @param n power
 * @retval - result
 */
uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
  uint32_t result = 1;
  while (n--)
    result *= m;
  return result;
}

/**
 * @brief - show number
 * @param x x coordinate
 * @param y y coordinate
 * @param num number
 * @param len length
 * @param size font size
 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
  uint8_t t, temp;
  uint8_t enshow = 0;
  for (t = 0; t < len; t++)
  {
    temp = (num / LCD_Pow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
        continue;
      }
      else
        enshow = 1;
    }
    LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0);
  }
}

/**
 * @brief - show x-digit numbers
 * @param x x coordinate
 * @param y y coordinate
 * @param num number(0~999999999)
 * @param len length
 * @param size font size
 * @param mode overlay mode
 * @note - [7]:0, no fill; 1, fill 0
 * @note - [6:1]:reserved
 * @note - [0]:0, non-overlay display; 1, overlay display
 */
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
  uint8_t t, temp;
  uint8_t enshow = 0;
  for (t = 0; t < len; t++)
  {
    temp = (num / LCD_Pow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        if (mode & 0X80)
          LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01);
        else
          LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01);
        continue;
      }
      else
        enshow = 1;
    }
    LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01);
  }
}

/**
 * @brief - show string
 * @param x x coordinate
 * @param y y coordinate
 * @param width width
 * @param height height
 * @param size font size
 * @param *p string
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{
  uint8_t x0 = x;
  width += x;
  height += y;
  while ((*p <= '~') && (*p >= ' ')) // judge whether it is a valid character
  {
    if (x >= width)
    {
      x = x0;
      y += size;
    }
    if (y >= height)
      break; // exit
    LCD_ShowChar(x, y, *p, size, 0);
    x += size / 2;
    p++;
  }
}
