#include "oled.h"
#include "spi.h"
#include <stdlib.h>
#include "string.h"
#include "font.h"


// **全局 OLED 显示缓冲区**
u8 FrameBuffer[1024] = {0};

static u16 RAMCursor;

ELEMENT *elementPtrs[MAX_ELEMENTS] = {0}; // 存储动态创建的 ELEMENT 指针
u8 elementCount = 0; // 当前元素数量


void OLED_RAM_Refresh(u8 *RAM) {
    u8 x, y;
    RAMCursor = 0;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for (x = 0;x < 128;x++)OLED_W_DATA(RAM[RAMCursor++]);
    }
}

void OLED_RAM_Clear(u8 *RAM) {
    memset(RAM, 0, OLED_SIZE);
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

void OLED_ShowChar(u8 x, u8 page, char c, u8 *RAM, u8 draw) {
    u8 i;
    switch(draw){
        case 0:
            for (i = 0; i < 8; i++)
            {
                RAM[page*128+x+i]&=~ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(page+1)*128+x+i]&=~ASCII_8X16[c - ' '][i+8];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++)
            {
                RAM[page*128+x+i]|=ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(page+1)*128+x+i]|=ASCII_8X16[c - ' '][i+8];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++)
            {
                RAM[page*128+x+i]=ASCII_8X16[c - ' '][i];
            }
            for (i = 0; i < 8; i++)
            {
                RAM[(page+1)*128+x+i]=ASCII_8X16[c - ' '][i+8];
            }
            break;
    }

}

void OLED_ShowString(u8 x, u8 page, char *str, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; str[i] != '\0'; i++)
    {
        OLED_ShowChar(x + i * 8, page, str[i], RAM, draw);
    }
}

void OLED_ShowNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; i < len; i++)                            
    {
        OLED_ShowChar(x+i*8, page, num / OLED_POW(10, len - i - 1) % 10 + '0', RAM ,draw);
    }
}

void OLED_ShowHexNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i, SingleNumber;
    for (i = 0; i < len; i++)                            
    {
        SingleNumber = num / OLED_POW(16, len - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(x + i * 8, page, SingleNumber + '0', RAM ,draw);
        }
        else
        {
            OLED_ShowChar(x + i * 8, page, SingleNumber - 10 + 'A', RAM ,draw);
        }
    }
}

void OLED_ShowCN(u8 x, u8 page, u8 CC[], u8 *RAM, u8 draw) {
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
                            RAM[(page+n)*128+x+m]&=~GB2312_16X16[fontnum].Data[i++];
                            break;
                        case 1:
                            RAM[(page+n)*128+x+m]|=GB2312_16X16[fontnum].Data[i++];
                            break;
                        case 2:
                            RAM[(page+n)*128+x+m]=GB2312_16X16[fontnum].Data[i++];
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
    OLED_ShowString(x,page,"??", RAM, draw);
}

void OLED_ShowCNString(u8 x, u8 page, char *String, u8 *RAM, u8 draw) {
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0'; i+=2)
    {
        c[0]=String[i];
        c[1]=String[i+1];
        OLED_ShowCN(x + i*8, page, c, RAM, draw);
    }
}

