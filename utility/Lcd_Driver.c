#include "Lcd_Driver.h"

  
/***************************************************************************************
STM32����ƽ̨����:
�����壺����ԭ��MiniSTM32������
MCU ��STM32_F103_RBT6
���� ��12MHZ
��Ƶ ��72MHZ
����˵��:
//-------------------------------------------------------------------------------------
#define LCD_CTRL   	  	GPIOB		//����TFT���ݶ˿�
#define LCD_LED        	GPIO_Pin_9  //PB9 ������TFT -LED
#define LCD_RS         	GPIO_Pin_10	//PB10������TFT --RS
#define LCD_CS        	GPIO_Pin_11 //PB11 ������TFT --CS
#define LCD_RST     	GPIO_Pin_12	//PB12������TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13������TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15������TFT - SDI
#define LCD_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	
//VCC:���Խ�5VҲ���Խ�3.3V
//LED:���Խ�5VҲ���Խ�3.3V����ʹ���������IO����(�ߵ�ƽʹ��)
//GND���ӵ�Դ��
//˵��������Ҫ��������ռ��IO�����Խ�LCD_CS�ӵأ�LCD_LED��3.3V��LCD_RST������Ƭ����λ�ˣ�
//�������ͷ�3������IO
//�ӿڶ�����Lcd_Driver.h�ڶ��壬
//������IO�ӷ������������ʵ�ʽ����޸���ӦIO��ʼ��LCD_GPIO_Init()
//-----------------------------------------------------------------------------------------
���̹���˵����
1.	��ˢ������
2.	Ӣ����ʾ����ʾ��
3.	������ʾ����ʾ��
4.	�����������ʾʾ��
5.	ͼƬ��ʾʾ��
6.	2D�����˵�ʾ��
7.	������֧�ֺ���/�����л�(�����USE_HORIZONTAL,���Lcd_Driver.h)
8.	������֧�����ģ��SPI/Ӳ��SPI�л�(�����USE_HARDWARE_SPI,���Lcd_Driver.h)
**********************************************************************************************/


//---------------------------------function----------------------------------------------------//

/****************************************************************************
* ��    �ƣ�void LCD_GPIO_Init(void)
* ��    �ܣ�STM32_ģ��SPI���õ���GPIO��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʼ��ģ��SPI���õ�GPIO
****************************************************************************/
void LCD_GPIO_Init(void)
{
        pinMode(SPI_CS_PIN, OUTPUT);
        pinMode(LCD_RS_PIN, OUTPUT);
        pinMode(SPI_SDA_PIN, OUTPUT);
        pinMode(SPI_DCLK_PIN, OUTPUT);
        pinMode(LCD_RST_PIN, OUTPUT);
        pinMode(LCD_LED_PIN, OUTPUT);

        digitalWrite(LCD_LED_PIN, 1);
}
/****************************************************************************
* ��    �ƣ�void  SPIv_WriteData(uint8_t Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void  SPIv_WriteData(uint8_t Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //�������
      else LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}

/****************************************************************************
* ��    �ƣ�uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
* ��    �ܣ�STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
* ��ڲ�����SPIx,Byte
* ���ڲ��������������յ�������
* ˵    ����STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
// uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
// {
// 	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//�ȴ���������	  
// 	SPIx->DR=Byte;	 	//����һ��byte   
// 	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//�ȴ�������һ��byte  
// 	return SPIx->DR;          	     //�����յ�������			
// } 


/****************************************************************************
* ��    �ƣ�SPI2_Init(void)
* ��    �ܣ�STM32_SPI2Ӳ�����ó�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����STM32_SPI2Ӳ�����ó�ʼ��
****************************************************************************/
void SPI2_Init(void)	
{
//         #define	LCD_CS_SET  	digitalWrite(SPI_CS_PIN, 1)   
// #define	LCD_RS_SET  	digitalWrite(LCD_RS_PIN, 1) 
// #define	LCD_SDA_SET  	digitalWrite(SPI_SDA_PIN, 1)  
// #define	LCD_SCL_SET  	digitalWrite(SPI_DCLK_PIN, 1)
// #define	LCD_RST_SET  	digitalWrite(LCD_RST_PIN, 1)    
// #define	LCD_LED_SET  	digitalWrite(LCD_LED_PIN, 1)   
        pinMode(SPI_CS_PIN, OUTPUT);
        pinMode(LCD_RS_PIN, OUTPUT);
        pinMode(SPI_SDA_PIN, OUTPUT);
        pinMode(SPI_DCLK_PIN, OUTPUT);
        pinMode(LCD_RST_PIN, OUTPUT);
        pinMode(LCD_LED_PIN, OUTPUT);

        digitalWrite(LCD_LED_PIN, 1);
	// SPI_InitTypeDef  SPI_InitStructure;
	// GPIO_InitTypeDef GPIO_InitStructure;
	 
	// //����SPI2�ܽ�
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;    
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	// GPIO_Init(GPIOB, &GPIO_InitStructure);  

	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// //SPI2����ѡ��
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	   
	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI_Init(SPI2, &SPI_InitStructure);

	// //ʹ��SPI2
	// SPI_Cmd(SPI2, ENABLE);   
}

