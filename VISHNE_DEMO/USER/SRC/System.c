/*
 * System.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */
#include "../INC/generalHeaders.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

int logo_time = 2500;    //2500
int text_time = 2500;    //2000

//#define FILTER_SIZE 10          // Number of samples for the moving average
float adcReadings[FILTER_SIZE]; // Array to store ADC samples
float lastPercentage;           // Initialize last percentage to an invalid value

bool isCharging = false;
//float readFlashedData[28];

void systemLoop(void)
{
	  spectrometer();
	  LCD();
	  RTC_DisplayTime();
}

void systemSetup(void)
{
	  SpectrometerSetup();
	  LCD_Setup();
	  RTC_Init();
	  USB_Setup();
	  FLASH_Setup();
	 // ReadBilResultsFromFlash(readFlashedData);
}

//################################################################################################//

void SpectrometerSetup(void)
{
	  TIM1->CCR1=5;  //50   50% Duty Cycle ==> to generate 135khz
	  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

	  HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB, SPEC_CLK_Pin|SPEC_EOS_Pin|SPEC_GAIN_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, SPEC_LED_Pin, GPIO_PIN_RESET); // Turn off the SPEC LED
}

void LCD_Setup(void)
{
	  HAL_GPIO_WritePin(GPIOA, SPEC_LED_Pin, GPIO_PIN_RESET); // Turn off the SPEC LED
	  ssd1306_Init();

	  BatteryLevelFilterInit();
	  TIM11_Init(); 			// Initialize the timer for interrupts ==> for batt level percentage calculation
	  for (int var = 0; var < 5; ++var)  BatteryPercentage();

	  //ChargerDetect_Init();

	  //Aymed_Logo();
	  //Aymed_Text();
}

void USB_Setup(void){

	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);          //to open 5 v from PA9

}

void FLASH_Setup(void){

	  //EraseAllBilFlashSectors();            // to reset flash

	  FindLastBilResultIndex();// On startup, find the last valid index in the flash memory

}
//################################################################################################//

void Aymed_Logo(void)
{
	 ssd1306_Fill(Black);
	 ssd1306_SetCursor(0, 0);
     ssd1306_DrawBitmap(0,0,image_data_ss,128,64, White );
     ssd1306_UpdateScreen();
     HAL_Delay(logo_time);
}

void Aymed_Text(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0, 0);
    ssd1306_DrawBitmap(0,0,image_data_logo_text,128,32, White );          //AYMED text
    //---------------------------------------------------//
    ssd1306_SetCursor(34, 48);
    ssd1306_WriteString("VISHNE v4", Font_7x10, White);					  //Device's Version
    //---------------------------------------------------//
    ssd1306_UpdateScreen();

    HAL_Delay(text_time);
}

void BatteryLevelFilterInit(void)
{
	 lastPercentage = -1.0; // Initialize last percentage to an invalid value
	  // Initialize the filter array with initial readings
	  for (int i = 0; i < FILTER_SIZE; i++) {
		  adcReadings[i] = 0;
	  }
}

// Timer Interrupt Initialization
void TIM11_Init(void) {
	HAL_TIM_IRQHandler(&htim11);
    // Enable clock for TIM2
    RCC->AHB2ENR |= RCC_APB2ENR_TIM11EN;

    // Configure TIM2: 1 tick per millisecond (assuming 16 MHz clock with APB1 prescaler 4)
    TIM11->PSC = 36000 - 1;      // Prescaler: 16 MHz / 16000 = 1 kHz (1 ms period)
    TIM11->ARR = 14000 - 1;      //4000=2 sec // Auto-reload: 1 kHz / 30000 = 0.033 Hz (30 second period)  10000 = 5 sec
    TIM11->CR1 |= TIM_CR1_CEN;   // Enable counter

    // Enable TIM2 interrupt
    TIM11->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}

// ################################################# INTERRUPTS ######################################## //
/* EXTI4 IRQ Handler */

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

// Callback for System interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_8) {									// Callback for BattCharger interrupt
        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_SET) {
			// Charger connected (e.g., voltage detected > threshold)
        	isCharging = true;
		} else {
			// Charger disconnected (e.g., voltage detected < threshold)
			isCharging = false;
		}
    }

    if (GPIO_Pin == GPIO_PIN_4) {         							  // Callback for SPEC_EOS interrupt
        eos_received = 1;
    }
}

