/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "tft.h"
#include "spi.h"
#include "delay.h"

typedef struct
{
    vu16 LCD_REG;
    vu16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(vu16 regval) {
    regval=regval;//ʹ��-O2�Ż���ʱ��,����������ʱ
    LCD->LCD_REG=regval;//д��Ҫд�ļĴ������
}

//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(vu16 data) {
    data=data;//ʹ��-O2�Ż���ʱ��,����������ʱ
    LCD->LCD_RAM=data;
}

//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void) {
    vu16 ram;//��ֹ���Ż�
    ram=LCD->LCD_RAM;
    return ram;
}

//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue) {
    LCD->LCD_REG = LCD_Reg;//д��Ҫд�ļĴ������
    LCD->LCD_RAM = LCD_RegValue;//д������
}

//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u16 LCD_Reg) {
    LCD_WR_REG(LCD_Reg);//д��Ҫ���ļĴ������
    delay_us(5); 
    return LCD_RD_DATA();//���ض�����ֵ
}

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void) {
    LCD->LCD_REG = 0X2C;
}

void tft_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
    FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB15 �������,���Ʊ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ�� //PB15 �������,���Ʊ���
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//rst
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ�� //PB15 �������,���Ʊ���
    PDout(7) = 1;
    
    GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14,15 AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  

    GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15,AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12

    GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);


    readWriteTiming.FSMC_AddressSetupTime = 0XF;     //��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns    
    readWriteTiming.FSMC_AddressHoldTime = 0x00;     //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�    
    readWriteTiming.FSMC_DataSetupTime = 60;            //���ݱ���ʱ��Ϊ60��HCLK    =6*60=360ns
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;     //ģʽA 


    writeTiming.FSMC_AddressSetupTime =9;          //��ַ����ʱ�䣨ADDSET��Ϊ9��HCLK =54ns 
    writeTiming.FSMC_AddressHoldTime = 0x00;     //��ַ����ʱ�䣨A        
    writeTiming.FSMC_DataSetupTime = 8;         //���ݱ���ʱ��Ϊ6ns*9��HCLK=54ns
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;     //ģʽA 

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;    //  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 

    delay_ms(50); // delay 50 ms 
    LCD_WriteReg(0x0000,0x0001);
    delay_ms(50); // delay 50 ms 
    uint16_t id = LCD_ReadReg(0x0000);
    if(id<0XFF||id==0XFFFF||id==0X9300)//����ID����ȷ,����id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
    {
        //����9341 ID�Ķ�ȡ        
        LCD_WR_REG(0XD3);                   
        id=LCD_RD_DATA();    //dummy read     
        id=LCD_RD_DATA();    //����0X00
        id=LCD_RD_DATA();       //��ȡ93                                   
        id<<=8;
        id|=LCD_RD_DATA();      //��ȡ41                       
        if(id!=0X9341)        //��9341,�����ǲ���6804
        {    
            LCD_WR_REG(0XBF);                   
            id=LCD_RD_DATA();     //dummy read      
            id=LCD_RD_DATA();       //����0X01               
            id=LCD_RD_DATA();     //����0XD0                   
            id=LCD_RD_DATA();    //�������0X68 
            id<<=8;
            id|=LCD_RD_DATA();    //�������0X04      
            if(id!=0X6804)        //Ҳ����6804,���Կ����ǲ���NT35310
            { 
                LCD_WR_REG(0XD4);                   
                id=LCD_RD_DATA();//dummy read  
                id=LCD_RD_DATA();//����0X01     
                id=LCD_RD_DATA();//����0X53    
                id<<=8;     
                id|=LCD_RD_DATA();    //�������0X10     
                if(id!=0X5310)        //Ҳ����NT35310,���Կ����ǲ���NT35510
                {
                    LCD_WR_REG(0XDA00);    
                    id=LCD_RD_DATA();        //����0X00     
                    LCD_WR_REG(0XDB00);    
                    id=LCD_RD_DATA();        //����0X80
                    id<<=8;    
                    LCD_WR_REG(0XDC00);    
                    id|=LCD_RD_DATA();        //����0X00        
                    if(id==0x8000)id=0x5510;//NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510
                    if(id!=0X5510)            //Ҳ����NT5510,���Կ����ǲ���SSD1963
                    {
                        LCD_WR_REG(0XA1);
                        id=LCD_RD_DATA();
                        id=LCD_RD_DATA();    //����0X57
                        id<<=8;     
                        id|=LCD_RD_DATA();    //����0X61    
                        if(id==0X5761)id=0X1963;//SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963
                    }
                }
            }
        }      
    } 
    if(id == 0X9341) {
        //��������дʱ����ƼĴ�����ʱ��   
        FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//��ַ����ʱ��(ADDSET)����      
        FSMC_Bank1E->BWTR[6]&=~(0XF<<8);//���ݱ���ʱ������
        FSMC_Bank1E->BWTR[6]|=3<<0;        //��ַ����ʱ��(ADDSET)Ϊ3��HCLK =18ns       
        FSMC_Bank1E->BWTR[6]|=2<<8;     //���ݱ���ʱ��(DATAST)Ϊ6ns*3��HCLK=18ns
        LCD_WR_REG(0xCF);  
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0xC1); 
        LCD_WR_DATA(0X30); 
        LCD_WR_REG(0xED);  
        LCD_WR_DATA(0x64); 
        LCD_WR_DATA(0x03); 
        LCD_WR_DATA(0X12); 
        LCD_WR_DATA(0X81); 
        LCD_WR_REG(0xE8);  
        LCD_WR_DATA(0x85); 
        LCD_WR_DATA(0x10); 
        LCD_WR_DATA(0x7A); 
        LCD_WR_REG(0xCB);  
        LCD_WR_DATA(0x39); 
        LCD_WR_DATA(0x2C); 
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x34); 
        LCD_WR_DATA(0x02); 
        LCD_WR_REG(0xF7);  
        LCD_WR_DATA(0x20); 
        LCD_WR_REG(0xEA);  
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x00); 
        LCD_WR_REG(0xC0);    //Power control 
        LCD_WR_DATA(0x1B);   //VRH[5:0] 
        LCD_WR_REG(0xC1);    //Power control 
        LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
        LCD_WR_REG(0xC5);    //VCM control 
        LCD_WR_DATA(0x30);      //3F
        LCD_WR_DATA(0x30);      //3C
        LCD_WR_REG(0xC7);    //VCM control2 
        LCD_WR_DATA(0XB7); 
        LCD_WR_REG(0x36);    // Memory Access Control 
        LCD_WR_DATA(0x48); 
        LCD_WR_REG(0x3A);   
        LCD_WR_DATA(0x55); 
        LCD_WR_REG(0xB1);   
        LCD_WR_DATA(0x00);   
        LCD_WR_DATA(0x1A); 
        LCD_WR_REG(0xB6);    // Display Function Control 
        LCD_WR_DATA(0x0A); 
        LCD_WR_DATA(0xA2); 
        LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA(0x00); 
        LCD_WR_REG(0x26);    //Gamma curve selected 
        LCD_WR_DATA(0x01); 
        LCD_WR_REG(0xE0);    //Set Gamma 
        LCD_WR_DATA(0x0F); 
        LCD_WR_DATA(0x2A); 
        LCD_WR_DATA(0x28); 
        LCD_WR_DATA(0x08); 
        LCD_WR_DATA(0x0E); 
        LCD_WR_DATA(0x08); 
        LCD_WR_DATA(0x54); 
        LCD_WR_DATA(0XA9); 
        LCD_WR_DATA(0x43); 
        LCD_WR_DATA(0x0A); 
        LCD_WR_DATA(0x0F); 
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x00);          
        LCD_WR_REG(0XE1);    //Set Gamma 
        LCD_WR_DATA(0x00); 
        LCD_WR_DATA(0x15); 
        LCD_WR_DATA(0x17); 
        LCD_WR_DATA(0x07); 
        LCD_WR_DATA(0x11); 
        LCD_WR_DATA(0x06); 
        LCD_WR_DATA(0x2B); 
        LCD_WR_DATA(0x56); 
        LCD_WR_DATA(0x3C); 
        LCD_WR_DATA(0x05); 
        LCD_WR_DATA(0x10); 
        LCD_WR_DATA(0x0F); 
        LCD_WR_DATA(0x3F); 
        LCD_WR_DATA(0x3F); 
        LCD_WR_DATA(0x0F); 
        LCD_WR_REG(0x2B); 
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x01);
        LCD_WR_DATA(0x3f);
        LCD_WR_REG(0x2A); 
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0xef);
        LCD_WR_REG(0x11); //Exit Sleep
        delay_ms(120);
        LCD_WR_REG(0x29); //display on
        GPIO_SetBits(GPIOF,GPIO_Pin_6);
        //lcd_fill(0,0,240,240,PINK);
        lcd_set_dir(0);
    }
}

