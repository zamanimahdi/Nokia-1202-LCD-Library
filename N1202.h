
#ifndef N1202_H_INCLUDED
#define N1202_H_INCLUDED

#include "N1202.c"


#define lcd_x_pix 96
#define lcd_y_pix 68



uint8_t bi_to_by (uint8_t byte,uint8_t select);
void N1202_Write(uint8_t DC,uint8_t c);
void N1202_clear(void);
void N1202_Init(void);
void N1202_Contrast(uint8_t cont);
void N1202_Update(void);
void N1202_GotoXY(uint8_t x, uint8_t y);
void N1202_Draw_pixel(uint8_t x,uint8_t y,uint8_t color);
void N1202_write_y(uint8_t x,uint8_t y,uint8_t data[7]);
void  N1202_WriteChar(uint8_t x, uint8_t y,char c,uint8_t color);
void N1202_Xline_len(uint8_t x,uint8_t y,uint8_t len,uint8_t color);
void N1202_Draw_FillRect(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint8_t color);
void N1202_text(uint8_t x,uint8_t y,char * message,uint8_t color);
void N1202_show_pic (uint8_t *pic,uint8_t xpos ,uint8_t ypos,uint8_t color);
void N1202_Show_Chart(uint32_t values[10],uint8_t page,char * title);
void N1202_Del_Chart(uint32_t v[10]);

uint8_t Caghe_Read (uint8_t x, uint8_t y);
void Caghe_Write (uint8_t x, uint8_t y,uint8_t data);

#endif /* N1202_H_INCLUDED */
