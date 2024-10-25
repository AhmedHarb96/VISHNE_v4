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

	//aci_gatt_update_char_value(service_handle, char_handle, 0, 1,&myCustomData[1]);

	ExtractDATA();          //Extract and read Bil,ID, TIME and YEAR from flash

	char chunk[200];  		// Buffer for Chunks to be sent	//			!!! INCREASE MTU  *****//
							// Min Buffer Size to send All data ==> (5+9+9+3)*(7) = 182
	memset(chunk, 0, sizeof(chunk));  // Clear the buffer
	//memset(Bil_msg2, 0, sizeof(Bil_msg2));
	for (int i = 0; i < 7; i++) {
		char Bil_msg[10];
		snprintf(Bil_msg, sizeof(Bil_msg), "\n%.2f", extracted_Bil[i]);
		strcat(chunk, Bil_msg);  // Append each value to the main message buffer
	}
	for (int i = 0; i < 7; i++) {
		char ID_msg[15];
		snprintf(ID_msg, sizeof(ID_msg), "\n%lu", extracted_ID[i]);
		strcat(chunk, ID_msg);  // Append each value to the main message buffer
	}
	for (int i = 0; i < 7; i++) {
		char TIME_msg[15];
		snprintf(TIME_msg, sizeof(TIME_msg), "\n%lu", extracted_TIME[i]);
		strcat(chunk, TIME_msg);  // Append each value to the main message buffer
	}
	for (int i = 0; i < 7; i++) {
		char YEAR_msg[5];
		snprintf(YEAR_msg, sizeof(YEAR_msg), "\n%lu", extracted_YEAR[i]);
		strcat(chunk, YEAR_msg);  // Append each value to the main message buffer
	}
	// Send the data in chunks if it exceeds MTU size (20 bytes)
	int data_len = strlen(chunk);
	int offset = 0;
	while (data_len > 0) {
		int chunk_size =(data_len > 20) ? 20 : data_len;  // Max chunk size is 20 bytes
		aci_gatt_update_char_value(service_handle, char_handle, 0, chunk_size, (uint8_t*)(chunk + offset)); //offset
		offset += chunk_size;
		data_len -= chunk_size;
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

void ExtractDATA(void) {

	ReadBilResultsFromFlash(readFlashedBil); // to read BIL _ float
	ReadBilResultsFromFlash(readFlashedID);  // to read ID _ char
	ReadBilResultsFromFlash(readFlashedTIME);  // to read RTC _ uint8_t
	ReadBilResultsFromFlash(readFlashedYEAR);  // to read RTC _ uint8_t

	for (int i = 0; i < 7; i++) {
		uint32_t tempID = 0;
		// Convert the 8 consecutive characters into a number
		for (int j = 0; j < 8; j++) {
			tempID = tempID * 10 + (readFlashedID[IDindices[i] + j] - '0'); // Convert char to int
		}
		if(tempID==3761633968) tempID = 0;                 // if user didn't scan ID prior the test -> ID=0;
		extracted_ID[i] = tempID; 						   // Store in the ID array
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
