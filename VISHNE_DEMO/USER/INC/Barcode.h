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

#define BcodeIndecesNO 8
extern char ID[8];
extern char Buffered_ID[(BcodeIndecesNO*2)+1];
extern int ID_idx;

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost);
void process_id(void);


#endif /* INC_BARCODE_H_ */
