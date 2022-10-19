#include "touch.h"
#include "lcd.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h"

_m_tp_dev tp_dev =
	{
		TP_Init,
		TP_Scan,
		TP_Adjust,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
};

uint8_t CMD_RDX = 0XD0;
uint8_t CMD_RDY = 0X90;

void TP_Write_Byte(uint8_t num)
{
	uint8_t count = 0;
	for (count = 0; count < 8; count++)
	{
		if (num & 0x80)
			TDIN_H;
		else
			TDIN_L;
		num <<= 1;
		TCLK_L;
		TCLK_H;
	}
}

uint16_t TP_Read_AD(uint8_t CMD)
{
	uint8_t count = 0;
	uint16_t Num = 0;
	TCLK_L;
	TDIN_L;
	TCS_L;
	TP_Write_Byte(CMD);
	TCLK_L;
	TCLK_H;
	TCLK_L;
	for (count = 0; count < 16; count++)
	{
		Num <<= 1;
		TCLK_L;
		TCLK_H;
		if (DOUT_R)
			Num++;
	}
	Num >>= 4;
	TCS_H;
	return (Num);
}

#define READ_TIMES 5
#define LOST_VAL 1
uint16_t TP_Read_XOY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum = 0;
	uint16_t temp;
	for (i = 0; i < READ_TIMES; i++)
		buf[i] = TP_Read_AD(xy);
	for (i = 0; i < READ_TIMES - 1; i++)
	{
		for (j = i + 1; j < READ_TIMES; j++)
		{
			if (buf[i] > buf[j])
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)
		sum += buf[i];
	temp = sum / (READ_TIMES - 2 * LOST_VAL);
	return temp;
}

uint8_t TP_Read_XY(uint16_t *x, uint16_t *y)
{
	uint16_t xtemp, ytemp;
	xtemp = TP_Read_XOY(CMD_RDX);
	ytemp = TP_Read_XOY(CMD_RDY);
	// if(xtemp<100||ytemp<100)return 0;
	*x = xtemp;
	*y = ytemp;
	return 1;
}

#define ERR_RANGE 50
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y)
{
	uint16_t x1, y1;
	uint16_t x2, y2;
	uint8_t flag;
	flag = TP_Read_XY(&x1, &y1);
	if (flag == 0)
		return (0);
	flag = TP_Read_XY(&x2, &y2);
	if (flag == 0)
		return (0);
	if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
	{
		*x = (x1 + x2) / 2;
		*y = (y1 + y2) / 2;
		return 1;
	}
	else
		return 0;
}

void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color)
{
	POINT_COLOR = color;
	LCD_DrawLine(x - 12, y, x + 13, y);
	LCD_DrawLine(x, y - 12, x, y + 13);
	LCD_DrawPoint(x + 1, y + 1);
	LCD_DrawPoint(x - 1, y + 1);
	LCD_DrawPoint(x + 1, y - 1);
	LCD_DrawPoint(x - 1, y - 1);
	LCD_Draw_Circle(x, y, 6);
}

void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
	POINT_COLOR = color;
	LCD_DrawPoint(x, y);
	LCD_DrawPoint(x + 1, y);
	LCD_DrawPoint(x, y + 1);
	LCD_DrawPoint(x + 1, y + 1);
}

uint8_t TP_Scan(uint8_t tp)
{
	if (PEN_R == 0)
	{
		if (tp)
			TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);
		else if (TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]))
		{
			tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] + tp_dev.xoff;
			tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
		}
		if ((tp_dev.sta & TP_PRES_DOWN) == 0)
		{
			tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES;
			tp_dev.x[4] = tp_dev.x[0];
			tp_dev.y[4] = tp_dev.y[0];
		}
	}
	else
	{
		if (tp_dev.sta & TP_PRES_DOWN)
		{
			tp_dev.sta &= ~(1 << 7);
		}
		else
		{
			tp_dev.x[4] = 0;
			tp_dev.y[4] = 0;
			tp_dev.x[0] = 0xffff;
			tp_dev.y[0] = 0xffff;
		}
	}
	return tp_dev.sta & TP_PRES_DOWN;
}

