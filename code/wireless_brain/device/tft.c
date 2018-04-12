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

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(vu16 regval) {
    regval=regval;//使用-O2优化的时候,必须插入的延时
    LCD->LCD_REG=regval;//写入要写的寄存器序号
}

//写LCD数据
//data:要写入的值
void LCD_WR_DATA(vu16 data) {
    data=data;//使用-O2优化的时候,必须插入的延时
    LCD->LCD_RAM=data;
}

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void) {
    vu16 ram;//防止被优化
    ram=LCD->LCD_RAM;
    return ram;
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue) {
    LCD->LCD_REG = LCD_Reg;//写入要写的寄存器序号
    LCD->LCD_RAM = LCD_RegValue;//写入数据
}

//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u16 LCD_Reg) {
    LCD_WR_REG(LCD_Reg);//写入要读的寄存器序号
    delay_us(5); 
    return LCD_RD_DATA();//返回读到的值
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void) {
    LCD->LCD_REG = 0X2C;
}

void LCD_SSD_BackLightSet(u8 pwm)
{	
	LCD_WR_REG(0xBE);	//配置PWM输出
	LCD_WR_DATA(0x05);	//1设置PWM频率
	LCD_WR_DATA(pwm*2.55);//2设置PWM占空比
	LCD_WR_DATA(0x01);	//3设置C
	LCD_WR_DATA(0xFF);	//4设置D
	LCD_WR_DATA(0x00);	//5设置E
	LCD_WR_DATA(0x00);	//6设置F
}


#define LCD_ILI9341_CMD  LCD_WR_REG
#define LCD_ILI9341_Parameter LCD_WR_DATA
#define delayms  delay_ms

void ILI9341_HSD32_Initial(void)
{
//************* Start Initial Sequence **********//
	//20171122 OK
LCD_ILI9341_CMD(0xcf); 
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0xC1);//0x99
LCD_ILI9341_Parameter(0x30);
LCD_ILI9341_CMD(0xed); 
LCD_ILI9341_Parameter(0x64);
LCD_ILI9341_Parameter(0x03);
LCD_ILI9341_Parameter(0x12);
LCD_ILI9341_Parameter(0x81);
LCD_ILI9341_CMD(0xe8); 
LCD_ILI9341_Parameter(0x85);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x78);	
LCD_ILI9341_CMD(0xcb); 
LCD_ILI9341_Parameter(0x39);
LCD_ILI9341_Parameter(0x2c);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x34);
LCD_ILI9341_Parameter(0x02);
LCD_ILI9341_CMD(0xF7); 
LCD_ILI9341_Parameter(0x20);
LCD_ILI9341_CMD(0xea); 
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);

LCD_ILI9341_CMD(0xC0); //Power control
LCD_ILI9341_Parameter(0x23); //VRH[5:0]  23 20171122

LCD_ILI9341_CMD(0xC1); //Power control
LCD_ILI9341_Parameter(0x12); //SAP[2:0];BT[3:0]

LCD_ILI9341_CMD(0xC2);
LCD_ILI9341_Parameter(0x11);

LCD_ILI9341_CMD(0xC5); //VCM control
LCD_ILI9341_Parameter(0x40);
LCD_ILI9341_Parameter(0x30);//调对比度，调大变浅，调小变深 

LCD_ILI9341_CMD(0xC7); 
LCD_ILI9341_Parameter(0xA8);//调水波纹,VCOML调小时，C7要调大  B6  A5

LCD_ILI9341_CMD(0xB1); // Frame Rate Control
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x18);//18

LCD_ILI9341_CMD(0x3A); 
LCD_ILI9341_Parameter(0x55);

//LCD_ILI9341_CMD(0x36); // Memory Access Control
//LCD_ILI9341_Parameter(0x08);//48

