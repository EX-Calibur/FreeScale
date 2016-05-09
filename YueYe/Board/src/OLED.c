/*****************************************************************************/
//�ļ���OLED.c
/*****************************************************************************/

#include "VCAN_LED.h"
#include "OLED.h"
#include "6x8.H"
#include "8x16.H"
#include "GB1616.H"
#include "picture.H"
#include "MK60_gpio.h"


#define CLK_SET         GPIO_SET(PTC16,1)         //��D0
#define CLK_CLE         GPIO_SET(PTC16,0)
#define MOSI_SET        GPIO_SET(PTC17,1)       //��D1
#define MOSI_CLE        GPIO_SET(PTC17,0)
#define RST_SET         GPIO_SET(PTC18,1)      //��RST
#define RST_CLE         GPIO_SET(PTC18,0)
#define DC_SET          GPIO_SET(PTC19,1)       //��DC
#define DC_CLE          GPIO_SET(PTC19,0)


/*************************************************************/
/*                    ��OLED��д������                       */
/*************************************************************/
void OLED_WrDat(unsigned char Data)
{
    unsigned char j = 8;
    DC_SET;
    CLK_CLE;

    while (j--)
    {
        if (Data & 0x80)
            MOSI_SET;
        else
            MOSI_CLE;
        
        CLK_SET; 
        asm("nop"); 
        CLK_CLE; 
        Data <<= 1;    
    }
}

/*************************************************************/
/*                    ��OLED��д������                       */
/*************************************************************/
void OLED_WrCmd(unsigned char Data)
{
    unsigned char i = 8;
    DC_CLE;
    CLK_CLE;

    while (i--)
    {
        if (Data & 0x80)
        {
            MOSI_SET;
        }
        else
        {
            MOSI_CLE;
        }
        CLK_SET;
        asm("nop");  
        CLK_CLE;  
        asm("nop");  
        Data <<= 1;   
    }  
}


/*************************************************************/
/*                          ��������                         */
/*************************************************************/
void OLED_Fill(unsigned char ucData)
{
    unsigned char ucPage,ucColumn;
    
    for(ucPage = 0; ucPage < 8; ucPage++)
    {
        OLED_WrCmd(0xb0 + ucPage);  //0xb0+0~7��ʾҳ0~7
        OLED_WrCmd(0x00);           //0x00+0~16��ʾ��128�зֳ�16�����ַ��ĳ���еĵڼ���
        OLED_WrCmd(0x10);           //0x10+0~16��ʾ��128�зֳ�16�����ַ���ڵڼ���
        for(ucColumn = 0; ucColumn < 128; ucColumn++)
        {
            OLED_WrDat(ucData);
        }
    }
} 


/*************************************************************/
/*             �趨��ʾ���꣬X,Y�ֱ�Ϊ�������������         */
/*************************************************************/
void OLED_SetPos(unsigned char X, unsigned char Y)
{ 
    OLED_WrCmd(0xb0 + Y);
    OLED_WrCmd(((X & 0xf0) >> 4) | 0x10);
    OLED_WrCmd((X & 0x0f) | 0x00); 
} 

//���³�ʼ���ĺ�������OLED�����ṩ�ĳ�ʼ������
//ʹ��ʱ���Բ��������Щ�����ľ�������
//�����һЩ�߼�Ӧ�ã������OLED�����ֲ���з���
void SetAddressingMode(unsigned char ucData)
{
    OLED_WrCmd(0x20);        // Set Memory Addressing Mode
    OLED_WrCmd(ucData);      // Default => 0x02
                            // 0x00 => Horizontal Addressing Mode
                            // 0x01 => Vertical Addressing Mode
                            // 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
    OLED_WrCmd(0x21);        // Set Column Address
    OLED_WrCmd(a);           // Default => 0x00 (Column Start Address)
    OLED_WrCmd(b);           // Default => 0x7F (Column End Address)
}

