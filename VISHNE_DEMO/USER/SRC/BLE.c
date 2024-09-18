/*
 * BLE.c
 *
 *  Created on: Sep 13, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"

#define flashIdxNo 7                               // Number of flash indeces to be read - 7 dataSet places
#define flashAddNo 7                               // Number of adresses in each index (dataSet)


float readFlashedBil[flashIdxNo*flashAddNo];  //49 //35 //28
//int BILindices[7] = {3, 7, 11, 15, 19, 23, 27};
//int BILindices[7] = {4, 9, 14, 19, 24, 29, 34};
int BILindices[7] = {4, 11, 18, 25, 32, 39, 46};
float extracted_Bil[7]; // Array to store 7 BIL results

char readFlashedID[flashIdxNo*flashAddNo*4];  //196 //140 //112
//int IDindices[7] = {4, 20, 36, 52, 68, 84, 100};  // Each ID starts 16 bytes apart
//int IDindices[7] = {8, 28, 48, 68, 88, 108, 128}; // Each ID starts 20 bytes apart
int IDindices[7] = {8, 36, 64, 92, 120, 148, 176};   // Each ID starts 28 bytes apart
uint32_t extracted_ID[7]; // Array to store 7 ID numbers

uint32_t readFlashedTIME[flashIdxNo*flashAddNo];  //49
int TIMEindices[7] = {5, 12, 19, 26, 33, 40, 47};
uint32_t extracted_TIME[7]; // Array to store 7 ID numbers

uint32_t readFlashedYEAR[flashIdxNo*flashAddNo];  //49
int YEARindices[7] = {6, 13, 20, 27, 34, 41, 48};
uint32_t extracted_YEAR[7]; // Array to store 7 ID numbers

void Send_TO_BLE(void){			//data to ESP ==> huart3

	ExtractIDandBIL();          //read and extract Bil and ID from flash

	char Bil_msg[10];  // Buffer for BIL message
	for (int i = 0; i < 7; i++) {
		snprintf(Bil_msg, sizeof(Bil_msg), "%.2f\n", extracted_Bil[i]);
		HAL_UART_Transmit(&huart3, (uint8_t *)Bil_msg, strlen(Bil_msg), HAL_MAX_DELAY);
		HAL_Delay(20);  // Small delay to allow the ESP32 to process the data
	}

	char ID_msg[10];  // Buffer for ID message
	for (int j = 0; j < 7; j++) {
		snprintf(ID_msg, sizeof(ID_msg), "%lu\n", extracted_ID[j]);
		HAL_UART_Transmit(&huart3, (uint8_t *)ID_msg, strlen(ID_msg), HAL_MAX_DELAY);
		HAL_Delay(20);  // Small delay to allow the ESP32 to process the data
	}
	char TIME_msg[12];  // Buffer for TIME message
	for (int m = 0; m < 7; m++) {
		snprintf(TIME_msg, sizeof(TIME_msg), "%lu\n", extracted_TIME[m]);
		HAL_UART_Transmit(&huart3, (uint8_t *)TIME_msg, strlen(TIME_msg), HAL_MAX_DELAY);
		HAL_Delay(20);  // Small delay to allow the ESP32 to process the data
	}
	char YEAR_msg[10];  // Buffer for YEAR message
	for (int n = 0; n < 7; n++) {
		snprintf(YEAR_msg, sizeof(YEAR_msg), "%lu\n", extracted_YEAR[n]);
		HAL_UART_Transmit(&huart3, (uint8_t *)YEAR_msg, strlen(YEAR_msg), HAL_MAX_DELAY);
		HAL_Delay(20);  // Small delay to allow the ESP32 to process the data
	}
}

/* ******************  SPLITTING TIME To: hours, minutes, date, and month *****************************
 * uint32_t extracted_TIME = 37355777; // Packed hours, minutes, date, and month

// Extract the individual components
uint8_t hours = (extracted_TIME >> 24) & 0xFF;   // Extract the most significant 8 bits (hours)
uint8_t minutes = (extracted_TIME >> 16) & 0xFF; // Extract the next 8 bits (minutes)
uint8_t date = (extracted_TIME >> 8) & 0xFF;     // Extract the next 8 bits (date)
uint8_t month = extracted_TIME & 0xFF;           // Extract the least significant 8 bits (month)

 *******************************************************************************************************
 */

void ExtractIDandBIL(void) {

	ReadBilResultsFromFlash(readFlashedBil); // to read BIL _ float
	ReadBilResultsFromFlash(readFlashedID);  // to read ID _ char
	ReadBilResultsFromFlash(readFlashedTIME);  // to read RTC _ uint8_t
	ReadBilResultsFromFlash(readFlashedYEAR);  // to read RTC _ uint8_t

	for (int i = 0; i < 7; i++) {
		uint32_t tempID = 0;
		//uint32_t temp = 0;
		// Convert the 8 consecutive characters into a number
		for (int j = 0; j < 8; j++) {
			tempID = tempID * 10 + (readFlashedID[IDindices[i] + j] - '0'); // Convert char to int
		}
		if(tempID==3761633968) tempID = 0;                 // if user didn't scan ID prior the test -> ID=0;
		extracted_ID[i] = tempID; // Store in the ID array
	}

	for (int i = 0; i < 7; i++) {
		extracted_Bil[i] = readFlashedBil[BILindices[i]];  // Copy value at the given index
	}
	for (int i = 0; i < 7; i++) {
		extracted_TIME[i] = readFlashedTIME[TIMEindices[i]];  // Copy value at the given index
	}
	for (int i = 0; i < 7; i++) {
		extracted_YEAR[i] = readFlashedYEAR[YEARindices[i]];  // Copy value at the given index
	}
}