LCD_ILI9341_CMD(0x36);     //set the model of scanning
//LCD_ILI9341_Parameter((1<<5)|(0<<6)|(1<<7)|(1<<3)); //左横屏
LCD_ILI9341_Parameter((1<<5)|(1<<6)|(1<<3));//右横屏幕
//LCD_ILI9341_Parameter(0x08); 竖屏幕
LCD_ILI9341_CMD(0x2B);     //set the page address 横屏幕设置
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0xEF);
LCD_ILI9341_CMD(0x2A);    //set the column address
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x01);
LCD_ILI9341_Parameter(0x3F);
//        LCD_ILI9341_CMD(0x2A);     //set the page address 竖屏幕设置
//        LCD_ILI9341_Parameter(0x00);
//        LCD_ILI9341_Parameter(0x00);
//        LCD_ILI9341_Parameter(0x00);
//        LCD_ILI9341_Parameter(0xEF);
//        LCD_ILI9341_CMD(0x2B);    //set the column address
//        LCD_ILI9341_Parameter(0x00);
//        LCD_ILI9341_Parameter(0x00);
//        LCD_ILI9341_Parameter(0x01);
//        LCD_ILI9341_Parameter(0x3F);


LCD_ILI9341_CMD(0xB6); // Display Function Control
LCD_ILI9341_Parameter(0x0a);
LCD_ILI9341_Parameter(0xa2);

LCD_ILI9341_CMD(0xF6); //20171122
LCD_ILI9341_Parameter(0x01);
LCD_ILI9341_Parameter(0x30);

LCD_ILI9341_CMD(0xF2); // 3Gamma Function Disable
LCD_ILI9341_Parameter(0x00);

LCD_ILI9341_CMD(0xF7);//Pump ratio control (F7h)
LCD_ILI9341_Parameter(0x20);

LCD_ILI9341_CMD(0x26); //Gamma curve selected
LCD_ILI9341_Parameter(0x01);

LCD_ILI9341_CMD(0xE0); //Set Gamma
LCD_ILI9341_Parameter(0x1f);
LCD_ILI9341_Parameter(0x24);
LCD_ILI9341_Parameter(0x23);
LCD_ILI9341_Parameter(0x0b);
LCD_ILI9341_Parameter(0x0f);
LCD_ILI9341_Parameter(0x08);
LCD_ILI9341_Parameter(0x50);
LCD_ILI9341_Parameter(0xd8);
LCD_ILI9341_Parameter(0x3b);
LCD_ILI9341_Parameter(0x08);
LCD_ILI9341_Parameter(0x0a);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x00);

LCD_ILI9341_CMD(0XE1); //Set Gamma
LCD_ILI9341_Parameter(0x00);
LCD_ILI9341_Parameter(0x1b);
LCD_ILI9341_Parameter(0x1c);
LCD_ILI9341_Parameter(0x04);
LCD_ILI9341_Parameter(0x10);
LCD_ILI9341_Parameter(0x07);
LCD_ILI9341_Parameter(0x2f);
LCD_ILI9341_Parameter(0x27);
LCD_ILI9341_Parameter(0x44);
LCD_ILI9341_Parameter(0x07);
LCD_ILI9341_Parameter(0x15);
LCD_ILI9341_Parameter(0x0f);
LCD_ILI9341_Parameter(0x3f);
LCD_ILI9341_Parameter(0x3f);
LCD_ILI9341_Parameter(0x1F);

LCD_ILI9341_CMD(0x11); //Exit Sleep
delay_ms(60);
LCD_ILI9341_CMD(0x29); //display on

}