#define SAVE_ADDR_BASE 40
void TP_Save_Adjdata(void)
{
	AT24CXX_Write(SAVE_ADDR_BASE, (uint8_t *)&tp_dev.xfac, 14);
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE + 14, 0X0A);
}

uint8_t TP_Get_Adjdata(void)
{
	uint8_t temp;
	temp = AT24CXX_ReadOneByte(SAVE_ADDR_BASE + 14);
	if (temp == 0X0A)
	{
		AT24CXX_Read(SAVE_ADDR_BASE, (uint8_t *)&tp_dev.xfac, 14);
		if (tp_dev.touchtype)
		{
			CMD_RDX = 0X90;
			CMD_RDY = 0XD0;
		}
		else
		{
			CMD_RDX = 0XD0;
			CMD_RDY = 0X90;
		}
		return 1;
	}
	return 0;
}

uint8_t *const TP_REMIND_MSG_TBL = "Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

void TP_Adj_Info_Show(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fac)
{
	POINT_COLOR = RED;
	LCD_ShowString(40, 160, lcddev.width, lcddev.height, 16, "x1:");
	LCD_ShowString(40 + 80, 160, lcddev.width, lcddev.height, 16, "y1:");
	LCD_ShowString(40, 180, lcddev.width, lcddev.height, 16, "x2:");
	LCD_ShowString(40 + 80, 180, lcddev.width, lcddev.height, 16, "y2:");
	LCD_ShowString(40, 200, lcddev.width, lcddev.height, 16, "x3:");
	LCD_ShowString(40 + 80, 200, lcddev.width, lcddev.height, 16, "y3:");
	LCD_ShowString(40, 220, lcddev.width, lcddev.height, 16, "x4:");
	LCD_ShowString(40 + 80, 220, lcddev.width, lcddev.height, 16, "y4:");
	LCD_ShowString(40, 240, lcddev.width, lcddev.height, 16, "fac is:");
	LCD_ShowNum(40 + 24, 160, x0, 4, 16);
	LCD_ShowNum(40 + 24 + 80, 160, y0, 4, 16);
	LCD_ShowNum(40 + 24, 180, x1, 4, 16);
	LCD_ShowNum(40 + 24 + 80, 180, y1, 4, 16);
	LCD_ShowNum(40 + 24, 200, x2, 4, 16);
	LCD_ShowNum(40 + 24 + 80, 200, y2, 4, 16);
	LCD_ShowNum(40 + 24, 220, x3, 4, 16);
	LCD_ShowNum(40 + 24 + 80, 220, y3, 4, 16);
	LCD_ShowNum(40 + 56, 240, fac, 3, 16);
}

