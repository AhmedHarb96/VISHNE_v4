/*
 * LCD.h
 *
 *  Created on: Aug 6, 2024
 *      Author: pc
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
/*
void LCD_Reset(void);
void LCD_WriteCommand(uint8_t command);
void LCD_WriteData(uint8_t data);
void LCD_Init(void);
void LCD_SetCursor(uint8_t x, uint8_t y);
void LCD_WriteString(char *str);
*/


void LCD_Init(void);
void LCD(void);
void LCD_DisplayMenu(void);
void LCD_HandleButtonPress(void);
void LCD_UpdateMenu(void);
void MeasureAverage(void);
void CompleteTest(void);
void LCD_Reset(void);
void DoesTestComplete(void);


//extern int avgValue;  // Initial average value
//extern int currentTest;



static const uint8_t image_data_ss[1024] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x18, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x18, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0xef, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xc7, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0x83, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0x81, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x01, 0xfe, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x03, 0xfc, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x07, 0xf8, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x07, 0xf8, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0f, 0xf0, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0f, 0xf0, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x03, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x01, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0xc0, 0x00, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x01, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x03, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const uint8_t image_data_logo_text[512] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x00, 0x1c, 0x3f, 0xfe, 0x38, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x18, 0x00, 0x0f, 0xff,
    0xff, 0x80, 0x00, 0x1c, 0x3f, 0xfe, 0x38, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x18, 0x00, 0x07, 0xff,
    0xff, 0x00, 0x00, 0x1c, 0x3f, 0xfe, 0x38, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x18, 0x00, 0x03, 0xff,
    0xff, 0x00, 0x00, 0x1c, 0x3f, 0xfe, 0x38, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x18, 0x00, 0x01, 0xff,
    0xff, 0x07, 0xfe, 0x1c, 0x3f, 0xfe, 0x38, 0x7f, 0xfe, 0x0e, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0xff,
    0xff, 0x0f, 0xff, 0x1c, 0x3f, 0xfe, 0x38, 0x7f, 0xff, 0x0e, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xff, 0x0f, 0xfe, 0x1c, 0x3f, 0xfe, 0x38, 0x7e, 0x3f, 0x0e, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xff, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x38, 0x7e, 0x3f, 0x0e, 0x00, 0x00, 0xf8, 0x7f, 0xf8, 0xff,
    0xff, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x38, 0x7e, 0x3f, 0x0e, 0x00, 0x00, 0xf8, 0x7f, 0xf8, 0xff,
    0xff, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x38, 0x7e, 0x3f, 0x0e, 0x00, 0x00, 0xf8, 0x7f, 0xf8, 0xff,
    0xff, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x38, 0x7e, 0x3f, 0x0e, 0x1f, 0xff, 0xf8, 0x7f, 0xf8, 0xff,
    0xff, 0x1f, 0xff, 0x1f, 0xff, 0xfc, 0x38, 0x7e, 0x3f, 0x0e, 0x1f, 0xff, 0xf8, 0x7f, 0xf8, 0xff,
    0xfe, 0x1f, 0xff, 0x1f, 0xff, 0xfc, 0x38, 0x7e, 0x3f, 0x0f, 0x0f, 0xff, 0xf8, 0x7f, 0xf8, 0xff,
    0xfe, 0x1f, 0xff, 0x1c, 0x00, 0x00, 0x38, 0x7e, 0x3f, 0x0f, 0x00, 0x00, 0x18, 0x00, 0x00, 0xff,
    0xfe, 0x1f, 0xff, 0x1c, 0x00, 0x00, 0x78, 0x7e, 0x1f, 0x0f, 0x80, 0x00, 0x18, 0x00, 0x00, 0xff,
    0xfe, 0x1f, 0xff, 0x1c, 0x00, 0x00, 0xf8, 0x7e, 0x1f, 0x0f, 0xc0, 0x00, 0x18, 0x00, 0x00, 0xff,
    0xfe, 0x1f, 0xfe, 0x1c, 0x00, 0x03, 0xf8, 0x7e, 0x1f, 0x0f, 0xe0, 0x00, 0x18, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

#endif /* INC_LCD_H_ */
