/*
 * Barcode.c
 *
 *  Created on: Sep 12, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"
#include "usb_host.h"
#include "usbh_hid.h"
#include "stdio.h"
#include "usbh_hid_keybd.h"

char key;
char Buffered_ID[17] = {0};    //17 // 8 characters
int ID_idx = 0;
char Uart_Buf[2];
char ID[8];

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if(USBH_HID_GetDeviceType(phost) == HID_KEYBOARD){
		HID_KEYBD_Info_TypeDef *Keyboard_Info;
		Keyboard_Info = USBH_HID_GetKeybdInfo(phost);
		key = USBH_HID_GetASCIICode(Keyboard_Info);
		//int len = sprintf(Uart_Buf, "%c", key);
		//HAL_UART_Transmit(&huart2, (uint8_t *)Uart_Buf, len, 1000);
        process_id();
	}
}

void process_id(void){

		Buffered_ID[ID_idx++] = key;
		//Buffered_ID[17] = '\0'; // Add null terminator manually	}
		if(ID_idx>=17) ID_idx = 1;

		for (int i=1,j=0; i < 16; i+=2,++j) {
			ID[j] = Buffered_ID[i];
		}
}

