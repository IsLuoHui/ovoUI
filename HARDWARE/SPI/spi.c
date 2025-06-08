#include "spi.h"
#include "stdlib.h"
#include "font.h"
#include "delay.h"

static u16 RAMCursor;

void OLED_SPI_GPIO_Init(void) {
    RCC->APB2ENR |= OLED_SPI_CLK_PIN_RCC | OLED_SPI_DIN_PIN_RCC | OLED_SPI_RES_PIN_RCC | OLED_SPI_DC_PIN_RCC | OLED_SPI_CS_PIN_RCC;

    //SPI CLK
    if (OLED_SPI_CLK_PIN_ID < 8) {
        OLED_SPI_CLK_GPIO->CRL &= ~(0xF << (4 * OLED_SPI_CLK_PIN_ID));  // 4位清0
        OLED_SPI_CLK_GPIO->CRL |= (0x3 << (4 * OLED_SPI_CLK_PIN_ID));   // MODE6=11(50MHz)
        OLED_SPI_CLK_GPIO->CRL &= ~(0xC << (4 * OLED_SPI_CLK_PIN_ID));  // CNF6=00(推挽输出)
    }
    else {
        OLED_SPI_CLK_GPIO->CRH &= ~(0xF << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
        OLED_SPI_CLK_GPIO->CRH |= (0x3 << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
        OLED_SPI_CLK_GPIO->CRH &= ~(0xC << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
    }

    //SPI DATA
    if (OLED_SPI_DIN_PIN_ID < 8) {
        OLED_SPI_DIN_GPIO->CRL &= ~(0xF << (4 * OLED_SPI_DIN_PIN_ID));
        OLED_SPI_DIN_GPIO->CRL |= (0x3 << (4 * OLED_SPI_DIN_PIN_ID));
        OLED_SPI_DIN_GPIO->CRL &= ~(0xC << (4 * OLED_SPI_DIN_PIN_ID));
    }
    else {
        OLED_SPI_DIN_GPIO->CRH &= ~(0xF << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
        OLED_SPI_DIN_GPIO->CRH |= (0x3 << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
        OLED_SPI_DIN_GPIO->CRH &= ~(0xC << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
    }

    //SPI CS
    if (OLED_SPI_CS_PIN_ID < 8) {
        OLED_SPI_CS_GPIO->CRL &= ~(0xF << (4 * OLED_SPI_CS_PIN_ID));
        OLED_SPI_CS_GPIO->CRL |= (0x3 << (4 * OLED_SPI_CS_PIN_ID));
        OLED_SPI_CS_GPIO->CRL &= ~(0xC << (4 * OLED_SPI_CS_PIN_ID));
    }
    else {
        OLED_SPI_CS_GPIO->CRH &= ~(0xF << (4 * (OLED_SPI_CS_PIN_ID - 8)));
        OLED_SPI_CS_GPIO->CRH |= (0x3 << (4 * (OLED_SPI_CS_PIN_ID - 8)));
        OLED_SPI_CS_GPIO->CRH &= ~(0xC << (4 * (OLED_SPI_CS_PIN_ID - 8)));
    }

    //SPI RST
    if (OLED_SPI_RES_PIN_ID < 8) {
        OLED_SPI_RES_GPIO->CRL &= ~(0xF << (4 * OLED_SPI_RES_PIN_ID));
        OLED_SPI_RES_GPIO->CRL |= (0x3 << (4 * OLED_SPI_RES_PIN_ID));
        OLED_SPI_RES_GPIO->CRL &= ~(0xC << (4 * OLED_SPI_RES_PIN_ID));
    }
    else {
        OLED_SPI_RES_GPIO->CRH &= ~(0xF << (4 * (OLED_SPI_RES_PIN_ID - 8)));
        OLED_SPI_RES_GPIO->CRH |= (0x3 << (4 * (OLED_SPI_RES_PIN_ID - 8)));
        OLED_SPI_RES_GPIO->CRH &= ~(0xC << (4 * (OLED_SPI_RES_PIN_ID - 8)));
    }

    //SPI DC
    if (OLED_SPI_DC_PIN_ID < 8) {
        OLED_SPI_DC_GPIO->CRL &= ~(0xF << (4 * OLED_SPI_DC_PIN_ID));
        OLED_SPI_DC_GPIO->CRL |= (0x3 << (4 * OLED_SPI_DC_PIN_ID));
        OLED_SPI_DC_GPIO->CRL &= ~(0xC << (4 * OLED_SPI_DC_PIN_ID));
    }
    else {
        OLED_SPI_DC_GPIO->CRH &= ~(0xF << (4 * (OLED_SPI_DC_PIN_ID - 8)));
        OLED_SPI_DC_GPIO->CRH |= (0x3 << (4 * (OLED_SPI_DC_PIN_ID - 8)));
        OLED_SPI_DC_GPIO->CRH &= ~(0xC << (4 * (OLED_SPI_DC_PIN_ID - 8)));
    }

    OLED_SPI_CLK_H;
    OLED_SPI_DIN_H;
    OLED_SPI_CS_H;
    OLED_SPI_RES_H;
    OLED_SPI_DC_H;
}

void OLED_Init(void) {
    delay_ms(50);
    OLED_SPI_RES_L;
    delay_ms(50);
    OLED_SPI_RES_H;

    OLED_W_CMD(0xAE);//--turn off oled panel
    OLED_W_CMD(0x00);//---set low column address
    OLED_W_CMD(0x10);//---set high column address
    OLED_W_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_W_CMD(0x81);//--set contrast control register
    OLED_W_CMD(0xCF); // Set SEG Output Current Brightness
    OLED_W_CMD(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_W_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_W_CMD(0xA6);//--set normal display
    OLED_W_CMD(0xA8);//--set multiplex ratio(1 to 64)
    OLED_W_CMD(0x3f);//--1/64 duty
    OLED_W_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_W_CMD(0x00);//-not offset
    OLED_W_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_W_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_W_CMD(0xD9);//--set pre-charge period
    OLED_W_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_W_CMD(0xDA);//--set com pins hardware configuration
    OLED_W_CMD(0x12);
    OLED_W_CMD(0xDB);//--set vcomh
    OLED_W_CMD(0x40);//Set VCOM Deselect Level
    OLED_W_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_W_CMD(0x02);//
    OLED_W_CMD(0x8D);//--set Charge Pump enable/disable
    OLED_W_CMD(0x14);//--set(0x10) disable
    OLED_W_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
    OLED_W_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
    OLED_W_CMD(0xAF);//--turn on oled panel

    OLED_Clear_Screen();
    OLED_Set_Cursor(0, 0);
}

void OLED_W_CMD(u8 cmd) {
    u8 i;
    OLED_SPI_DC_L;
    OLED_SPI_CS_L;
    for(i=0;i<8;i++)
    {
        OLED_SPI_CLK_L;
        if(cmd&0x80) OLED_SPI_DIN_H;
        else OLED_SPI_DIN_L;
        OLED_SPI_CLK_H;
        cmd<<=1;
    }
    OLED_SPI_CS_H;
    OLED_SPI_DC_H;
}

void OLED_W_DATA(u8 data) {
    u8 i;
    OLED_SPI_DC_H;
    OLED_SPI_CS_L;
    for(i=0;i<8;i++)
    {
        OLED_SPI_CLK_L;
        if(data&0x80) OLED_SPI_DIN_H;
        else OLED_SPI_DIN_L;
        OLED_SPI_CLK_H;
        data<<=1;
    }
    OLED_SPI_CS_H;
    OLED_SPI_DC_H;
}

void OLED_Display_On(void) {
    OLED_W_CMD(0X8D);  //SET DCDC命令
    OLED_W_CMD(0X14);  //DCDC ON
    OLED_W_CMD(0XAF);  //DISPLAY ON
}

void OLED_Display_Off(void) {
    OLED_W_CMD(0X8D);  //SET DCDC命令
    OLED_W_CMD(0X10);  //DCDC OFF
    OLED_W_CMD(0XAE);  //DISPLAY OFF
}

void OLED_Set_Cursor(u8 x, u8 y) {
    OLED_W_CMD(0xb0 + y);
    OLED_W_CMD(((x & 0xf0) >> 4) | 0x10);
    OLED_W_CMD((x & 0x0f) | 0x01);
}

void OLED_Clear_Screen(void) {
    u8 x, y;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for(x=0;x<128;x++)OLED_W_DATA(0x00);
    }
}

void OLED_Fill_Screen(void) {
    u8 x, y;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for (x = 0;x < 128;x++)OLED_W_DATA(0xFF);
    }
}

/**
 * RAM
 */
void OLED_Refresh_RAM(u8 *RAM) {
    u8 x, y;
    RAMCursor = 0;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for (x = 0;x < 128;x++)OLED_W_DATA(RAM[RAMCursor++]);
    }
}

void OLED_Clear_RAM(u8 *RAM) {
    for(RAMCursor=0;RAMCursor<OLED_SIZE;RAMCursor++)RAM[RAMCursor]=0x00;
}

void OLED_Fill_RAM(u8 *RAM) {
    for(RAMCursor=0;RAMCursor<OLED_SIZE;RAMCursor++)RAM[RAMCursor]=0xFF;
}

void OLED_DrawPoint(u8 x, u8 y, u8 *RAM, u8 draw) {
    if (x > OLED_WIDTH - 1 || y > OLED_HEIGHT * 8 - 1)return;
    switch(draw){
        case 0:
            RAM[y/8*128+x]&=~(0x01<<(y%8));
            break;
        case 1:
            RAM[y/8*128+x]|=0x01<<(y%8);
            break;
        case 2:
            if(RAM[y/8*128+x]>>(y%8)&0x01)RAM[y/8*128+x]&=~(0x01<<(y%8));
            else RAM[y/8*128+x]|=0x01<<(y%8);
            break;
    }
}

void OLED_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw) {
    u8 dx =abs(x1-x0), dy =abs(y1-y0);
    char sx =x0<x1?1:-1, sy =y0<y1?1:-1; 
    short err =(dx>dy?dx:-dy)/2, e2;
    while(1){
        OLED_DrawPoint(x0, y0, RAM, draw);
        if(x0==x1&&y0==y1) break;
        e2=err;
        if(e2>-dx){
            err-=dy;
            x0+=sx;
        }
        if(e2<dy){
            err+=dx;
            y0+=sy;
        }
    }
}

void OLED_DrawDashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, u8 *RAM, u8 draw) {
    u8 dx = abs(x1 - x0), dy = abs(y1 - y0);
    int8_t sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;;
    int16_t err = (dx > dy ? dx : -dy) / 2, e2;
    u8 count =0;
    
    while(1){
        //间隔与线端等长
        if(count<dashlen)OLED_DrawPoint(x0, y0, RAM, draw);
        count =(count+1)%(2*dashlen);
        //间隔画点
        //if(count%dashlen==0)OLED_DrawPoint(x0, y0, RAM, draw);
        //count++;
        
        if(x0==x1&&y0==y1) break;
        e2=err;
        if(e2>-dx){
            err-=dy;
            x0+=sx;
        }
        if(e2<dy){
            err+=dx;
            y0+=sy;
        }
    }
}

void OLED_DrawRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw) {
    OLED_DrawLine(x0, y0, x0, y1, RAM, draw);
    OLED_DrawLine(x0,y0,x1,y0,RAM,draw);
    OLED_DrawLine(x1,y1,x0,y1,RAM,draw);
    OLED_DrawLine(x1,y1,x1,y0,RAM,draw);
}

void OLED_DrawFillRect(u8 x0, u8 y0, u8 x1, u8 y1, u8 *RAM, u8 draw) {
    u8 i, j;
    if(x0>x1){i=x1;x1=x0;x0=i;}
    if(y0>y1){i=y1;y1=y0;y0=i;}
    for(i=x0;i<x1;i++)
    {
        for(j=y0;j<y1;j++)OLED_DrawPoint(i,j,RAM,draw);
    }
}

u32 OLED_POW(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_GShowChar(u8 x, u8 y, char c, u8 *RAM, u8 draw) {
    u8 i;
    switch(draw){
        case 0:
            for (i = 0; i < 8; i++)
            {
                RAM[y*128+x+i]&=~ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(y+1)*128+x+i]&=~ASCII_8X16[c - ' '][i+8];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++)
            {
                RAM[y*128+x+i]|=ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(y+1)*128+x+i]|=ASCII_8X16[c - ' '][i+8];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++)
            {
                RAM[y*128+x+i]=ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(y+1)*128+x+i]=ASCII_8X16[c - ' '][i+8];
            }
            break;
    }

}