void OLED_ShowBMP(u8 x, u8 page, u8 w, u8 h, u8 BMP[], u8 *RAM, u8 draw)
{
    u32 i=0;
    u8 j,k;
    for(j=page;j<j+h;j++)
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

void OLED_ShowMixString(u8 x, u8 page, char *String, u8 *RAM, u8 draw)
{
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0';)
    {
        if(String[i]<=127)
        {
            OLED_ShowChar(x+i*8, page, String[i], RAM, draw);
            i++;
        }
        else
        {
            c[0]=String[i];
            c[1]=String[i+1];
            OLED_ShowCN(x + i*8, page, c, RAM, draw);
            i+=2;
        }
    }
}

// **创建 ELEMENT（动态分配）**
ELEMENT *OLED_Create_Element(int16_t x, int16_t y, u8 w, u8 h,u8 mix, u8 *data) {
    if (elementCount >= MAX_ELEMENTS) return NULL; // 超出最大元素限制

    ELEMENT *ele = (ELEMENT *)malloc(sizeof(ELEMENT));
    if (!ele) return NULL; // 分配失败

    ele->x = x;
    ele->y = y;
    ele->w = (x + w > WIDTH) ? WIDTH - x : w;
    ele->h = (y + h > HEIGHT) ? HEIGHT - y : h;
    ele->mix = mix;
    ele->data = data;

    elementPtrs[elementCount++] = ele; // 存入指针数组
    return ele;
}

// **修改 ELEMENT**
void modifyElement(ELEMENT *ele, int16_t x, int16_t y, u8 w, u8 h, u8 *data, u8 mix) {
    if (!ele) return;
    ele->x = x;
    ele->y = y;
    //ele->w = (x + w > WIDTH) ? WIDTH - x : w;
    //ele->h = (y + h > HEIGHT) ? HEIGHT - y : h;
    //ele->data = data;
    //ele->mix = mix;
}

// **删除 ELEMENT**
void removeElement(u8 index) {
    if (index >= elementCount || !elementPtrs[index]) return;
    free(elementPtrs[index]); // 释放内存
    for (u8 i = index; i < elementCount - 1; i++) {
        elementPtrs[i] = elementPtrs[i + 1]; // 移动指针
    }
    elementPtrs[elementCount - 1] = NULL;
    elementCount--;
}

void OLED_Mix_Print() {
    memset(FrameBuffer, 0, 1024);
    for (u8 n = 0; n < elementCount; n++) {
        ELEMENT *e = elementPtrs[n];
        if (!e) continue;

        // 边界保护
        if (e->x >= WIDTH || e->y >= HEIGHT || e->x + e->w <= 0 || e->y + e->h <= 0) continue;

        int16_t x0 = e->x < 0 ? 0 : e->x;
        int16_t x1 = (e->x + e->w > WIDTH) ? WIDTH : (e->x + e->w);

        int16_t page_start;
        u8 y_offset;
        if (e->y >= 0) {
            page_start = e->y / 8;
            y_offset = e->y % 8;
        } else {
            int16_t abs_y = -e->y;
            page_start = (e->y - 7) / 8; // 向下取整
            y_offset = (8 - (abs_y % 8)) % 8;
        }

        // 计算需要显示的页数，确保像素全部显示
        u8 page_cnt = (e->h + y_offset + 7) / 8;

        for (int16_t col = x0; col < x1; col++) {
            int16_t data_col = col - e->x;
            u8 prev = 0;
            for (u8 page = 0; page < page_cnt; page++) {
                u8 data = 0;
                int16_t data_idx = page * e->w + data_col;
                if (data_col >= 0 && data_col < e->w && data_idx < e->w * ((e->h + 7) / 8))
                    data = e->data[data_idx];

                u8 out;
                if (page == 0) {
                    out = data << y_offset;
                } else {
                    out = (data << y_offset) | (prev >> (8 - y_offset));
                }
                prev = data;

                int16_t fb_page = page_start + page;
                if (col >= 0 && col < WIDTH && fb_page >= 0 && fb_page < HEIGHT / 8)
                    FrameBuffer[fb_page * WIDTH + col] = out;
            }
        }

        int16_t px = e->x;
        int16_t py = e->y;
        if (px < 0) {
            px = -px;
            OLED_ShowChar(0, 6, '-', FrameBuffer, 1);
        }
        if (py < 0) {
            py = -py;
            OLED_ShowChar(0, 4, '-', FrameBuffer, 1);
        }
        OLED_ShowNum(8, 4, py, 3, FrameBuffer, 1);
        OLED_ShowNum(8, 6, px, 3, FrameBuffer, 1);
    }
    OLED_RAM_Refresh(FrameBuffer);
}