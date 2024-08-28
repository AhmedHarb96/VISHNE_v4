/*
 * Spectrometer.h
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */

#ifndef INC_SPECTROMETER_H_
#define INC_SPECTROMETER_H_

#include "generalHeaders.h"

extern uint8_t testDone;
extern int StartTestMenuFlag;
extern uint8_t eos_received;

void spectrometer(void);
void generate_spectrometer_signals(void);

void DWT_Init(void);
void DWT_Delay(uint32_t us);



#endif /* INC_SPECTROMETER_H_ */