void OLED_GShowString(u8 x, u8 y, char *str, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; str[i] != '\0'; i++)
    {
        OLED_GShowChar(x+i*8, y, str[i], RAM, draw);
    }
}

void OLED_GShowNum(u8 x, u8 y, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; i < len; i++)                            
    {
        OLED_GShowChar(x+i*8, y, num / OLED_POW(10, len - i - 1) % 10 + '0', RAM ,draw);
    }
}

void OLED_GShowHexNum(u8 x, u8 y, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i, SingleNumber;
    for (i = 0; i < len; i++)                            
    {
        SingleNumber = num / OLED_POW(16, len - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_GShowChar(x + i * 8, y, SingleNumber + '0', RAM ,draw);
        }
        else
        {
            OLED_GShowChar(x + i * 8, y, SingleNumber - 10 + 'A', RAM ,draw);
        }
    }
}

void OLED_GShowCN(u8 x, u8 y, u8 CC[], u8 *RAM, u8 draw) {
    u8 m, n;
    u8 i=0;
    u8 fontnum=0;
    while(GB2312_16X16[fontnum].Index[0]!='\0'){
        if(CC[0]==GB2312_16X16[fontnum].Index[0]&&CC[1]==GB2312_16X16[fontnum].Index[1]){
            for(n=0;n<2;n++)
            {
                for(m=0;m<16;m++)
                {
                    switch(draw){
                        case 0:
                            RAM[(y+n)*128+x+m]&=~GB2312_16X16[fontnum].Data[i++];
                            break;
                        case 1:
                            RAM[(y+n)*128+x+m]|=GB2312_16X16[fontnum].Data[i++];
                            break;
                        case 2:
                            RAM[(y+n)*128+x+m]=GB2312_16X16[fontnum].Data[i++];
                            break;
                    }
                }    
            }
            return;
        }else
        {
            fontnum++;
        }
    }
    OLED_GShowString(x,y,"??", RAM, draw);
}

