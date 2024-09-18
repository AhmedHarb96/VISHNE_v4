#include "main.h"

#include "string.h"
#include "stdio.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "System.h"
#include "Calculation.h"
#include "Spectrometer.h"
#include "LCD.h"
#include "RTC.h"
#include "Flash.h"
#include "Barcode.h"

extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim11;
extern DMA_HandleTypeDef hdma_tim1_ch1;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern SPI_HandleTypeDef hspi3;

extern SPI_HandleTypeDef hi2c3;
extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart3;

extern char message[10];            //Result to be sent to UART and LCD
//extern struct FlashEntry magic;

//#define BUTTON_PIN GPIO_PIN_5
//#define LED_PIN GPIO_PIN_6

#define NUM_WAVELENGTHS 256
#define main_spec_start 99
#define main_spec_end 145

#define delay_us 10
#define Filtered_Spec_Len 20  //256
/*
#define TARGET_WAVELENGTH_450 450 // Target wavelength for bilirubin in nm (blue light)
#define TARGET_WAVELENGTH_570 570 // Target wavelength for bilirubin in nm (yellow light)

#define BASELINE_ABSORPTION 0  //  sensor's response when no sample is present, essentially the "dark" or "reference" measurement
#define CALIBRATION_FACTOR 1.0f // Replace with actual calibration factor

#define WAVELENGTH_START 340  // Start of the wavelength range in nm
#define WAVELENGTH_END 780    // End of the wavelength range in nm
//#define TARGET_WAVELENGTH 450 // Target wavelength for bilirubin in nm
//#define NUM_CALIBRATION_POINTS 5
*/
//-----------------------------------------------------



