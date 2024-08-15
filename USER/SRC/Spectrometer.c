/*
 * Spectrometer.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */


#include "../INC/generalHeaders.h"

volatile uint8_t eos_received = 0;

void spectrometer(void)
{
	if ((HAL_GPIO_ReadPin(GPIOA, BUTTON_Pin) == GPIO_PIN_RESET) )
	{
			generate_spectrometer_signals();
	}
}

void generate_spectrometer_signals(void)
{

	// Button is pressed
	HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET); // Turn on the LED
	HAL_Delay(1); // Wait for stabilization

	// Generate start signal for the spectrometer
	HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_RESET);
	//DWT_Delay(1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_SET);
	HAL_Delay(20); //8//500 									// Higher integration time Higher Spectrum Amplitude
	HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_SET);

	// Wait for EOS signal
	//while (!eos_received) {}
	capture_and_send_data();
	HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET); 			// Turn off the LED
	eos_received = 0;

}

/////////////////////////////////////// EOS Interrupt ///////////////////////////////////////////
/* EXTI4 IRQ Handler */
void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

// Callback for EOS interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_4) {
        eos_received = 1;
    }
}
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