void OLED_GShowCNString(u8 x, u8 y, char *String, u8 *RAM, u8 draw) {
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0'; i+=2)
    {
        c[0]=String[i];
        c[1]=String[i+1];
        OLED_GShowCN(x + i*8, y, c, RAM, draw);
    }
}

void OLED_GShowBMP(u8 x, u8 y, u8 w, u8 h, u8 BMP[], u8 *RAM, u8 draw)
{
    u32 i=0;
    u8 j,k;
    for(j=y;j<j+h;j++)
    {
        for(k=x;k<x+w;k++)
        {
            switch(draw){
                case 0:
                    RAM[j*128+k]&=~BMP[i++];
                    break;
                case 1:
                    RAM[j*128+k]|=BMP[i++];
                    break;
                case 2:
                    RAM[j*128+k]=BMP[i++];
                    break;
            }
        }
    }
}

void OLED_GShowMixString(u8 x, u8 y, char *String, u8 *RAM, u8 draw)
{
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0';)
    {
        if(String[i]<=127)
        {
            OLED_GShowChar(x+i*8, y, String[i], RAM, draw);
            i++;
        }
        else
        {
            c[0]=String[i];
            c[1]=String[i+1];
            OLED_GShowCN(x + i*8, y, c, RAM, draw);
            i+=2;
        }
    }
}


/*
void OLED_ShowChar(u8 x, u8 y, u8 chr)
{
	unsigned char c=0,i=0;	
    c=chr-' ';//得到偏移后的值			
    if(x>Max_Column-1){x=0;y=y+2;}
    if(SIZE ==16)
			{
                OLED_Set_Cursor(x,y);	
                for(i=0;i<8;i++)
                OLED_W_DATA(F8X16[c*16+i]);
                OLED_Set_Cursor(x,y+1);
                for(i=0;i<8;i++)
                OLED_W_DATA(F8X16[c*16+i+8]);
			}
			else {	
				OLED_Set_Cursor(x,y+1);
				for(i=0;i<6;i++)
				OLED_W_DATA(F6x8[c][i]);
				
			}
}
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		}
		OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
    x+=8;
    if(x>120){x=0;y+=2;}
    j++;
}
}
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
    unsigned int j=0;
	unsigned char x,y;
    
	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
        OLED_Set_Cursor(x0,y);
		for(x=x0;x<x1;x++)
		{
            OLED_W_DATA(BMP[j++]);
		}
	}
}
*/