void SetStartLine(unsigned char ucData)
{
    OLED_WrCmd(0x40|ucData); // Set Display Start Line
                            // Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char ucData)
{
    OLED_WrCmd(0x81);        // Set Contrast Control
    OLED_WrCmd(ucData);      // Default => 0x7F
}

void SetChargePump(unsigned char ucData)
{
    OLED_WrCmd(0x8D);        // Set Charge Pump
    OLED_WrCmd(0x10|ucData); // Default => 0x10
                            // 0x10 (0x00) => Disable Charge Pump
                            // 0x14 (0x04) => Enable Charge Pump
}

void SetSegmentRemap(unsigned char ucData)
{
    OLED_WrCmd(0xA0|ucData); // Set Segment Re-Map
                            // Default => 0xA0
                            // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                            // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SetEntireDisplay(unsigned char ucData)
{
    OLED_WrCmd(0xA4|ucData); // Set Entire Display On / Off
                            // Default => 0xA4
                            // 0xA4 (0x00) => Normal Display
                            // 0xA5 (0x01) => Entire Display On
}

void SetInverseDisplay(unsigned char ucData)
{
    OLED_WrCmd(0xA6|ucData); // Set Inverse Display On/Off
                            // Default => 0xA6
                            // 0xA6 (0x00) => Normal Display
                            // 0xA7 (0x01) => Inverse Display On
}

void SetMultiplexRatio(unsigned char ucData)
{
    OLED_WrCmd(0xA8);        // Set Multiplex Ratio
    OLED_WrCmd(ucData);      // Default => 0x3F (1/64 Duty)
}

void SetDisplayOnOff(unsigned char ucData)
{
    OLED_WrCmd(0xAE|ucData); // Set Display On/Off
                            // Default => 0xAE
                            // 0xAE (0x00) => Display Off
                            // 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char ucData)
{
    OLED_WrCmd(0xB0|ucData); // Set Page Start Address for Page Addressing Mode
                            // Default => 0xB0 (0x00)
}

void SetCommonRemap(unsigned char ucData)
{
    OLED_WrCmd(0xC0|ucData); // Set COM Output Scan Direction
                            // Default => 0xC0
                            // 0xC0 (0x00) => Scan from COM0 to 63
                            // 0xC8 (0x08) => Scan from COM63 to 0
}

void SetDisplayOffset(unsigned char ucData)
{
    OLED_WrCmd(0xD3);        // Set Display Offset
    OLED_WrCmd(ucData);      // Default => 0x00
}

void SetDisplayClock(unsigned char ucData)
{
    OLED_WrCmd(0xD5);        // Set Display Clock Divide Ratio / Oscillator Frequency
    OLED_WrCmd(ucData);      // Default => 0x80
                            // D[3:0] => Display Clock Divider
                            // D[7:4] => Oscillator Frequency
}

void SetPrechargePeriod(unsigned char ucData)
{
    OLED_WrCmd(0xD9);        // Set Pre-Charge Period
    OLED_WrCmd(ucData);      // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                            // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                            // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SetCommonConfig(unsigned char ucData)
{
    OLED_WrCmd(0xDA);        // Set COM Pins Hardware Configuration
    OLED_WrCmd(0x02|ucData); // Default => 0x12 (0x10)
                            // Alternative COM Pin Configuration
                            // Disable COM Left/Right Re-Map
}

void SetVCOMH(unsigned char ucData)
{
    OLED_WrCmd(0xDB);        // Set VCOMH Deselect Level
    OLED_WrCmd(ucData);      // Default => 0x20 (0.77*VCC)
}

void SetNop(void)
{
    OLED_WrCmd(0xE3);        // Command for No Operation
}

/*************************************************************/
/*                      ��ʼ��OLED��                         */
/*************************************************************/
void OLED_init(void)        
{
    unsigned char i;
    //����A�ڵ�14~17Ϊ���
    
    gpio_init (PTC16, GPO,0);
    gpio_init (PTC17, GPO,0);
    gpio_init (PTC18, GPO,0);
    gpio_init (PTC19, GPO,0);
    
    //gpio_init(PORT_B, 1, PIN_OUT, 1);
    //gpio_init(PORT_B, 2, PIN_OUT, 0);
    //gpio_init(PORT_B, 3, PIN_OUT, 1);
    //CLK_SET;      
    //RST_CLE;
    
    for(i=0;i<200;i++)
    {
        asm("nop");         //�ȴ�����λ  
    }
    
    RST_SET;

    SetDisplayOnOff(0x00);     // Display Off (0x00/0x01)
    SetDisplayClock(0x80);     // Set Clock as 100 Frames/Sec
    SetMultiplexRatio(0x3F);   // 1/64 Duty (0x0F~0x3F)
    SetDisplayOffset(0x00);    // Shift Mapping RAM Counter (0x00~0x3F)
    SetStartLine(0x00);        // Set Mapping RAM Display Start Line (0x00~0x3F)
    SetChargePump(0x04);       // Enable Embedded DC/DC Converter (0x00/0x04)
    SetAddressingMode(0x02);   // Set Page Addressing Mode (0x00/0x01/0x02)
    SetSegmentRemap(0x01);     // Set SEG/Column Mapping     0x00���ҷ��� 0x01����
    SetCommonRemap(0x08);      // Set COM/Row Scan Direction 0x00���·��� 0x08����
    SetCommonConfig(0x10);     // Set Sequential Configuration (0x00/0x10)
    SetContrastControl(0xCF);  // Set SEG Output Current
    SetPrechargePeriod(0xF1);  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SetVCOMH(0x40);            // Set VCOM Deselect Level
    SetEntireDisplay(0x00);    // Disable Entire Display On (0x00/0x01)
    SetInverseDisplay(0x00);   // Disable Inverse Display On (0x00/0x01)  
    SetDisplayOnOff(0x01);     // Display On (0x00/0x01)
    OLED_Fill(0x00);           // ����
    OLED_SetPos(0,0);
} 
 
/*************************************************************/
/*                     ��ʾ6X8���ַ�                         */
/*************************************************************/
void OLED_6x8Char(unsigned char X1, unsigned char Y1, unsigned char Data1)
{
    unsigned char xx, temp;     
       
    temp = Data1-32;
    if(X1 > 122)
    {
        X1 = 0;
        Y1++;
    }
    
    OLED_SetPos(X1, Y1);
    
    for(xx = 0; xx < 6; xx++)
    {     
        OLED_WrDat(F6x8[temp][xx]);  
    }
}

/*************************************************************/
/*                     ��ʾ6X8���ַ���                       */
/*************************************************************/
/*void OLED_6x8Str(unsigned char X1, unsigned char Y1, unsigned char *ucStr)
{
    unsigned char yy, temp1; 

    while(*ucStr)
    {
        temp1 = *ucStr - 32;
        if(X1 > 122)
        {
            X1 = 0;
            Y1++;
        }
        
        OLED_SetPos(X1, Y1);
        
        for(yy = 0; yy < 6; yy++)
        {     
            OLED_WrDat(F6x8[temp1][yy]);  
        }
        X1 += 6;
        ucStr++;
    }
        
}
*/
void OLED_6x8Str(unsigned char X1, unsigned char Y1, unsigned char *ucStr,unsigned char state)
{
    unsigned char yy, temp1; 

    while(*ucStr)
    {
        temp1 = *ucStr - 32;
        if(X1 > 122)
        {
            X1 = 0;
            Y1++;
        }
        
        OLED_SetPos(X1, Y1);
        if(state == 1)
        {
          for(yy = 0; yy < 6; yy++)
          {     
              //OLED_WrDat(F6x8[temp1][yy]);  
              OLED_WrDat(F6x8[temp1][yy]^0XFF);  
          }
        }
        if(state == 0)
        {  
          for(yy = 0; yy < 6; yy++)
          {     
              OLED_WrDat(F6x8[temp1][yy]);  
              //OLED_WrDat(F6x8[temp1^0XFF][yy]);  
          }
        
        }
        X1 += 6;
        ucStr++;
    }
        
}


/*************************************************************/
/*                     ��ʾ8X16���ַ���                      */
/*************************************************************/
void OLED_8x16Char(unsigned char X1, unsigned char Y1, unsigned char Data1)
{
    unsigned char yy, temp1;

    Y1 <<= 1;
    
    temp1 = Data1 - 32;
    if(X1 > 120)
    {
        X1 = 0;
        Y1 += 2;
    }
    OLED_SetPos(X1, Y1);   
    
    for(yy = 0; yy < 8; yy++) 
    {
        OLED_WrDat(F8X16[(temp1 << 4) + yy]);
    }
    
    OLED_SetPos(X1, Y1 + 1);   
    
    for(yy = 0; yy < 8; yy++) 
    {
        OLED_WrDat(F8X16[(temp1 << 4) + yy + 8]);
    }
    X1 += 8;

}

/*************************************************************/
/*                     ��ʾ16X16���ַ�                       */
/*************************************************************/
void OLED_16x16Char(unsigned char X1,unsigned char Y1,unsigned char c[2])
{
    unsigned char xx, k;

    Y1 <<= 1;
    if(X1 > 112)
    {
        X1 = 0;
        Y1 += 2;
    }
    
    for (k=0;k<64;k++) //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
    { 
        if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1]))
        { 

            OLED_SetPos(X1, Y1);
            for(xx = 0; xx < 16; xx++)               
                OLED_WrDat(codeGB_16[k].Msk[xx]);   
            
            OLED_SetPos(X1,Y1 + 1); 
            for(xx = 0;xx <16;xx++)          
                OLED_WrDat(codeGB_16[k].Msk[16+xx]);
            break;
        }  
    }	

}

/*************************************************************/
/*                         ��ʾ�ַ���                        */
/*************************************************************/
void OLED_PutString(unsigned char X1,unsigned char Y1,unsigned char *s) 
{
     while(*s) 
    {
        if( *s < 0x80) 
        {
            OLED_8x16Char(X1,Y1,*s);
            s++;X1+=8;
            if(X1 > 120)
            {
                X1 = 0;
                Y1+=1;
            }

        }
        else
        {
            OLED_16x16Char(X1,Y1,(unsigned char*)s);
            s+=2;X1+=16;
            if(X1 > 112)
            {
                X1 = 0;
                Y1+=1;
            }
        }
    }
}

/*************************************************************/
/*                       OLED��ʾͼƬ                        */
/*************************************************************/
void OLED_image(void) 
{
    unsigned int xx,yy;
    for(yy=0;yy<8;yy++)
    {
        OLED_SetPos(0,yy);
        for(xx=0;xx<128;xx++)
            OLED_WrDat(dianbiao[yy*128+xx]);
    }
} 

//**********************************************
void OLED_xs_number(unsigned char X1,unsigned char Y1,int num)
{
  uint8 ch = 0;
  unsigned char s[5];int i;
  if(num <0)
  {
    num = - num;
    ch = 1;
  }
  s[4]='\0';//s[1]='.';
  for(i=3;i>=0;i--)
  {
    s[i]=num%10+0x30;
    num=num/10;
  }
  if(ch == 1)
    s[0]='-';
  //s[0]=num%10+0x30;
  OLED_6x8Str(X1,Y1,s,0);
}



//�ض����
uint8 pic_zip[Image_H][Image_W];
void OLED_3number(char String[5],int16 num)
{ //��λ������
  if(num<0) 
  {
    String[0]='-';
    num=abs(num);
  }
  else
    String[0]='+';
  
    String[1]=0x30+num/100;
    num=num%100;
    String[2]=0x30+num/10;
    num=num%10;
    String[3]=0x30+num;
    String[4]='\0';
}
void OLED_5number(char String[7],int16 num)
{ //��λ������
  if(num<0) 
  {
    String[0]='-';
    num=abs(num);
  }
  else
    String[0]='+';
  
    String[1]=0x30+num/10000;
    num=num%10000;
    String[2]=0x30+num/1000;
    num=num%1000;
    String[3]=0x30+num/100;
    num=num%100;
    String[4]=0x30+num/10;
    num=num%10;
    String[5]=0x30+num;
    String[6]='\0';
}
void OLED_7number(char String[8],int32 num)
{ //7λ������
    if(num>0)
      String[0]='+';
    else
      String[0]='-'; 
    String[7]='\0';
    for(int i=0;i<7;i++)
    {
     String[6-i]=num%10+0x30;
     num=num/10;
    }
}
/**************��ʾ��·60x80****************/
//�Լ������
void OLED_PrintImage(uint8 picture[Image_H][Image_W])
{
 uint8 i,j,k,m,n,RowTmp;
 uint8 Data;
 /*for(i=0;i<Image_H;i+=2)
   for(j=0;j<Image_W;j+=2)
   {
     if(picture[i][j]+picture[i][j+1]+picture[i+1][j]+picture[i+1][j+1]>=Black)
       pic_zip[i/2][j/2]=1;
     else
       pic_zip[i/2][j/2]=0;
   }//ͼ��ѹ��
 */
   // ͼ����ʾ
    m = Image_H>> 3;   //����ͼƬ������8λΪһ������������
    n = Image_H % 8;    //����������ʣ�µ����� 
   
   for(i=0; i < Image_W; i++) //��������ɨ��
    {   
        for(j=0; j < m; j++) //��ɨ��        
        {
            OLED_SetPos(i,j);  
            Data = 0;
            RowTmp=j<<3;           //��ԭ��
            for(k = 0; k < 8; k++) //��i���ж���8��ɨ��
            {
                Data = Data >> 1;
                if(picture[RowTmp+k][i]== Black)
                {
                    Data = Data | 0x80;
                }  
            }
            OLED_WrDat(Data);   
        }
    }
       
    for(i = 0; i < Image_W; i++) //��ɨ��        
    { OLED_SetPos(i,j); //����ʣ�µ�����ʾ����ʼ����    
        Data = 0;
        RowTmp=j<<3;
        for(k = 0; k < n; k++) //��ʣ�µ���ɨ��
        {
            Data = Data >> 1;
            if(picture[RowTmp+k][i]==Black)
            {
                Data = Data | 0x80;
            }
            
        }
        Data = Data >> (8 - n);
        OLED_WrDat(Data);
    }    
}
/****************��ʾMid_farCount��R_Count��L_Count***********/
void Show_Data(uint8 Mid_farCount,uint8 L_Count,uint8 R_Count,int turn_error)
{ char show[5];
  OLED_3number(show,Mid_farCount);//ת��Ϊ�ַ���
  OLED_6x8Char(94,1,'F');
  OLED_6x8Str(102,1,(uint8*)show,0);//������
  OLED_3number(show,L_Count);//ת��Ϊ�ַ���
  OLED_6x8Char(94,3,'L');
  OLED_6x8Str(102,3,(uint8*)show,0);//������
  OLED_3number(show,R_Count);//ת��Ϊ�ַ���
  OLED_6x8Char(94,5,'R');
  OLED_6x8Str(102,5,(uint8*)show,0);//������
  OLED_3number(show,turn_error);//ת��Ϊ�ַ���
  OLED_6x8Char(94,7,'T');
  OLED_6x8Str(102,7,(uint8*)show,0);//������
  

}
