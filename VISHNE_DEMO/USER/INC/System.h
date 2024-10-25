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
void BLE_Setup(void);
void Add_Custom_Service(void);
void UserEvtRx(void *pData);
void transport_layer_init(void);

extern uint8_t myCustomData[2];
extern uint16_t service_handle;
extern uint16_t char_handle;

void Aymed_Logo(void);
void Aymed_Text(void);
void BatteryLevelFilterInit(void);
void TIM11_Init(void);
void TIM10_Init(void);
//void TIM9_Init(void);
void ChargerDetect_Init(void);
void DisplayChargingSymbol(bool isCharging);

void Wakeup_Init(void);
void Enter_Standby_Mode(void);

extern float adcReadings[FILTER_SIZE]; // Array to store ADC samples
extern float lastPercentage;
extern bool isCharging;




#endif /* INC_SYSTEM_H_ */
