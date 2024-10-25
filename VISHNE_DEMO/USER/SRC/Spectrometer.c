/*
 * Spectrometer.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */


#include "../INC/generalHeaders.h"

  uint8_t eos_received = 0;  //volatile


void spectrometer(void){
	if ((HAL_GPIO_ReadPin(GPIOA, StartTest_BTN_Pin) == GPIO_PIN_RESET) ){
		UserAction_Detected();                    // Reset StandBy mode counter
		if(StartTestMenuFlag==1){
			generate_spectrometer_signals();
			Send_SPEC_UART();                                            // Send SPEC to UART           //***//
			//Send_TO_BLE();												 // Send Patient Data to ESP_BLE -> moved to LCD.c

		}else if (StartTestMenuFlag==2) {                               // if current menu != StartTest
			 HAL_GPIO_WritePin(ERR_BUZZER_GPIO_Port, ERR_BUZZER_Pin, GPIO_PIN_SET);   // ***** ERROR Buzzer ***//
			 HAL_Delay(500);
			 HAL_GPIO_WritePin(ERR_BUZZER_GPIO_Port, ERR_BUZZER_Pin, GPIO_PIN_RESET); // ***** ERROR Buzzer ***//
		}
	}
}

void generate_spectrometer_signals(void){

	// Button is pressed
	HAL_GPIO_WritePin(SPEC_LED_GPIO_Port, SPEC_LED_Pin, GPIO_PIN_SET); 				// Turn on the LED
	HAL_Delay(1); // Wait for stabilization

	// Generate start signal for the spectrometer
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_RESET);
	//DWT_Delay(1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_SET);
	HAL_Delay(20); //8//500 									// Higher integration time Higher Spectrum Amplitude
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_SET);

	// Wait for EOS signal
	//while (!eos_received) {}
	capture_and_send_data();
	HAL_GPIO_WritePin(SPEC_LED_GPIO_Port, SPEC_LED_Pin, GPIO_PIN_RESET); 			// Turn off the LED
	eos_received = 0;

}


void Send_SPEC_UART(void){
	// Format the concentration values as a string
	char message[12];            					//Spectrum to be sent to UART
	for (int j = 0; j < NUM_WAVELENGTHS*2; j++){
	  //printf("%lu\n", spectral_data_256[j]);
	  snprintf(message, sizeof(message), "%lu\n",spectral_data[j]);
	  // Send the concentration values via UART
	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sizeof(spectral_data[j]), HAL_MAX_DELAY);    //to PC
	  //len = sizeof(spectral_data[j]);
	  //HAL_UART_Transmit(&huart3, (uint8_t *)message, sizeof(spectral_data[j]), HAL_MAX_DELAY);    //to ESP
	}
	/*char Bil_message[12];
	snprintf(Bil_message, sizeof(Bil_message), "%.2f\n",BilResult);
	HAL_UART_Transmit(&huart3, (uint8_t *)Bil_message, sizeof(Bil_message), HAL_MAX_DELAY);    //BilResult to ESP
	*/
}

/////////////////////////////////////// EOS Interrupt ///////////////////////////////////////////
/* EXTI4 IRQ Handler */
void EXTI4_IRQHandler(void){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

// Callback for EOS interrupt  (in System.c)
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_4) {
        eos_received = 1;
    }
}*/
/////////////////////////////////////// us Delay Timer ///////////////////////////////////////////
// Function to initialize the DWT unit
void DWT_Init(void) {
    // Enable TRC
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Unlock DWT access
    //DWT->LAR = 0xC5ACCE55;
    // Enable the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
// Function to get the system clock frequency
uint32_t DWT_GetSysClockFreq(void) {
    return SystemCoreClock;
}

// Function to create a microsecond delay
void DWT_Delay(uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (DWT_GetSysClockFreq() / 1000000);
    while ((DWT->CYCCNT - startTick) < delayTicks);
}