/****************************************************************************
* ��    �ƣ�Lcd_WriteIndex(uint8_t Index)
* ��    �ܣ���Һ����дһ��8λָ��
* ��ڲ�����Index   �Ĵ�����ַ
* ���ڲ�������
* ˵    ��������ǰ����ѡ�п��������ڲ�����
****************************************************************************/
void Lcd_WriteIndex(uint8_t Index)
{
   LCD_CS_CLR;
   LCD_RS_CLR;
#if USE_HARDWARE_SPI   
//    SPI_WriteByte(SPI2,Index);
#else
   SPIv_WriteData(Index);
#endif 
   LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�Lcd_WriteData(uint8_t Data)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
void Lcd_WriteData(uint8_t Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
#if USE_HARDWARE_SPI   
   SPI_WriteByte(SPI2,Data);
#else
   SPIv_WriteData(Data);
#endif 
   LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�void LCD_WriteReg(uint8_t Index,uint16_t Data)
* ��    �ܣ�д�Ĵ�������
* ��ڲ�����Index,Data
* ���ڲ�������
* ˵    ����������Ϊ��Ϻ�������Index��ַ�ļĴ���д��Dataֵ
****************************************************************************/
void LCD_WriteReg(uint8_t Index,uint16_t Data)
{
	Lcd_WriteIndex(Index);
  	Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* ��    �ƣ�void Lcd_WriteData_16Bit(uint16_t Data)
* ��    �ܣ���Һ����дһ��16λ����
* ��ڲ�����Data
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��16λ����
****************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data)
{	
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);	
}

/****************************************************************************
* ��    �ƣ�void Lcd_Reset(void)
* ��    �ܣ�Һ��Ӳ��λ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��ǰ��ִ��һ�θ�λ����
****************************************************************************/
void Lcd_Reset(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);
}
/****************************************************************************
* ��    �ƣ�void Lcd_Init(void)
* ��    �ܣ�Һ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��_ILI9225_176X220
****************************************************************************/
void Lcd_Init(void)
{	
#if USE_HARDWARE_SPI //ʹ��Ӳ��SPI
	SPI2_Init();
#else	
	LCD_GPIO_Init();//ʹ��ģ��SPI
#endif
	Lcd_Reset(); //Reset before LCD Init.

	Lcd_WriteIndex(0xCB);  
        Lcd_WriteData(0x39); 
        Lcd_WriteData(0x2C); 
        Lcd_WriteData(0x00); 
        Lcd_WriteData(0x34); 
        Lcd_WriteData(0x02); 

        Lcd_WriteIndex(0xCF);  
        Lcd_WriteData(0x00); 
        Lcd_WriteData(0XC1); 
        Lcd_WriteData(0X30); 
 
        Lcd_WriteIndex(0xE8);  
        Lcd_WriteData(0x85); 
        Lcd_WriteData(0x00); 
        Lcd_WriteData(0x78); 
 
        Lcd_WriteIndex(0xEA);  
        Lcd_WriteData(0x00); 
        Lcd_WriteData(0x00); 
 
        Lcd_WriteIndex(0xED);  
        Lcd_WriteData(0x64); 
        Lcd_WriteData(0x03); 
        Lcd_WriteData(0X12); 
        Lcd_WriteData(0X81); 

        Lcd_WriteIndex(0xF7);  
        Lcd_WriteData(0x20); 
  
        Lcd_WriteIndex(0xC0);    //Power control 
        Lcd_WriteData(0x23);   //VRH[5:0] 
 
        Lcd_WriteIndex(0xC1);    //Power control 
        Lcd_WriteData(0x10);   //SAP[2:0];BT[3:0] 
 
        Lcd_WriteIndex(0xC5);    //VCM control 
        Lcd_WriteData(0x3e); //�Աȶȵ���
        Lcd_WriteData(0x28); 
 
        Lcd_WriteIndex(0xC7);    //VCM control2 
        Lcd_WriteData(0x86);  //--
 
        Lcd_WriteIndex(0x36);    // Memory Access Control 
#ifdef USE_HORIZONTAL
        Lcd_WriteData(0xE8); //C8	   //48 68����//28 E8 ����
#else
		Lcd_WriteData(0x48); 
#endif

        Lcd_WriteIndex(0x3A);    
        Lcd_WriteData(0x55); 

        Lcd_WriteIndex(0xB1);    
        Lcd_WriteData(0x00);  
        Lcd_WriteData(0x18); 
 
        Lcd_WriteIndex(0xB6);    // Display Function Control 
        Lcd_WriteData(0x08); 
        Lcd_WriteData(0x82);
        Lcd_WriteData(0x27);  
 
        Lcd_WriteIndex(0xF2);    // 3Gamma Function Disable 
        Lcd_WriteData(0x00); 
 
        Lcd_WriteIndex(0x26);    //Gamma curve selected 
        Lcd_WriteData(0x01); 
 
        Lcd_WriteIndex(0xE0);    //Set Gamma 
        Lcd_WriteData(0x0F); 
        Lcd_WriteData(0x31); 
        Lcd_WriteData(0x2B); 
        Lcd_WriteData(0x0C); 
        Lcd_WriteData(0x0E); 
        Lcd_WriteData(0x08); 
        Lcd_WriteData(0x4E); 
        Lcd_WriteData(0xF1); 
        Lcd_WriteData(0x37); 
        Lcd_WriteData(0x07); 
        Lcd_WriteData(0x10); 
        Lcd_WriteData(0x03); 
        Lcd_WriteData(0x0E); 
        Lcd_WriteData(0x09); 
        Lcd_WriteData(0x00); 

        Lcd_WriteIndex(0XE1);    //Set Gamma 
        Lcd_WriteData(0x00); 
        Lcd_WriteData(0x0E); 
        Lcd_WriteData(0x14); 
        Lcd_WriteData(0x03); 
        Lcd_WriteData(0x11); 
        Lcd_WriteData(0x07); 
        Lcd_WriteData(0x31); 
        Lcd_WriteData(0xC1); 
        Lcd_WriteData(0x48); 
        Lcd_WriteData(0x08); 
        Lcd_WriteData(0x0F); 
        Lcd_WriteData(0x0C); 
        Lcd_WriteData(0x31); 
        Lcd_WriteData(0x36); 
        Lcd_WriteData(0x0F); 
 
        Lcd_WriteIndex(0x11);    //Exit Sleep 
        delay_ms(120); 
				
        Lcd_WriteIndex(0x29);    //Display on 
        Lcd_WriteIndex(0x2c); 
	
        Lcd_Clear(GREEN);//����	
}



