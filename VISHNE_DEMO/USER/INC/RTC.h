/*
 * RTC.h
 *
 *  Created on: Aug 15, 2024
 *      Author: pc
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_


uint8_t decToBcd(int val);

int bcdToDec(uint8_t val);

void RTC_Init(void);
uint8_t DS1307_is_running();

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t date, uint8_t month, uint8_t year);

void RTC_GetTime(uint8_t* hours, uint8_t* minutes, uint8_t* seconds, uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year);

void RTC_DisplayTime(void);

extern uint8_t hours, minutes, seconds, day, date, month, year;


#endif /* INC_RTC_H_ */
