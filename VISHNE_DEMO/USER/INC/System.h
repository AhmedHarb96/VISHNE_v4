/*
 * System.h
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "generalHeaders.h"
#define FILTER_SIZE 5   //10// Number of samples for the moving average

void systemLoop(void);
void systemSetup(void);
void SpectrometerSetup(void);
void LCD_Setup(void);
void USB_Setup(void);
void FLASH_Setup(void);

void Aymed_Logo(void);
void Aymed_Text(void);
void BatteryLevelFilterInit(void);
void TIM11_Init(void);
void ChargerDetect_Init(void);
void DisplayChargingSymbol(bool isCharging);


extern float adcReadings[FILTER_SIZE]; // Array to store ADC samples
extern float lastPercentage;
extern bool isCharging;




#endif /* INC_SYSTEM_H_ */
