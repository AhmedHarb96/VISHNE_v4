/*
 * BLE.c
 *
 *  Created on: Sep 13, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"

void Send_TO_BLE(void){

	char Bil_message[12];
	snprintf(Bil_message, sizeof(Bil_message), "%.2f\n",BilResult);
	HAL_UART_Transmit(&huart3, (uint8_t *)Bil_message, sizeof(Bil_message), HAL_MAX_DELAY);    //BilResult to ESP

}

