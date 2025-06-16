#include "oled.h"
#include "spi.h"
#include <stdlib.h>
#include "string.h"
#include "font.h"

u8 FrameBuffer[OLED_BUFFER_SIZE] = {0};

void OLED_BUFFER_Refresh(void) {
    u8 x, y;
    u16 cur = 0;
    for (y = 0;y < 8;y++)
    {
        OLED_Set_Cursor(0, y);
        for (x = 0;x < 128;x++) {
            OLED_W_DATA(FrameBuffer[cur++]);
        }
    }
}

void OLED_BUFFER_Clear(void) {
    memset(FrameBuffer, 0, OLED_BUFFER_SIZE);
}

void OLED_BUFFER_Fill(void) {
    for(u16 cur=0;cur<OLED_BUFFER_SIZE;cur++)FrameBuffer[cur]=0xFF;
}

void OLED_Draw_Point(u8 x, u8 y, OLED_MIX_MODE mix) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT_PIXEL) return;
    u8 bitmask = 0x01 << (y & 0x07);
    switch (mix) {
        case OLED_MIX_COVER:
            FrameBuffer[(y >> 3) * 128 + x] |= bitmask;
        case OLED_MIX_OR:
            FrameBuffer[(y >> 3) * 128 + x] |= bitmask;
            break;
        case OLED_MIX_XOR:
            FrameBuffer[(y >> 3) * 128 + x] ^= bitmask;
            break;
        default:
            break;
    }
}

void OLED_Draw_Line(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix) {
    int16_t  dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int16_t  dy = y1 > y0 ? y1 - y0 : y0 - y1;
    int16_t  sx = x0 < x1 ? 1 : -1;
    int16_t  sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;

    while (1) {
        OLED_Draw_Point(x0, y0, mix);

        if (x0 == x1 && y0 == y1) break;

        int16_t  e2 = err << 1;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void OLED_Draw_DashedLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 dashlen, OLED_MIX_MODE mix) {
    int16_t dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int16_t dy = y1 > y0 ? y1 - y0 : y0 - y1;
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;
    u8 count = 0, pattern_len = dashlen << 1;

    while (1) {
        if ((count % pattern_len) < dashlen)
            OLED_Draw_Point(x0, y0, mix);

        if (x0 == x1 && y0 == y1) break;

        int16_t e2 = err << 1;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }

        count++;
    }
}

void OLED_Draw_Rect(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix) {
    if (x0 > x1) { u8 tmp = x0; x0 = x1; x1 = tmp; }
    if (y0 > y1) { u8 tmp = y0; y0 = y1; y1 = tmp; }

    // y
    for (u8 x = x0; x <= x1; x++) {
        OLED_Draw_Point(x, y0, mix);
        OLED_Draw_Point(x, y1, mix);
    }
    // x
    for (u8 y = y0 + 1; y < y1; y++) {
        OLED_Draw_Point(x0, y, mix);
        OLED_Draw_Point(x1, y, mix);
    }
}

void OLED_Draw_FillRect(u8 x0, u8 y0, u8 x1, u8 y1, OLED_MIX_MODE mix) {
    if (mix == OLED_MIX_HIDE)return;
    if (x0 > x1) { u8 t = x0; x0 = x1; x1 = t; }
    if (y0 > y1) { u8 t = y0; y0 = y1; y1 = t; }
    if (x1 > OLED_WIDTH) x1 = OLED_WIDTH;
    if (y1 > OLED_HEIGHT_PIXEL) y1 = OLED_HEIGHT_PIXEL;

    u8 page_start = y0 / 8;
    u8 page_end = (y1 - 1) / 8;
    u8 mask_start = 0xFF << (y0 % 8);
    u8 mask_end = 0xFF >> (7 - ((y1 - 1) % 8));

    for (u8 x = x0; x < x1; x++) {
        for (u8 page = page_start; page <= page_end; page++) {
            u8 mask = 0xFF;
            if (page == page_start) mask &= mask_start;
            if (page == page_end)   mask &= mask_end;
            switch (mix) {
                case OLED_MIX_COVER:
                    FrameBuffer[page * OLED_WIDTH + x] = (FrameBuffer[page * OLED_WIDTH + x] & ~mask) | mask;
                    break;
                case OLED_MIX_OR:
                    FrameBuffer[page * OLED_WIDTH + x] |= mask;
                    break;
                case OLED_MIX_AND:
                    FrameBuffer[page * OLED_WIDTH + x] &= mask;
                    break;
                case OLED_MIX_XOR:
                    FrameBuffer[page * OLED_WIDTH + x] ^= mask;
                    break;
            }
        }
    }
}

