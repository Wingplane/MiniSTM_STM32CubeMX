#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "lcd.h"
								 
extern void led_set(uint8_t sta);
extern void test_fun(void(*ledset)(uint8_t),uint8_t sta);										  
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"uint32_t read_addr(uint32_t addr)",
	(void*)write_addr,"void write_addr(uint32_t addr,uint32_t val)",	 
#endif		   
 	(void*)HAL_Delay,"void HAL_Delay(uint32_t Delay)",	 
	(void*)LCD_Clear,"void LCD_Clear(uint16_t Color)",
	(void*)LCD_Fill,"void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)",
	(void*)LCD_DrawLine,"void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)",
	(void*)LCD_DrawRectangle,"void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)",
	(void*)LCD_Draw_Circle,"void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)",
	(void*)LCD_ShowNum,"void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)",
	(void*)LCD_ShowString,"void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)",
	(void*)LCD_Fast_DrawPoint,"void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)",
	(void*)LCD_ReadPoint,"uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)",							 
	(void*)LCD_Display_Dir,"void LCD_Display_Dir(uint8_t dir)",
	(void*)LCD_ShowxNum,"void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)", 
		
	(void*)led_set,"void led_set(uint8_t sta)",
	(void*)test_fun,"void test_fun(void(*ledset)(uint8_t),uint8_t sta)",						
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















