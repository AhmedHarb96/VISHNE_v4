/*
 * RTC.c
 *
 *  Created on: Aug 15, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"



#define DS1307_ADDRESS 0x68  // DS1307 I2C address

// BCD conversion helper functions
uint8_t decToBcd(int val) {
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

int bcdToDec(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
}

// Function to check if the DS1307 RTC is running (Clock Halt bit)
uint8_t DS1307_is_running() {
    uint8_t seconds;
    uint8_t startAddr = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, DS1307_ADDRESS << 1, &startAddr, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c3, DS1307_ADDRESS << 1, &seconds, 1, HAL_MAX_DELAY);

    // Check if the Clock Halt (CH) bit is set (bit 7 of the seconds register)
    return (seconds & 0x80) == 0;
}
// Function to initialize the DS1307 RTC
void RTC_Init(void) {
    /*// Ensure the RTC is started by setting the CH (Clock Halt) bit to 0
    uint8_t initData[2] = {0x00, 0x00};
    HAL_I2C_Master_Transmit(&hi2c3, DS1307_ADDRESS << 1, initData, 2, HAL_MAX_DELAY);*/
	if (!DS1307_is_running()) {
		// If the RTC is not running, set the initial time (e.g., 00:00:00)
		RTC_SetTime(0, 0, 0, 1, 1, 1, 24);  // Set time to 00:00:00, date to 01/01/2021
	}
}

// Function to set the time and date on the DS1307
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t date, uint8_t month, uint8_t year) {
    uint8_t setData[8];
    setData[0] = 0x00;  // Register address to start with
    setData[1] = decToBcd(seconds & 0x7F);
    setData[2] = decToBcd(minutes);
    setData[3] = decToBcd(hours);
    setData[4] = decToBcd(day);
    setData[5] = decToBcd(date);
    setData[6] = decToBcd(month);
    setData[7] = decToBcd(year);

    HAL_I2C_Master_Transmit(&hi2c3, DS1307_ADDRESS << 1, setData, 8, HAL_MAX_DELAY);
}

// Function to read the current time and date from the DS1307
void RTC_GetTime(uint8_t* hours, uint8_t* minutes, uint8_t* seconds, uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year) {
    uint8_t readData[7];
    uint8_t startAddr = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, DS1307_ADDRESS << 1, &startAddr, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c3, DS1307_ADDRESS << 1, readData, 7, HAL_MAX_DELAY);

    *seconds = bcdToDec(readData[0] & 0x7F);  // Mask to ignore CH bit
    *minutes = bcdToDec(readData[1]);
    *hours = bcdToDec(readData[2] & 0x3F);  // 24-hour format
    *day = bcdToDec(readData[3]);
    *date = bcdToDec(readData[4]);
    *month = bcdToDec(readData[5]);
    *year = bcdToDec(readData[6]);
}

// Function to display the time and date on the OLED
void RTC_DisplayTime(void) {
	static char prevBuffer[20] = {0};
    uint8_t hours, minutes, seconds, day, date, month, year;
    char buffer[20];

    RTC_GetTime(&hours, &minutes, &seconds, &day, &date, &month, &year);

    //ssd1306_Fill(Black);

    // Format time
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    if (strcmp(buffer, prevBuffer) != 0) {
		ssd1306_SetCursor(2, 0);
		ssd1306_WriteString(buffer, Font_6x8 ,White);
		strcpy(prevBuffer, buffer);
	}
    //ssd1306_SetCursor(2, 0);
    //ssd1306_WriteString(buffer, Font_6x8 ,White);

    // Format date
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d", date, month, year);
    if (strcmp(buffer, prevBuffer) != 0) {
	   ssd1306_SetCursor(80, 0);
	   ssd1306_WriteString(buffer, Font_6x8, White);
	   strcpy(prevBuffer, buffer);
   }
    //ssd1306_SetCursor(68, 0);
    //ssd1306_WriteString(buffer,Font_6x8, White);

    //ssd1306_UpdateScreen();
}
