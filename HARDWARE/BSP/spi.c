#include "spi.h"
#include "delay.h"

void OLED_SPI_GPIO_Init(void) {
    RCC->APB2ENR |= OLED_SPI_CLK_PIN_RCC | OLED_SPI_DIN_PIN_RCC | OLED_SPI_RES_PIN_RCC | OLED_SPI_DC_PIN_RCC | OLED_SPI_CS_PIN_RCC;

//SPI CLK
#if OLED_SPI_CLK_PIN_ID < 8
    OLED_SPI_CLK_GPIO->CRL &= ~((u32)0xF << (4 * OLED_SPI_CLK_PIN_ID));  // 4位清0
    OLED_SPI_CLK_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_SPI_CLK_PIN_ID));   // MODE6=11(50MHz)
    OLED_SPI_CLK_GPIO->CRL &= ~((u32)0xC << (4 * OLED_SPI_CLK_PIN_ID));  // CNF6=00(推挽输出)
#else 
    OLED_SPI_CLK_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
    OLED_SPI_CLK_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
    OLED_SPI_CLK_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_SPI_CLK_PIN_ID - 8)));
#endif

//SPI DATA
#if OLED_SPI_DIN_PIN_ID < 8
    OLED_SPI_DIN_GPIO->CRL &= ~((u32)0xF << (4 * OLED_SPI_DIN_PIN_ID));
    OLED_SPI_DIN_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_SPI_DIN_PIN_ID));
    OLED_SPI_DIN_GPIO->CRL &= ~((u32)0xC << (4 * OLED_SPI_DIN_PIN_ID));
#else 
    OLED_SPI_DIN_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
    OLED_SPI_DIN_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
    OLED_SPI_DIN_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_SPI_DIN_PIN_ID - 8)));
#endif

//SPI CS
#if OLED_SPI_CS_PIN_ID < 8
    OLED_SPI_CS_GPIO->CRL &= ~((u32)0xF << (4 * OLED_SPI_CS_PIN_ID));
    OLED_SPI_CS_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_SPI_CS_PIN_ID));
    OLED_SPI_CS_GPIO->CRL &= ~((u32)0xC << (4 * OLED_SPI_CS_PIN_ID));
#else 
    OLED_SPI_CS_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_SPI_CS_PIN_ID - 8)));
    OLED_SPI_CS_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_SPI_CS_PIN_ID - 8)));
    OLED_SPI_CS_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_SPI_CS_PIN_ID - 8)));
#endif

//SPI RST
#if OLED_SPI_RES_PIN_ID < 8
    OLED_SPI_RES_GPIO->CRL &= ~((u32)0xF << (4 * OLED_SPI_RES_PIN_ID));
    OLED_SPI_RES_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_SPI_RES_PIN_ID));
    OLED_SPI_RES_GPIO->CRL &= ~((u32)0xC << (4 * OLED_SPI_RES_PIN_ID));
#else 
    OLED_SPI_RES_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_SPI_RES_PIN_ID - 8)));
    OLED_SPI_RES_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_SPI_RES_PIN_ID - 8)));
    OLED_SPI_RES_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_SPI_RES_PIN_ID - 8)));
#endif

//SPI DC
#if OLED_SPI_DC_PIN_ID < 8
    OLED_SPI_DC_GPIO->CRL &= ~((u32)0xF << (4 * OLED_SPI_DC_PIN_ID));
    OLED_SPI_DC_GPIO->CRL |=  ((u32)0x3 << (4 * OLED_SPI_DC_PIN_ID));
    OLED_SPI_DC_GPIO->CRL &= ~((u32)0xC << (4 * OLED_SPI_DC_PIN_ID));
#else
    OLED_SPI_DC_GPIO->CRH &= ~((u32)0xF << (4 * (OLED_SPI_DC_PIN_ID - 8)));
    OLED_SPI_DC_GPIO->CRH |=  ((u32)0x3 << (4 * (OLED_SPI_DC_PIN_ID - 8)));
    OLED_SPI_DC_GPIO->CRH &= ~((u32)0xC << (4 * (OLED_SPI_DC_PIN_ID - 8)));
#endif

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
    OLED_W_CMD(0x00); // 亮度变化量很小
    OLED_W_CMD(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_W_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_W_CMD(0xA6);//--set normal display
    OLED_W_CMD(0xA8);//--set multiplex ratio(1 to 64)
    OLED_W_CMD(0x3f);//--1/64 duty
    OLED_W_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_W_CMD(0x00);//-not offset
    OLED_W_CMD(0xD5);//--set display clock divide ratio/oscillator frequency
    OLED_W_CMD(0xC0);//--set divide ratio, Set Clock as 100 Frames/Sec
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

    OLED_Screen_Clear();
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
    OLED_W_CMD(x & 0x0f);
}

void OLED_Screen_Clear(void) {
    u8 x, y;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for(x=0;x<128;x++)OLED_W_DATA(0x00);
    }
}

void OLED_Screen_Fill(void) {
    u8 x, y;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for (x = 0;x < 128;x++)OLED_W_DATA(0xFF);
    }
}