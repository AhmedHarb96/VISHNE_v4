/*
 * Calculation.h
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */

#ifndef INC_CALCULATION_H_
#define INC_CALCULATION_H_
#include "generalHeaders.h"


extern float BilResult;
extern uint32_t spectral_data[NUM_WAVELENGTHS*2];

void capture_and_send_data(void);

#endif /* INC_CALCULATION_H_ */