/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(uint16_t Xpos, uint16_t Ypos)
{	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData_16Bit(Xpos);
	Lcd_WriteIndex(0x2b);
	Lcd_WriteData_16Bit(Ypos);
	Lcd_WriteIndex(0x2c);	
} 
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
//������ʾ����
void Lcd_SetRegion(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData_16Bit(xStar);
	Lcd_WriteData_16Bit(xEnd);
	Lcd_WriteIndex(0x2b);
	Lcd_WriteData_16Bit(yStar);
	Lcd_WriteData_16Bit(yEnd);
	Lcd_WriteIndex(0x2c);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�����xy�������ɫ����
����ֵ����
*************************************************/
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);

}    

/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(uint16_t Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   
}


/*****************************************
 ????:?TFT??????                          
 ????:color  ????                                 
******************************************/
uint8_t Lcd_ReadData(uint8_t Data)
{
	uint8_t temp;
	LCD_CS_CLR;
  LCD_RS_SET; 
//   temp = SPI_WriteByte(SPI2,Data);
 // LCD_CS_SET;
	return temp;
}

uint16_t Lcd_ReadPoint(uint16_t x,uint16_t y)
{

	uint8_t r=0,g=0,b=0;
	uint16_t R,G,B,Data;
	Lcd_SetRegion(x,y,x,y);  
	Lcd_WriteIndex(0X2E); //?????????
//	LCD_CS_CLR;
//  LCD_RS_SET;
  Lcd_ReadData(0xff);
  Lcd_ReadData(0xff);
	g=Lcd_ReadData(0xff); //?????
	b=Lcd_ReadData(0xff); //?????
	r=Lcd_ReadData(0xff); //�������������ǰ���GBR˳���
//	LCD_CS_SET;
	
//	R = (r<<1)&0x00FF;
//	G = g&0x00FF;
//	B = (b<<1)&0x00FF;
	 
//	Data = 	(R<<8)|(G<<5)|(B>>3);
//	Data = (((SPI_WriteByte(SPI2,0xff)) & 0x007c) << 9) | (((SPI_WriteByte(SPI2,0xff))) << 4) | (((SPI_WriteByte(SPI2,0xff))) >> 2);
	//Data = (((Lcd_ReadData(0xff)) & 0x007c) << 9) | (((Lcd_ReadData(0xff))) << 4) | (((Lcd_ReadData(0xff))) >> 2);
	R = (r >> 3) & 0x00ff;
	G = (g >> 2) & 0x00ff;
	B = (b >> 3) & 0x00ff;
	
	Data = (R << 11) | (G << 5) | (B);
	return Data;
}