void TP_Adjust(void)
{
	uint16_t pos_temp[4][2];
	uint16_t d1, d2;
	uint32_t tem1, tem2;
	double fac;
	uint16_t outtime = 0;
	uint8_t cnt = 0;
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	LCD_Clear(WHITE);
	POINT_COLOR = RED;
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(40, 40, 160, 100, 16, (uint8_t *)TP_REMIND_MSG_TBL);
	TP_Drow_Touch_Point(20, 20, RED);
	tp_dev.sta = 0;
	tp_dev.xfac = 0;
	while (1)
	{
		tp_dev.scan(1);
		if ((tp_dev.sta & 0xc0) == TP_CATH_PRES)
		{
			outtime = 0;
			tp_dev.sta &= ~(1 << 6);

			pos_temp[cnt][0] = tp_dev.x[0];
			pos_temp[cnt][1] = tp_dev.y[0];
			cnt++;
			switch (cnt)
			{
			case 1:
				TP_Drow_Touch_Point(20, 20, WHITE);
				TP_Drow_Touch_Point(lcddev.width - 20, 20, RED);
				break;
			case 2:
				TP_Drow_Touch_Point(lcddev.width - 20, 20, WHITE);
				TP_Drow_Touch_Point(20, lcddev.height - 20, RED);
				break;
			case 3:
				TP_Drow_Touch_Point(20, lcddev.height - 20, WHITE);
				TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, RED);
				break;
			case 4:

				tem1 = abs(pos_temp[0][0] - pos_temp[1][0]);
				tem2 = abs(pos_temp[0][1] - pos_temp[1][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d1 = sqrt(tem1 + tem2);

				tem1 = abs(pos_temp[2][0] - pos_temp[3][0]);
				tem2 = abs(pos_temp[2][1] - pos_temp[3][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d2 = sqrt(tem1 + tem2);
				fac = (float)d1 / d2;
				if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0)
				{
					cnt = 0;
					TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE);																							 //清除点4
					TP_Drow_Touch_Point(20, 20, RED);																															 //画点1
					TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
					continue;
				}
				tem1 = abs(pos_temp[0][0] - pos_temp[2][0]);
				tem2 = abs(pos_temp[0][1] - pos_temp[2][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d1 = sqrt(tem1 + tem2);

				tem1 = abs(pos_temp[1][0] - pos_temp[3][0]);
				tem2 = abs(pos_temp[1][1] - pos_temp[3][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d2 = sqrt(tem1 + tem2);
				fac = (float)d1 / d2;
				if (fac < 0.95 || fac > 1.05)
				{
					cnt = 0;
					TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE);																							 //清除点4
					TP_Drow_Touch_Point(20, 20, RED);																															 //画点1
					TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
					continue;
				}

				tem1 = abs(pos_temp[1][0] - pos_temp[2][0]);
				tem2 = abs(pos_temp[1][1] - pos_temp[2][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d1 = sqrt(tem1 + tem2);

				tem1 = abs(pos_temp[0][0] - pos_temp[3][0]);
				tem2 = abs(pos_temp[0][1] - pos_temp[3][1]);
				tem1 *= tem1;
				tem2 *= tem2;
				d2 = sqrt(tem1 + tem2);
				fac = (float)d1 / d2;
				if (fac < 0.95 || fac > 1.05)
				{
					cnt = 0;
					TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE);																							 //清除点4
					TP_Drow_Touch_Point(20, 20, RED);																															 //画点1
					TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
					continue;
				}
				tp_dev.xfac = (float)(lcddev.width - 40) / (pos_temp[1][0] - pos_temp[0][0]);
				tp_dev.xoff = (lcddev.width - tp_dev.xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2;

				tp_dev.yfac = (float)(lcddev.height - 40) / (pos_temp[2][1] - pos_temp[0][1]);
				tp_dev.yoff = (lcddev.height - tp_dev.yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2;
				if (abs(tp_dev.xfac) > 2 || abs(tp_dev.yfac) > 2)
				{
					cnt = 0;
					TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE);
					TP_Drow_Touch_Point(20, 20, RED);
					LCD_ShowString(40, 26, lcddev.width, lcddev.height, 16, "TP Need readjust!");
					tp_dev.touchtype = !tp_dev.touchtype;
					if (tp_dev.touchtype)
					{
						CMD_RDX = 0X90;
						CMD_RDY = 0XD0;
					}
					else
					{
						CMD_RDX = 0XD0;
						CMD_RDY = 0X90;
					}
					continue;
				}
				POINT_COLOR = BLUE;
				LCD_Clear(WHITE);
				LCD_ShowString(35, 110, lcddev.width, lcddev.height, 16, "Touch Screen Adjust OK!");
				HAL_Delay(1000);
				TP_Save_Adjdata();
				LCD_Clear(WHITE);
				return;
			}
		}
		HAL_Delay(10);
		outtime++;
		if (outtime > 1000)
		{
			TP_Get_Adjdata();
			break;
		}
	}
}

uint8_t TP_Init(void)
{
	TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]);
	if (TP_Get_Adjdata())
		return 0;
	else
	{
		LCD_Clear(WHITE);
		TP_Adjust();
	}
	TP_Get_Adjdata();
	return 1;
}