void OLED_Draw_Element(ELEMENT ele) {
    
    //混合模式0x00不显示
    if (ele.mix == OLED_MIX_HIDE)return;
    //宽高为0不显示
    if (ele.w == 0 || ele.h == 0)return;
    //完全超出不显示
    if (ele.x >= OLED_WIDTH || ele.y >= OLED_HEIGHT_PIXEL || ele.x + ele.w <= 0 || ele.y + ele.h <= 0) return;
    int16_t x0 = ele.x < 0 ? 0 : ele.x;
    int16_t x1 = (ele.x + ele.w > OLED_WIDTH) ? OLED_WIDTH : (ele.x + ele.w);
    int16_t page_start;
    u8 y_offset;
    if (ele.y >= 0) {
        page_start = ele.y / 8;
        y_offset = ele.y % 8;
    } else {
        int16_t abs_y = -ele.y;
        page_start = (ele.y - 7) / 8; // 向下取整
        y_offset = (8 - (abs_y % 8)) % 8;
    }
    // 计算需要显示的页数，确保像素全部显示
    u8 page_cnt = (ele.h + y_offset + 7) / 8;
    for (int16_t col = x0; col < x1; col++) {
        int16_t data_col = col - ele.x;
        u8 prev = 0;
        for (u8 page = 0; page < page_cnt; page++) {
            u8 data = 0;
            int16_t data_idx = page * ele.w + data_col;
            if (data_col >= 0 && data_col < ele.w && data_idx < ele.w * ((ele.h + 7) / 8))
                data = ele.data[data_idx];
            u8 out;
            if (page == 0) {
                out = data << y_offset;
            } else {
                out = (data << y_offset) | (prev >> (8 - y_offset));
            }
            prev = data;
            int16_t fb_page = page_start + page;
            if (col >= 0 && col < OLED_WIDTH && fb_page >= 0 && fb_page < OLED_HEIGHT_PIXEL / 8) {
                switch (ele.mix){
                    case OLED_MIX_COVER:
                        FrameBuffer[fb_page * OLED_WIDTH + col] = out;
                        break;
                    case OLED_MIX_OR:
                        FrameBuffer[fb_page * OLED_WIDTH + col] |= out;
                        break;
                    case OLED_MIX_AND:
                        FrameBuffer[fb_page * OLED_WIDTH + col] &= out;
                        break;
                    case OLED_MIX_XOR:
                        FrameBuffer[fb_page * OLED_WIDTH + col] ^= out;
                        break;
                }
            }
        }
    }
}

void TEXT_Preprocess(TEXT *text) {
    if (!text || !text->str) return;

    for (int i = 0; text->str[i] != '\0' && i < 128; i++) {
        char c = text->str[i];
        if (c < ' ' || c > '~') {
            text->font[i] = 0;
        } else {
            text->font[i] = (u8 *)&ASCII_8X16[c - ' '][0];
        }
    }
}

