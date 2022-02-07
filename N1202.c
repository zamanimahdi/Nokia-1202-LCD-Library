#include "main.h"
#include"N1202.h"
#include <math.h>

#define lcd_x_pix 96
#define lcd_y_pix 68

#define cbi(port, bit)									(port) &= ~(1 << (bit))
#define sbi(port, bit)									(port) |= (1 << (bit))
#define inv_bit(port,bit)                               (port) ^=(1 << (bit))

uint8_t N1202_cache[lcd_x_pix][9]; //....lcd_y_pix/8

uint8_t FontLookup[97][5] = { { 0x00, 0x00, 0x00, 0x00, 0x00 }, // (space)
		{ 0x00, 0x00, 0x5F, 0x00, 0x00 }, // !
		{ 0x00, 0x07, 0x00, 0x07, 0x00 }, // "
		{ 0x14, 0x7F, 0x14, 0x7F, 0x14 }, // #
		{ 0x24, 0x2A, 0x7F, 0x2A, 0x12 }, // $
		{ 0x23, 0x13, 0x08, 0x64, 0x62 }, // %
		{ 0x36, 0x49, 0x55, 0x22, 0x50 }, // &
		{ 0x00, 0x05, 0x03, 0x00, 0x00 }, // '
		{ 0x00, 0x1C, 0x22, 0x41, 0x00 }, // (
		{ 0x00, 0x41, 0x22, 0x1C, 0x00 }, // )
		{ 0x08, 0x2A, 0x1C, 0x2A, 0x08 }, // *
		{ 0x08, 0x08, 0x3E, 0x08, 0x08 }, // +
		{ 0x00, 0x50, 0x30, 0x00, 0x00 }, // ,
		{ 0x08, 0x08, 0x08, 0x08, 0x08 }, // -
		{ 0x00, 0x30, 0x30, 0x00, 0x00 }, // .
		{ 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
		{ 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
		{ 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
		{ 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
		{ 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
		{ 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
		{ 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
		{ 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
		{ 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
		{ 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
		{ 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
		{ 0x00, 0x36, 0x36, 0x00, 0x00 }, // :
		{ 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;
		{ 0x00, 0x08, 0x14, 0x22, 0x41 }, // <
		{ 0x14, 0x14, 0x14, 0x14, 0x14 }, // =
		{ 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
		{ 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?
		{ 0x32, 0x49, 0x79, 0x41, 0x3E }, // @
		{ 0x7E, 0x11, 0x11, 0x11, 0x7E }, // A
		{ 0x7F, 0x49, 0x49, 0x49, 0x36 }, // B
		{ 0x3E, 0x41, 0x41, 0x41, 0x22 }, // C
		{ 0x7F, 0x41, 0x41, 0x22, 0x1C }, // D
		{ 0x7F, 0x49, 0x49, 0x49, 0x41 }, // E
		{ 0x7F, 0x09, 0x09, 0x01, 0x01 }, // F
		{ 0x3E, 0x41, 0x41, 0x51, 0x32 }, // G
		{ 0x7F, 0x08, 0x08, 0x08, 0x7F }, // H
		{ 0x00, 0x41, 0x7F, 0x41, 0x00 }, // I
		{ 0x20, 0x40, 0x41, 0x3F, 0x01 }, // J
		{ 0x7F, 0x08, 0x14, 0x22, 0x41 }, // K
		{ 0x7F, 0x40, 0x40, 0x40, 0x40 }, // L
		{ 0x7F, 0x02, 0x04, 0x02, 0x7F }, // M
		{ 0x7F, 0x04, 0x08, 0x10, 0x7F }, // N
		{ 0x3E, 0x41, 0x41, 0x41, 0x3E }, // O
		{ 0x7F, 0x09, 0x09, 0x09, 0x06 }, // P
		{ 0x3E, 0x41, 0x51, 0x21, 0x5E }, // Q
		{ 0x7F, 0x09, 0x19, 0x29, 0x46 }, // R
		{ 0x46, 0x49, 0x49, 0x49, 0x31 }, // S
		{ 0x01, 0x01, 0x7F, 0x01, 0x01 }, // T
		{ 0x3F, 0x40, 0x40, 0x40, 0x3F }, // U
		{ 0x1F, 0x20, 0x40, 0x20, 0x1F }, // V
		{ 0x7F, 0x20, 0x18, 0x20, 0x7F }, // W
		{ 0x63, 0x14, 0x08, 0x14, 0x63 }, // X
		{ 0x03, 0x04, 0x78, 0x04, 0x03 }, // Y
		{ 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z
		{ 0x00, 0x00, 0x7F, 0x41, 0x41 }, // [
		{ 0x02, 0x04, 0x08, 0x10, 0x20 }, // "\"
		{ 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
		{ 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^
		{ 0x40, 0x40, 0x40, 0x40, 0x40 }, // _
		{ 0x00, 0x01, 0x02, 0x04, 0x00 }, // `
		{ 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
		{ 0x7F, 0x48, 0x44, 0x44, 0x38 }, // b
		{ 0x38, 0x44, 0x44, 0x44, 0x20 }, // c
		{ 0x38, 0x44, 0x44, 0x48, 0x7F }, // d
		{ 0x38, 0x54, 0x54, 0x54, 0x18 }, // e
		{ 0x08, 0x7E, 0x09, 0x01, 0x02 }, // f
		{ 0x08, 0x14, 0x54, 0x54, 0x3C }, // g
		{ 0x7F, 0x08, 0x04, 0x04, 0x78 }, // h
		{ 0x00, 0x44, 0x7D, 0x40, 0x00 }, // i
		{ 0x20, 0x40, 0x44, 0x3D, 0x00 }, // j
		{ 0x00, 0x7F, 0x10, 0x28, 0x44 }, // k
		{ 0x00, 0x41, 0x7F, 0x40, 0x00 }, // l
		{ 0x7C, 0x04, 0x18, 0x04, 0x78 }, // m
		{ 0x7C, 0x08, 0x04, 0x04, 0x78 }, // n
		{ 0x38, 0x44, 0x44, 0x44, 0x38 }, // o
		{ 0x7C, 0x14, 0x14, 0x14, 0x08 }, // p
		{ 0x08, 0x14, 0x14, 0x18, 0x7C }, // q
		{ 0x7C, 0x08, 0x04, 0x04, 0x08 }, // r
		{ 0x48, 0x54, 0x54, 0x54, 0x20 }, // s
		{ 0x04, 0x3F, 0x44, 0x40, 0x20 }, // t
		{ 0x3C, 0x40, 0x40, 0x20, 0x7C }, // u
		{ 0x1C, 0x20, 0x40, 0x20, 0x1C }, // v
		{ 0x3C, 0x40, 0x30, 0x40, 0x3C }, // w
		{ 0x44, 0x28, 0x10, 0x28, 0x44 }, // x
		{ 0x0C, 0x50, 0x50, 0x50, 0x3C }, // y
		{ 0x44, 0x64, 0x54, 0x4C, 0x44 }, // z
		{ 0x00, 0x08, 0x36, 0x41, 0x00 }, // {
		{ 0x00, 0x00, 0x7F, 0x00, 0x00 }, // |
		{ 0x00, 0x41, 0x36, 0x08, 0x00 }, // }
		{ 0x08, 0x1C, 0x2A, 0x08, 0x08 }, // <-
		{ 0x08, 0x08, 0x2A, 0x1C, 0x08 }, // ->
		};


uint8_t bi_to_by(uint8_t byte, uint8_t select) {
	if (byte >> select & 0x01)
		return 1;
	else
		return 0;
}

void N1202_Write(uint8_t DC, uint8_t c) {

	N1202_CS_GPIO_Port->BRR = N1202_CS_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, DC & 0x01);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x80);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x40);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x20);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x10);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x08);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x04);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x02);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_SCLK_GPIO_Port->BRR = N1202_SCLK_Pin;
	HAL_GPIO_WritePin(N1202_SI_GPIO_Port, N1202_SI_Pin, c & 0x01);
	N1202_SCLK_GPIO_Port->BSRR = N1202_SCLK_Pin;

	N1202_CS_GPIO_Port->BSRR = N1202_CS_Pin;

}

void N1202_GotoXY(uint8_t x, uint8_t y) {
	N1202_Write(0, (0xB0 | (y & 0x0F)));      //Y axis initialisation: 0100 rrrr
	N1202_Write(0, (0x00 | (x & 0x0F))); //X axis initialisation: 0000 xxxx  (x3 x2 x1 x0)
	N1202_Write(0, (0x10 | ((x >> 4) & 0x07))); //X axis initialisation: 0010 0xxx  (x6 x5 x4)
}

void N1202_clear(void) {
	uint8_t x, r;

	N1202_GotoXY(0, 0);
	N1202_Write(0, 0xAE); 					// disable display;

	//fill LCD and the video buffer with zero!
	for (r = 0; r < lcd_y_pix / 8 + 1; r++) {
		for (x = 0; x < lcd_x_pix; x++) {
			N1202_cache[x][r] = 0x00;
			N1202_GotoXY(x, r);
			N1202_Write(1, 0x00);
		}
	}

	N1202_Write(0, 0xAF);
}

void N1202_Init(void) {

	HAL_GPIO_WritePin(N1202_RES_GPIO_Port, N1202_RES_Pin, 0);
	HAL_Delay(10);
	HAL_GPIO_WritePin(N1202_RES_GPIO_Port, N1202_RES_Pin, 1);
	HAL_Delay(10);

	N1202_Write(0, 0x23); // write VOP register

	N1202_Write(0, 0x90);
	N1202_Write(0, 0xA4); // all on/normal display
	N1202_Write(0, 0x2F); // Power control set(charge pump on/off)
	N1202_Write(0, 0x40); // set start row address = 0
	N1202_Write(0, 0xB0); // set Y-address = 0
	N1202_Write(0, 0x10); // set X-address, upper 3 bits
	N1202_Write(0, 0x0);  // set X-address, lower 4 bits
//N1202_Write(0,0xC8); // mirror Y axis (about X axis)
//N1202_Write(0,0xa1); // Invert screen in horizontal axis
	N1202_Write(0, 0xAC); // set initial row (R0) of the display
	N1202_Write(0, 0x07);
//lcd_write(CMD,0xF9);
	N1202_Write(0, 0xAF); // display ON/OFF

	N1202_clear();

	N1202_Write(0, 0xA6);      // normal display (non inverted)
	N1202_Write(0, 0xAF);
	//N1202_Write(0, 0xA5); // all on/normal display
}

uint8_t Caghe_Read(uint8_t x, uint8_t y) {
	//return ((N1202_cache[x][y/8]>>y%8)&0x01);
	return N1202_cache[x][y / 8];
}
;

void Caghe_Write(uint8_t x, uint8_t y, uint8_t data) {

	uint8_t yte = y / 8, ybe = y % 8;

	N1202_cache[x][yte] ^= (-data ^ N1202_cache[x][yte]) & (1 << ybe);
}
;

void N1202_Contrast(uint8_t cont) {
	N1202_Write(0, 0x20);
	N1202_Write(0, 0x9f);
	N1202_Write(0, 0x27);
	N1202_Write(0, 0xE1);
}

void N1202_Draw_pixel(uint8_t x, uint8_t y, uint8_t color) {

	//if ((x > lcd_x_pix-1) || (y > lcd_y_pix-1))			return;				//exit if coordinates are not legal

	Caghe_Write(x, y, color);

	N1202_GotoXY(x, y / 8);
	N1202_Write(1, Caghe_Read(x, y));
}

void N1202_Circle(uint8_t xpos, uint8_t ypos, uint8_t radius, uint8_t step,
		uint8_t color) {
	uint8_t i;
	int x, y;

	if (step == 0)
		return;

	for (i = 0; i <= 90; i += step) {
		x = (int) ((float) radius * cos((float) i * 0.015708));
		y = (int) ((float) radius * sin((float) i * 0.015708));
		N1202_Draw_pixel((uint8_t) (xpos + x), (uint8_t) (ypos + y), color);
		N1202_Draw_pixel((uint8_t) (xpos - x), (uint8_t) (ypos + y), color);
		N1202_Draw_pixel((uint8_t) (xpos + x), (uint8_t) (ypos - y), color);
		N1202_Draw_pixel((uint8_t) (xpos - x), (uint8_t) (ypos - y), color);

	}
}

void N1202_Xline_len(uint8_t x, uint8_t y, uint8_t len, uint8_t color) {

	uint8_t x2;
	x2 = x + len;

	for (; x < x2; x++) {
		N1202_Draw_pixel(x, y, color);
	}
}

void N1202_Yline_len(uint8_t x, uint8_t y, uint8_t len, uint8_t color) {

	uint8_t y2;
	y2 = y + len;

	for (; y < y2; y++) {
		N1202_Draw_pixel(x, y, color);
	}

}

void N1202_write_y(uint8_t x, uint8_t y, uint8_t data[7]) {

	uint8_t i;

	for (i = 0; i <= 7; i++) {
		N1202_Draw_pixel(x, y + i, data[i]);
	}
}

void N1202_WriteChar(uint8_t x, uint8_t y, char c, uint8_t color) {
	uint8_t xx, yy;
	uint16_t ch = 0;

	N1202_Yline_len(x - 1, y, 8, color);
	N1202_Yline_len(x + 5, y - 1, 9, color);
	N1202_Xline_len(x - 1, y - 1, 6, color);

	if (x + 5 > lcd_x_pix || y + 7 > lcd_y_pix)
		return;
	for (xx = x; xx < x + 5; xx++) {
		if (color)
			ch = ~FontLookup[c - 31][xx - (x + 5)];
		else
			ch = FontLookup[c - 31][xx - (x + 5)];
		for (yy = y; yy < y + 8; yy++) {
			N1202_Draw_pixel(xx, yy, bi_to_by(ch, yy - y));
		}
	}
}

void N1202_text(uint8_t x, uint8_t y, char *message, uint8_t color) {

	//if ((x == 0 || y == 0)  || (x < 0 || y < 0))
	//return; //return if text position false

	while (*message) {
		N1202_WriteChar(x, y, *message++, color);
		x = x + 6;
	}

//      N1202_Write(0,0xAF);

}


void N1202_show_pic(uint8_t *pic, uint8_t xpos, uint8_t ypos, uint8_t color) {

	uint8_t data[8];
	uint16_t i, repeat = 0, pic_x, pic_y;
	uint8_t x, y;

	pic_x = pic[0];
	pic_y = pic[1];

	if ((pic_x + xpos) > lcd_x_pix)
		return;
	if ((pic_y + ypos) > lcd_y_pix)
		return;

	N1202_Write(0, 0xA4);

	for (y = ypos; y < pic_y + ypos; y = y + 8) {

		for (x = xpos; x < pic_x + xpos; x++) {

			repeat++;

			for (i = 0; i < 8; i++) {
				if (color == 0) {
					if (pic[repeat + 1] & 1 << i)
						data[i] = 1;
					else
						data[i] = 0;
				};
				if (color == 1) {
					if (pic[repeat + 1] & 1 << i)
						data[i] = 0;
					else
						data[i] = 1;
				};
			}
			N1202_write_y(x, y, data);
		}

	}
	N1202_Write(0, 0xAF);
}