//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���              
void LCD_Scan_Dir(u8 dir,uint16_t width,uint16_t height) {
    u16 regval=0;
    u16 dirreg=0;
    u16 temp;  
    switch(dir) {
        case L2R_U2D://������,���ϵ���
            regval|=(0<<7)|(0<<6)|(0<<5); 
        break;
        case L2R_D2U://������,���µ���
            regval|=(1<<7)|(0<<6)|(0<<5); 
        break;
        case R2L_U2D://���ҵ���,���ϵ���
            regval|=(0<<7)|(1<<6)|(0<<5); 
        break;
        case R2L_D2U://���ҵ���,���µ���
            regval|=(1<<7)|(1<<6)|(0<<5); 
        break;     
        case U2D_L2R://���ϵ���,������
            regval|=(0<<7)|(0<<6)|(1<<5); 
        break;
        case U2D_R2L://���ϵ���,���ҵ���
            regval|=(0<<7)|(1<<6)|(1<<5); 
        break;
        case D2U_L2R://���µ���,������
            regval|=(1<<7)|(0<<6)|(1<<5); 
        break;
        case D2U_R2L://���µ���,���ҵ���
            regval|=(1<<7)|(1<<6)|(1<<5); 
        break;
    }
    LCD_WriteReg(dirreg,regval);
    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(0);LCD_WR_DATA(0);
    LCD_WR_DATA((width-1)>>8);LCD_WR_DATA((width-1)&0XFF);
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(0);LCD_WR_DATA(0);
    LCD_WR_DATA((height-1)>>8);LCD_WR_DATA((height-1)&0XFF);
}     
//����LCD��ʾ����
//dir:0,������1,����
void lcd_set_dir(u8 dir) {
    if(dir==0) {//����
        LCD_Scan_Dir(DFT_SCAN_DIR,240,320);
    } else {//����
        LCD_Scan_Dir(DFT_SCAN_DIR,320,240);
    }
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{ 
    LCD_WR_REG(0X2A); 
    LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 
    LCD_WR_REG(0X2B); 
    LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF);
} 

void lcd_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color) {
    u16 i,j;
    u16 xlen=0;
    u16 temp;
    xlen=ex-sx+1;     
    for(i=sy;i<=ey;i++) {
        LCD_SetCursor(sx,i);                      //���ù��λ�� 
        LCD_WriteRAM_Prepare();                 //��ʼд��GRAM      
        for(j=0;j<xlen;j++)LCD->LCD_RAM=color;    //��ʾ��ɫ         
    } 
}  

void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p) {
    u16 i,j;
    u16 xlen=0;
    u16 temp;
    xlen=ex-sx+1;
    for(i=sy;i<=ey;i++) {
        LCD_SetCursor(sx,i);//���ù��λ�� 
        LCD_WriteRAM_Prepare();//��ʼд��GRAM
        for(j=0;j<xlen;j++) {
            LCD->LCD_RAM=*color_p;
            color_p ++;
        }
    } 
    lv_flush_ready();
}