void OLED_Draw_Text(TEXT text) {
    if (!text.str) return;
    if (text.mix == OLED_MIX_HIDE) return; // 混合模式为隐藏时不绘制

    int16_t x_start = text.x;
    int16_t y       = text.y;
    const char *str = text.str;

    // 计算字符串像素宽度
    uint16_t max_chars = 0;
    for (; str[max_chars] != '\0' && max_chars < 128; max_chars++);
    uint16_t text_w = max_chars * 8;

    // 如果整体在屏幕外就直接 return
    if (x_start + text_w <= 0 || x_start >= OLED_WIDTH || y + 16 <= 0 || y >= OLED_HEIGHT_PIXEL)
        return;

    int16_t x_cursor = x_start;
    int16_t page_start = (y >= 0) ? (y / 8) : ((y - 7) / 8);
    uint8_t y_offset = (y >= 0) ? (y % 8) : ((8 - ((-y) % 8)) % 8);
    uint8_t page_cnt = (16 + y_offset + 7) / 8;

    for (uint8_t i = 0; str[i] != '\0' && i < 128; i++, x_cursor += 8) {
        if (!text.font[i]) continue;
        const uint8_t *font_data = text.font[i];

        for (uint8_t col_offset = 0; col_offset < 8; col_offset++) {
            uint8_t prev = 0;

            for (uint8_t page = 0; page < page_cnt; page++) {
                uint8_t data = 0;
                if (page == 0)      data = font_data[col_offset];
                else if (page == 1) data = font_data[col_offset + 8];

                uint8_t out = (page == 0) ? (data << y_offset) : ((data << y_offset) | (prev >> (8 - y_offset)));
                prev = data;

                int16_t draw_col = x_cursor + col_offset;
                int16_t fb_page  = page_start + page;

                if ((uint16_t)draw_col < OLED_WIDTH &&
                    (uint16_t)fb_page < (OLED_HEIGHT_PIXEL / 8)) {
                    switch (text.mix) {
                    case OLED_MIX_COVER:
                        FrameBuffer[fb_page * OLED_WIDTH + draw_col] = out;
                        break;
                    case OLED_MIX_OR:
                        FrameBuffer[fb_page * OLED_WIDTH + draw_col] |= out;
                        break;
                    case OLED_MIX_AND:
                        FrameBuffer[fb_page * OLED_WIDTH + draw_col] &= out;
                        break;
                    case OLED_MIX_XOR:
                        FrameBuffer[fb_page * OLED_WIDTH + draw_col] ^= out;
                        break;
                    }
                }
            }
        }
    }
}



u32 OLED_POW(u8 m, u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_Show_Char(u8 x, u8 page, char c, u8 *RAM, u8 draw) {
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

void OLED_Show_String(u8 x, u8 page, char *str, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; str[i] != '\0'; i++)
    {
        OLED_Show_Char(x + i * 8, page, str[i], RAM, draw);
    }
}

void OLED_Show_Num(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i;
    for (i = 0; i < len; i++)                            
    {
        OLED_Show_Char(x+i*8, page, num / OLED_POW(10, len - i - 1) % 10 + '0', RAM ,draw);
    }
}

void OLED_Show_HexNum(u8 x, u8 page, u32 num, u8 len, u8 *RAM, u8 draw) {
    u8 i, SingleNumber;
    for (i = 0; i < len; i++)                            
    {
        SingleNumber = num / OLED_POW(16, len - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_Show_Char(x + i * 8, page, SingleNumber + '0', RAM ,draw);
        }
        else
        {
            OLED_Show_Char(x + i * 8, page, SingleNumber - 10 + 'A', RAM ,draw);
        }
    }
}

void OLED_Show_CN(u8 x, u8 page, u8 CC[], u8 *RAM, u8 draw) {
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
    OLED_Show_String(x,page,"??", RAM, draw);
}

void OLED_Show_CNString(u8 x, u8 page, char *String, u8 *RAM, u8 draw) {
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0'; i+=2)
    {
        c[0]=String[i];
        c[1]=String[i+1];
        OLED_Show_CN(x + i*8, page, c, RAM, draw);
    }
}

void OLED_Show_MixString(u8 x, u8 page, char *String, u8 *RAM, u8 draw)
{
    u8 i;
    u8 c[] ={0,0};
    for (i = 0; String[i] != '\0';)
    {
        if(String[i]<=127)
        {
            OLED_Show_Char(x+i*8, page, String[i], RAM, draw);
            i++;
        }
        else
        {
            c[0]=String[i];
            c[1]=String[i+1];
            OLED_Show_CN(x + i*8, page, c, RAM, draw);
            i+=2;
        }
    }
}

