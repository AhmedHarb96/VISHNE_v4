/*
 * System.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */
#include "../INC/generalHeaders.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "sensor.h"
#include "bluenrg1_events.h"

int logo_time = 1500;    //2500
int text_time = 2500;    //2000

//#define FILTER_SIZE 10          // Number of samples for the moving average
float adcReadings[FILTER_SIZE]; // Array to store ADC samples
float lastPercentage;           // Initialize last percentage to an invalid value
bool isCharging = false;

// Prepare the data you want to send (example data)
uint8_t myCustomData[2] = {14, 15}; // Example custom data
tBleStatus ret;
uint8_t buff_Temp[2];
// Global variables for BLE service and characteristic handles
uint16_t service_handle;
uint16_t char_handle;
//static uint16_t connection_handle;

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
	  BLE_Setup();
	  Wakeup_Init();      // Check if the system woke up from Standby
}



//################################################################################################//



void SpectrometerSetup(void)
{
	  TIM1->CCR1=5;  //50   50% Duty Cycle ==> to generate 135khz
	  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

	  HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SPEC_GAIN_GPIO_Port, SPEC_GAIN_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SPEC_EOS_GPIO_Port, SPEC_EOS_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SPEC_LED_GPIO_Port, SPEC_LED_Pin, GPIO_PIN_RESET); // Turn off the SPEC LED
}

void LCD_Setup(void)
{
	  HAL_GPIO_WritePin(EN_5vReg_GPIO_Port, EN_5vReg_Pin, GPIO_PIN_RESET);                   // Turn off 5v REG
	  HAL_GPIO_WritePin(SPEC_LED_GPIO_Port, SPEC_LED_Pin, GPIO_PIN_RESET); 					// Turn off the SPEC LED
	  ssd1306_Init();

	  BatteryLevelFilterInit();
	  TIM11_Init(); 			// Initialize the timer for interrupts ==> for batt level percentage calculation
	  TIM10_Init();
	  for (int var = 0; var < 5; ++var)  BatteryPercentage();

	  //ChargerDetect_Init();

	  Aymed_Logo();
	  //Aymed_Text();      //moved to LCD.c
}

void USB_Setup(void){

	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);          //to open 5 v from PA9

}

void FLASH_Setup(void){

	  //EraseAllBilFlashSectors();            // to reset flash

	  FindLastBilResultIndex();// On startup, find the last valid index in the flash memory

}
//Handle_Packets_Pair_Entry_t xy;
void BLE_Setup(void){
	// Initialization of the BLE stack
	  	 //tBleStatus ret = BLE_STATUS_SUCCESS;
	  	// Initialize the BlueNRG hardware (assuming this is done elsewhere in your setup code)
	  	 //hci_init(UserEvtRx, transport_layer_init);
	    // Initialize GATT and GAP
	     ret = aci_gatt_init();
	     ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, NULL, NULL);
	     // Add custom service and characteristic
	     Add_Custom_Service();
	     // Start the MTU exchange process to request a larger MTU size
	     //uint16_t desired_MTU_size = 64;  // Set desired MTU size (e.g., 64 bytes)
	     //aci_gatt_exchange_config(xy.Connection_Handle);
	     // Make the device discoverable (set as connectable)
	     //aci_gap_set_discoverable(ADV_IND, 0x20, 0x30, PUBLIC_ADDR, NO_WHITE_LIST_USE,0, NULL, 0, NULL, 0, 0);
}

//=============================    BLE Init FNs      ===============================
// Function to add a custom service and characteristic
void Add_Custom_Service(void){
    //tBleStatus ret;
    Service_UUID_t service_uuid;
    Char_UUID_t char_uuid;

    // Example UUIDs for the service and characteristic (16-bit UUID)
    service_uuid.Service_UUID_16 = 0x1234;   // Example service UUID
    char_uuid.Char_UUID_16 = 0x5678;         // Example characteristic UUID

    // Add custom service
    ret = aci_gatt_add_service(UUID_TYPE_16, &service_uuid, PRIMARY_SERVICE, 7, &service_handle);

    // Add custom characteristic (read/write, 20 bytes max size)
    ret = aci_gatt_add_char(service_handle, UUID_TYPE_16, &char_uuid, 20,
                            CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY,
                            ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                            16, 1, &char_handle);
}
void UserEvtRx(void *pData){
    hci_notify_asynch_evt((void*)pData);
}

// Example transport layer initialization (depends on your setup, UART, SPI, etc.)
void transport_layer_init(void){
    // Initialize the transport layer here (e.g., UART or SPI communication)
}
//====================================================================================

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
// ################################################# TIMERS Configurations ######################################## //
// Timer Interrupt Initialization
void TIM11_Init(void) {                   // Battery calculation
	HAL_TIM_IRQHandler(&htim11);
    // Enable clock for TIM2
    RCC->AHB2ENR |= RCC_APB2ENR_TIM11EN;

    // Configure TIM2: 1 tick per millisecond (assuming 16 MHz clock with APB1 prescaler 4)
    TIM11->PSC = 168000-1;    //168*5->10 sec //36000 - 1;  // Prescaler: 16 MHz / 16000 = 1 kHz (1 ms period)
    TIM11->ARR = 20000-1;    //14000 - 1;  //4000=2 sec //50000 = 5 sec
    TIM11->CR1 |= TIM_CR1_CEN;   // Enable counter

    // Enable TIM2 interrupt
    TIM11->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}
// Timer Interrupt Initialization
void TIM10_Init(void) {                     //dotCounter++; && Standby_Mode ctr
	HAL_TIM_IRQHandler(&htim10);
    // Enable clock for TIM2
    RCC->AHB2ENR |= RCC_APB2ENR_TIM10EN;

    // Configure TIM2: 1 tick per millisecond (assuming 16 MHz clock with APB1 prescaler 4)
    TIM10->PSC = 84000 - 1;      // Prescaler: 16 MHz / 16000 = 1 kHz (1 ms period)
    TIM10->ARR = 10000 - 1;      //10000 = 1 sec
    TIM10->CR1 |= TIM_CR1_CEN;   // Enable counter

    // Enable TIM2 interrupt
    TIM10->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}
// ################################################# Standby Mode ######################################## //
void Enter_Standby_Mode(void){
	HAL_GPIO_WritePin(Bcode_INIT_GPIO_Port, Bcode_INIT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BT_INIT_GPIO_Port, BT_INIT_Pin, GPIO_PIN_RESET);

    // Enable wake-up sources (RTC or external pin)
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);  // Wake-up from PA0 (WKUP pin)

    // Clear the Wake-up flag
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    // Enter Standby Mode
    HAL_PWR_EnterSTANDBYMode();
}

void Wakeup_Init(void){
    // Check if the system was reset due to Standby mode wake-up
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET){
        // Clear the Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

        // Clear Wake-up flags
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    }
}

// ################################################# INTERRUPTS ######################################## //
/* EXTI1 IRQ Handler */
void EXTI0_IRQHandler(void)                 // System Wake-UP
{
    // Handle external interrupt (button press on PB1)
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    // Clear interrupt and wake up the MCU
}

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

