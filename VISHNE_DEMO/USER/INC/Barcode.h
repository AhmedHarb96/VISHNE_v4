/*
 * Barcode.h
 *
 *  Created on: Sep 12, 2024
 *      Author: pc
 */

#ifndef INC_BARCODE_H_
#define INC_BARCODE_H_
#include "usb_host.h"
#include "usbh_hid.h"
#include "stdio.h"
#include "usbh_hid_keybd.h"

extern char ID[8];

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost);
void process_id(void);


#endif /* INC_BARCODE_H_ */