void tft_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
    FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能PD,PE,PF,PG时钟  
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB15 推挽输出,控制背光
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化 //PB15 推挽输出,控制背光
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//rst
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化 //PB15 推挽输出,控制背光
    PDout(7) = 0;
    delay_ms(150);
    PDout(7) = 1;
    
    GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14,15 AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化  

    GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15,AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 

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


    readWriteTiming.FSMC_AddressSetupTime = 0XF;     //地址建立时间（ADDSET）为16个HCLK 1/168M=6ns*16=96ns    
    readWriteTiming.FSMC_AddressHoldTime = 0x00;     //地址保持时间（ADDHLD）模式A未用到    
    readWriteTiming.FSMC_DataSetupTime = 60;            //数据保存时间为60个HCLK    =6*60=360ns
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;     //模式A 


    writeTiming.FSMC_AddressSetupTime =9;          //地址建立时间（ADDSET）为9个HCLK =54ns 
    writeTiming.FSMC_AddressHoldTime = 0x00;     //地址保持时间（A        
    writeTiming.FSMC_DataSetupTime = 8;         //数据保存时间为6ns*9个HCLK=54ns
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;     //模式A 

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;    //  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 

    delay_ms(50); // delay 50 ms 
    LCD_WriteReg(0x0000,0x0001);
    delay_ms(50); // delay 50 ms 
    uint16_t id = LCD_ReadReg(0x0000);
    if(id<0XFF||id==0XFFFF||id==0X9300)//读到ID不正确,新增id==0X9300判断，因为9341在未被复位的情况下会被读成9300
    {
        //尝试9341 ID的读取        
        LCD_WR_REG(0XD3);                   
        id=LCD_RD_DATA();    //dummy read     
        id=LCD_RD_DATA();    //读到0X00
        id=LCD_RD_DATA();       //读取93                                   
        id<<=8;
        id|=LCD_RD_DATA();      //读取41                       
        if(id!=0X9341)        //非9341,尝试是不是6804
        {    
            LCD_WR_REG(0XBF);                   
            id=LCD_RD_DATA();     //dummy read      
            id=LCD_RD_DATA();       //读回0X01               
            id=LCD_RD_DATA();     //读回0XD0                   
            id=LCD_RD_DATA();    //这里读回0X68 
            id<<=8;
            id|=LCD_RD_DATA();    //这里读回0X04      
            if(id!=0X6804)        //也不是6804,尝试看看是不是NT35310
            { 
                LCD_WR_REG(0XD4);                   
                id=LCD_RD_DATA();//dummy read  
                id=LCD_RD_DATA();//读回0X01     
                id=LCD_RD_DATA();//读回0X53    
                id<<=8;     
                id|=LCD_RD_DATA();    //这里读回0X10     
                if(id!=0X5310)        //也不是NT35310,尝试看看是不是NT35510
                {
                    LCD_WR_REG(0XDA00);    
                    id=LCD_RD_DATA();        //读回0X00     
                    LCD_WR_REG(0XDB00);    
                    id=LCD_RD_DATA();        //读回0X80
                    id<<=8;    
                    LCD_WR_REG(0XDC00);    
                    id|=LCD_RD_DATA();        //读回0X00        
                    if(id==0x8000)id=0x5510;//NT35510读回的ID是8000H,为方便区分,我们强制设置为5510
                    if(id!=0X5510)            //也不是NT5510,尝试看看是不是SSD1963
                    {
                        LCD_WR_REG(0XA1);
                        id=LCD_RD_DATA();
                        id=LCD_RD_DATA();    //读回0X57
                        id<<=8;     
                        id|=LCD_RD_DATA();    //读回0X61    
                        if(id==0X5761)id=0X1963;//SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963
                    }
                }
            }
        }      
    } 
    if(id == 0X9341) {
        //重新配置写时序控制寄存器的时序   
        FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//地址建立时间(ADDSET)清零      
        FSMC_Bank1E->BWTR[6]&=~(0XF<<8);//数据保存时间清零
        FSMC_Bank1E->BWTR[6]|=3<<0;        //地址建立时间(ADDSET)为3个HCLK =18ns       
        FSMC_Bank1E->BWTR[6]|=2<<8;     //数据保存时间(DATAST)为6ns*3个HCLK=18ns
        ILI9341_HSD32_Initial();
        GPIO_SetBits(GPIOF,GPIO_Pin_6);
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
        LCD_SetCursor(sx,i);                      //设置光标位置 
        LCD_WriteRAM_Prepare();                 //开始写入GRAM      
        for(j=0;j<xlen;j++)LCD->LCD_RAM=color;    //显示颜色         
    } 
}  

void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p) {
    u16 i,j;
    u16 xlen=0;
    u16 temp;
    xlen=ex-sx+1;
    for(i=sy;i<=ey;i++) {
        LCD_SetCursor(sx,i);//设置光标位置 
        LCD_WriteRAM_Prepare();//开始写入GRAM
        for(j=0;j<xlen;j++) {
            LCD->LCD_RAM=*color_p;
            color_p ++;
        }
    } 
    lv_flush_ready();
}
