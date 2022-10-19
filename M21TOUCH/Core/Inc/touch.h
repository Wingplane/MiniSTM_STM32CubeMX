#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f1xx_hal.h"

#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40
#define CT_MAX_TOUCH 5

typedef struct
{
	uint8_t (*init)(void);
	uint8_t (*scan)(uint8_t);
	void (*adjust)(void);
	uint16_t x[CT_MAX_TOUCH];
	uint16_t y[CT_MAX_TOUCH];
	uint8_t sta;
	float xfac;
	float yfac;
	short xoff;
	short yoff;
	uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;

#define PEN_R HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)
#define DOUT_R HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)
#define TDIN_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)
#define TDIN_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)
#define TCLK_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define TCLK_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define TCS_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define TCS_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)

void TP_Write_Byte(uint8_t num);
uint16_t TP_Read_AD(uint8_t CMD);
uint16_t TP_Read_XOY(uint8_t xy);
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y);
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y);
void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color);
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color);
void TP_Save_Adjdata(void);
uint8_t TP_Get_Adjdata(void);
void TP_Adjust(void);
void TP_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac);

uint8_t TP_Scan(uint8_t tp);
uint8_t TP_Init(void);

#endif
