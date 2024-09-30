/*
 * LCD.c
 *
 *  Created on: Aug 6, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

// Menu states
typedef enum {
	MENU_START_DEV,
	MENU_READ_ID,
    MENU_SET_AVG,
    MENU_START_TEST,
    MENU_SHOW_RESULT,
	MENU_SEND_BLE,
	MENU_EDIT_RTC,
	MENU_TOTAL
} MenuState;

// Cursor positions within a menu
typedef enum {
    CURSOR_ON_MENU,
    CURSOR_ON_VALUE,
    CURSOR_TOTAL
} CursorPosition;

// Define 16x8 battery icons for different levels (horizontal)
const uint8_t battery_empty[] = {
    0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t battery_low[] = {
    0x0E, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F,
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t battery_half[] = {
    0x0E, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t battery_full[] = {
    0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Example of drawing a simple charging symbol bitmap
//const uint8_t charging_symbol[] = {
//    0x04, 0x0E, 0x1F, 0x04, 0x1F, 0x0E, 0x04, 0x00,
//};

// Define an 8x8 lightning bolt (charging) symbol
const uint8_t charging_symbol[] = {
	0x08,0x1C,0x3E,0x0E,  0x1C,  0x3E,  0x08, 0x00
};

// Variables to keep track of current state
static MenuState currentMenu = MENU_START_DEV;   //MENU_READ_ID;    //MENU_SET_AVG;
static CursorPosition currentCursor = CURSOR_ON_MENU;
static int avgValue = 1;  // Initial average value
static int currentTest = 1;
static uint8_t holdNavigateBtn = 0;
int cnt=1;
float BilArray[12] = {0};
float AveragedBil = 0;
float SumBil = 0;
int Debounce_Delay = 150;
int Tests_Intratime = 500;

int menu_line_X = 35;   //35
int menu_line_Y = 22;   //22

int set_line_X = 40;
int set_line_Y = 38;//44;   //30

int StartTestMenuFlag = 0;
int TimeSetDone=0;

 uint8_t hours = 0, minutes = 0, seconds = 0, date = 1, day = 1, month = 1, year = 24; //static
char buffer[20];

uint16_t adcValue1=0;
float percentage=0.0;
float batteryVoltage=0;
int dotCounter = 0;  // To cycle the dots
int sendBLEFlag=0;

volatile uint32_t inactivityCounter = 0;  // Inactivity timer
int TimeToStandBy = 120;   // StandBy within x sec
// Define constants for the moving average filter
//#define FILTER_SIZE 10   				 // Number of samples for the moving average
//float adcReadings[FILTER_SIZE]; 		 // Array to store ADC samples

#define UPDATE_THRESHOLD 10.0 			//5.0 //1.0 // Minimum percentage change to update display

uint8_t filterIndex = 0;       			 // Current index in the filter array
float filteredVoltage = 0.0;    		 // Filtered voltage value
int displayPercFlag = 0;

void LCD(void)
{
	LCD_DisplayMenu();
    LCD_HandleButtonPress();
}


// Function to display the current menu
void LCD_DisplayMenu(void) {
    ssd1306_Fill(Black);
    HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_RESET); // Turn off Indication LED
    DisplayPercentage();
    RTC_DisplayTime();
    TimeSetDone=0;
    //strncpy(entry.patientID, ID, sizeof(entry.patientID));  // Copy the ID
    HAL_GPIO_WritePin(GPIOD, Bcode_INIT_Pin, GPIO_PIN_RESET); 				// Turn off BT
    HAL_GPIO_WritePin(GPIOD, BT_INIT_Pin, GPIO_PIN_RESET); 				    // Turn off BT

    switch (currentMenu)
    {
    	case MENU_START_DEV:
			ssd1306_SetCursor(0, 0);
			ssd1306_DrawBitmap(0,0,image_data_logo_text,128,32, White );          //AYMED text
			//---------------------------------------------------//
			ssd1306_SetCursor(34, 38);  //48
			ssd1306_WriteString("VISHNE v4", Font_7x10, White);					  //Device's Version
			//---------------------------------------------------//
			ssd1306_SetCursor(5, 55);  //48
			ssd1306_WriteString("EnterID", Font_6x8, White);		         // Prev btn (left) -> with ID
			ssd1306_SetCursor(90, 55);  //48
			ssd1306_WriteString("SkipID", Font_6x8, White);					 // Next btn (right)-> without ID

    		break;
      // New menu for scanning patient ID
		case MENU_READ_ID:
			HAL_GPIO_WritePin(GPIOD, Bcode_INIT_Pin, GPIO_PIN_SET); 				// Turn on BT
			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)
			//ssd1306_SetCursor(10, 0);
			//ssd1306_WriteString("Scan ID", Font_7x10, White);
			ssd1306_SetCursor(10, 15);
			ssd1306_WriteString("Patient ID:", Font_7x10, White);
			ssd1306_SetCursor(10, 35);
			ssd1306_WriteString(ID, Font_11x18, White);  //Font_11x18 // Display scanned ID

			if (strlen(ID) == 0) {
				ssd1306_SetCursor(10, 35);
				//ssd1306_WriteString("Waiting for ID", Font_7x10, White); // Display message if no ID scanned

				// Generate moving dots based on the dotCounter
				switch (dotCounter % 3) {
					case 0:
						ssd1306_WriteString("Wait for ID .", Font_7x10, White);  // One dot
						break;
					case 1:
						ssd1306_WriteString("Wait for ID ..", Font_7x10, White);  // Two dots
						break;
					case 2:
						ssd1306_WriteString("Wait for ID ...", Font_7x10, White);  // Three dots
						break;
				}
				// Increment dotCounter to cycle through dots
				//dotCounter++;   // TIM10 used
				if (dotCounter >= 60) {  // Reset counter after 60 refresh cycles (arbitrary limit)
					dotCounter = 0;
				}
			}
			break;

        case MENU_SET_AVG:
			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)
        	//ssd1306_FillRectangle(126, 0, 128, 128, Black);
        	DisplayPercentage();
        	RTC_DisplayTime();
        	testDone=0;                       // Do not read any result here
        	StartTestMenuFlag = 2;            // Not in StartMenu ==> Don't init Spectrometer

        	if (currentCursor == CURSOR_ON_MENU) {
				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 35, White);
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Set AVG", Font_7x10, Black);
        	}else{
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Set AVG", Font_7x10, White);
        	}


            if (currentCursor == CURSOR_ON_VALUE) {
            	ssd1306_FillRectangle(set_line_X, set_line_Y, 72, 52, White);
            	ssd1306_SetCursor(set_line_X , set_line_Y);
            	 char avgStr[10];
				 snprintf(avgStr, sizeof(avgStr), "-%d-", avgValue);
				 ssd1306_WriteString(avgStr, Font_11x18, Black);

            }else{
            	ssd1306_SetCursor(set_line_X , set_line_Y);
            	char avgStr[10];
				snprintf(avgStr, sizeof(avgStr), "-%d-", avgValue);
				ssd1306_WriteString(avgStr, Font_11x18, White);
            }

            break;

        case MENU_START_TEST:
			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)
        	DisplayPercentage();
        	RTC_DisplayTime();
        	StartTestMenuFlag = 1;						//  in StartMenu

            if (currentCursor == CURSOR_ON_MENU){
            	ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 100, 35, White);
            	ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("StartTest", Font_7x10, Black);
			}

            ssd1306_SetCursor(set_line_X - 10, set_line_Y);
            char testStr[20];
            snprintf(testStr, sizeof(testStr), "%d of %d", currentTest, avgValue);
            ssd1306_WriteString(testStr, Font_11x18, White);

            DoesTestComplete();            			    // if test btn pressed ==> ++currentTest

            if (currentTest > avgValue){
            	currentTest = 1;
            	//*********Bil_AVG and FlashWrite***********//
            	MeasureAverage(); //HAL_Delay(10);
            	SaveBilResultToFlash();
            	//************RESET************************//
				for (int i = 0; i <= 12-1; i++) BilArray[i]=0;
				SumBil = 0;
				//AveragedBil = 0;                 // Flashing done but Displaying on LCD -> can not reset
				BilResult=0;
				memset(ID, 0, sizeof(ID));         //Bcode reset
				memset(Buffered_ID, 0, sizeof(ID));
				ID_idx=0;
            	//***********************//
                currentMenu = MENU_SHOW_RESULT;
                LCD_DisplayMenu();
            } else {                                   // Test not performed yet
                // Turn on the indication LED (READY)
                HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_SET); 			// IND LED
            }
            break;

        case MENU_SHOW_RESULT:
			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)
        	DisplayPercentage();
        	RTC_DisplayTime();
        	StartTestMenuFlag = 2;						// Not in StartMenu ==> Don't init Spectrometer

            if (currentCursor == CURSOR_ON_MENU){
				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 35, White);
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString(" Result", Font_7x10, Black);
			}

            ssd1306_SetCursor(set_line_X - 25, set_line_Y);
            char bilStr[20];
            snprintf(bilStr, sizeof(bilStr), "%.2f mg/dL", AveragedBil);
            ssd1306_WriteString(bilStr, Font_11x18, White);
            break;

        case MENU_EDIT_RTC:
			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)

        	DisplayPercentage();
        	RTC_DisplayTime();
        	StartTestMenuFlag = 2;						// Not in StartMenu ==> Don't init Spectrometer
        	if (currentCursor == CURSOR_ON_MENU){

				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 34, White);

				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Edit RTC", Font_7x10, Black);
				//*****************************************//
			    ssd1306_SetCursor(menu_line_X, menu_line_Y+16);
			    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
			    ssd1306_WriteString(buffer, Font_7x10, White);

			    ssd1306_SetCursor(menu_line_X, menu_line_Y+32);
			    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d", day, month, year);
			    ssd1306_WriteString(buffer, Font_7x10, White);
				//*****************************************//
			}else{
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Edit RTC", Font_7x10, White);
        	}

        	if (currentCursor == CURSOR_ON_VALUE){
        		EditRTC();
        		currentCursor = CURSOR_ON_MENU;
        	}

			break;

        case MENU_SEND_BLE:
        	HAL_GPIO_WritePin(GPIOD, BT_INIT_Pin, GPIO_PIN_SET); 				// Turn on BT
        	AveragedBil=0;//to Erase the old test from LCD while applying new one

			ssd1306_SetCursor(5, 56);
			ssd1306_WriteString("PREV", Font_6x8, White);		    // Prev btn (left)
			ssd1306_SetCursor(102, 56);
			ssd1306_WriteString("NEXT", Font_6x8, White);			// Next btn (right)
            DisplayPercentage();
            RTC_DisplayTime();
            StartTestMenuFlag = 2;  // Not in StartMenu ==> Don't init Spectrometer

            if (currentCursor == CURSOR_ON_MENU) {
                ssd1306_FillRectangle(menu_line_X-10, menu_line_Y, 100, 40, White);
                ssd1306_SetCursor(menu_line_X-5, menu_line_Y+5);
                ssd1306_WriteString("Send to BT", Font_7x10, Black);
            }

            // Indicate that the user can press the NAVIGATE button to send data
            ssd1306_SetCursor(set_line_X - 5, menu_line_Y + 23);
            ssd1306_WriteString("Press OK", Font_7x10, White);

            // After NAVIGATE is pressed, send data via BLE
            if (sendBLEFlag) {
            	ssd1306_FillRectangle(menu_line_X-5, menu_line_Y, 105, 40, Black);
                ssd1306_SetCursor(menu_line_X - 5, menu_line_Y + 30);
                ssd1306_WriteString("Sending...", Font_7x10, White);
                Send_TO_BLE();  // Call the function that sends data over BLE
                sendBLEFlag = 0;  // Reset the flag after sending
            }

            break;
    }

    ssd1306_UpdateScreen();
}

// Function to handle button presses
void LCD_HandleButtonPress(void) {
    // Assume button GPIOs are connected and configured
    if ((HAL_GPIO_ReadPin(GPIOA, NAVIGATE_BTN_Pin) == GPIO_PIN_RESET)) { // Navigate Button
    	UserAction_Detected();
    	RTC_DisplayTime();
        holdNavigateBtn++;
        HAL_Delay(Debounce_Delay); // Debounce delay

        if (holdNavigateBtn >= 12) { // Assuming hold for 2000 ms
            LCD_Reset();
        }
        else {
        	 if (currentMenu == MENU_SET_AVG || currentMenu == MENU_EDIT_RTC)
        	 {
				// Move cursor within the menu
				currentCursor = (currentCursor + 1) % CURSOR_TOTAL;
				if(currentCursor>=2)  currentCursor = (currentCursor - 1) % CURSOR_TOTAL;
				LCD_UpdateMenu();
        	 }
        	 /*if (currentMenu == MENU_EDIT_RTC)
			 {
				// Move cursor within the menu
				currentCursor = (currentCursor + 1) % CURSOR_TOTAL;
				if(currentCursor>=2)  currentCursor = (currentCursor - 1) % CURSOR_TOTAL;
				LCD_UpdateMenu();
			 }*/
        	 else if (currentMenu == MENU_SEND_BLE) {
				 // When in MENU_SEND_BLE, pressing NAVIGATE triggers BLE data sending
				 sendBLEFlag = 1;  // Set the flag to send data
				 LCD_UpdateMenu();  // Update display to show sending status
			 }
        }

    } else if (HAL_GPIO_ReadPin(GPIOE, NEXT_BTN_Pin) == GPIO_PIN_RESET) { // Next Button
    	UserAction_Detected();
    	RTC_DisplayTime();
        HAL_Delay(Debounce_Delay); // Debounce delay


		if (currentCursor == CURSOR_ON_MENU ) {
			//HAL_Delay(50);
			if (currentMenu == MENU_START_DEV) currentMenu = MENU_SET_AVG;
			else currentMenu = (currentMenu + 1) % MENU_TOTAL;
			LCD_UpdateMenu();

		} else if (currentCursor == CURSOR_ON_VALUE) {
			if (currentMenu == MENU_SET_AVG) {
				avgValue++;
				if (avgValue > 9) avgValue = 9; // Max AVG value
				LCD_UpdateMenu();
			}
		}

    } else if (HAL_GPIO_ReadPin(GPIOE, PREV_BTN_Pin) == GPIO_PIN_RESET) { // Prev Button
    	UserAction_Detected();
    	RTC_DisplayTime();
        HAL_Delay(Debounce_Delay); // Debounce delay

		if (currentMenu == MENU_SHOW_RESULT) {SumBil = 0;AveragedBil = 0;BilResult=0;testDone=0;}
		if (currentCursor == CURSOR_ON_MENU ) {
			if (currentMenu == MENU_START_DEV) currentMenu = MENU_READ_ID;
			else currentMenu = (currentMenu - 1 + MENU_TOTAL) % MENU_TOTAL;
			LCD_UpdateMenu();

		} else if (currentCursor == CURSOR_ON_VALUE) {
			if (currentMenu == MENU_SET_AVG) {
				avgValue--;
				if (avgValue < 1) avgValue = 1; // Min AVG value
				LCD_UpdateMenu();
			}
		}

    } else {
        holdNavigateBtn = 0; // Reset hold counter if no button is pressed
    }
}

//##############################################################################################################

void HighlightNumber(int x, int y, const char* format, int value) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), format, value);
    ssd1306_SetCursor(x, y);
    ssd1306_FillRectangle(x, y, x+8 , y+8 , White);
    ssd1306_WriteString(buffer, Font_7x10, Black); // Write black text on white background
}

void EditRTC() {
    // Variables to store user input for the RTC
    static int editIndex = 0; // Index to navigate through time and date components

	while(editIndex<6 && !TimeSetDone)  // && !TimeSetDone
	{
		    // Display and highlight the component being edited
		    ssd1306_SetCursor(menu_line_X, menu_line_Y+16);
		    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
		    ssd1306_WriteString(buffer, Font_7x10, White);

		    ssd1306_SetCursor(menu_line_X, menu_line_Y+32);
		    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d", day, month, year);
		    ssd1306_WriteString(buffer, Font_7x10, White);


		// Highlight the specific component being edited
		    switch (editIndex) {
		        case 0:
		            HighlightNumber(menu_line_X, menu_line_Y+16, "%02d", hours);
		            break;
		        case 1:
		            HighlightNumber(menu_line_X+22, menu_line_Y+16, "%02d", minutes);
		            break;
		        case 2:
		            HighlightNumber(menu_line_X+44-2, menu_line_Y+16, "%02d", seconds);
		            break;
		        case 3:
		            HighlightNumber(menu_line_X, menu_line_Y+32, "%02d", day);
		            break;
		        case 4:
		            HighlightNumber(menu_line_X+22, menu_line_Y+32, "%02d", month);
		            break;
		        case 5:
		            HighlightNumber(menu_line_X+44-2, menu_line_Y+32, "%02d", year);
		            break;
		    }

		// Handle button presses for editing
		if (HAL_GPIO_ReadPin(GPIOE, NEXT_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			switch (editIndex) {
				case 0: hours = (hours + 1) % 24; break;
				case 1: minutes = (minutes + 1) % 60; break;
				case 2: seconds = (seconds + 1) % 60; break;
				case 3: day = (day % 31) + 1; break;
				case 4: month = (month % 12) + 1; break;
				case 5: year = (year + 1) % 100; break;
			}
		} else if (HAL_GPIO_ReadPin(GPIOE, PREV_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			switch (editIndex) {
				case 0: hours = (hours == 0) ? 23 : hours - 1; break;
				case 1: minutes = (minutes == 0) ? 59 : minutes - 1; break;
				case 2: seconds = (seconds == 0) ? 59 : seconds - 1; break;
				case 3: day = (day == 1) ? 31 : day - 1; break;
				case 4: month = (month == 1) ? 12 : month - 1; break;
				case 5: year = (year == 0) ? 99 : year - 1; break;
			}
		} else if (HAL_GPIO_ReadPin(GPIOA, NAVIGATE_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			editIndex = (editIndex + 1) % 6;
			holdNavigateBtn++;
			ssd1306_UpdateScreen();


			if (editIndex == 0) { // Completed editing all components
				RTC_SetTime(hours, minutes, seconds, date, day, month, year);

				HAL_Delay(10);
				ssd1306_Fill(Black);
				ssd1306_SetCursor(menu_line_X+5, menu_line_Y+5);
				ssd1306_WriteString("DONE", Font_11x18, White);
				ssd1306_UpdateScreen();
				HAL_Delay(2000);

				TimeSetDone=1;
			}
		}

		ssd1306_UpdateScreen();
	}  //while

    currentMenu = MENU_SET_AVG;
	LCD();
}


// Function to calculate battery percentage
void BatteryPercentage(void) {                  //NOTE: With TMR: Calculate Percenatge Every 5 sec & Filter Window = 5 & UPDATE_THRESHOLD = 5

	/*HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adcValue1 = HAL_ADC_GetValue(&hadc1);*/

    HAL_ADC_Start_DMA(&hadc1, &adcValue1, 1);

	batteryVoltage = (adcValue1 / 4575.0) * 3.3; //4095.0 // Convert ADC value to voltage
	batteryVoltage *= 1.52;    //1.48;  //1.53 = 240927    						//1.67	//2	 //  = (R1 + R2) / R2 = 2

	// Update the filter array with the new reading
	adcReadings[filterIndex] = batteryVoltage;
	filterIndex = (filterIndex + 1) % FILTER_SIZE; // Circular buffer index

	// Calculate the filtered voltage using the moving average
	filteredVoltage = CalculateAverage(adcReadings, FILTER_SIZE);

    // Calculate battery percentage based on filtered voltage
	percentage = CalculateBatteryPercentage(filteredVoltage);

}


float CalculateBatteryPercentage(float batteryVoltage) {
    float percentage;

    if (batteryVoltage >= 4.20) {  //4.15
        percentage = 99.0;
    } else if (batteryVoltage <= 3.6) {
        percentage = 0.0;
    } else {
        percentage = (batteryVoltage - 3.6) / (4.20 - 3.6) * 100;    //4.15
    }

    return percentage;
}

// Function to calculate the average of an array
float CalculateAverage(float *array, uint8_t size) {
    float sum = 0.0;
    for (uint8_t i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

void DisplayPercentage(void) {

	// Update the display only if there is a significant change in percentage
	if (fabs(percentage - lastPercentage) >= UPDATE_THRESHOLD){
		char buffer1[20];
		snprintf(buffer1, sizeof(buffer1), "%.0f", percentage);
		ssd1306_SetCursor(107, 12);
		ssd1306_WriteString(buffer1, Font_7x10, White);
		//ssd1306_UpdateScreen();
		lastPercentage = percentage;           // Update the last percentage value
		//***************** BATT ICON *******************************************//
		const uint8_t* batteryIcon = GetBatteryIcon(percentage);
		OLED_DrawBitmap(122, 12, batteryIcon, 16, 8);
	}
	else{
		char buffer1[20];
		snprintf(buffer1, sizeof(buffer1), "%.0f", lastPercentage);
		ssd1306_SetCursor(107, 12);
		ssd1306_WriteString(buffer1, Font_7x10, White);
		//***********************************************************************//
		const uint8_t* batteryIcon = GetBatteryIcon(lastPercentage);
		OLED_DrawBitmap(122, 12, batteryIcon, 16, 8);
	}

	if(isCharging){
		// Show charging symbol next to battery icon
		ssd1306_DrawBitmap(99, 12, charging_symbol, 8, 8, White);
		//OLED_DrawBitmap(99, 12, charging_symbol, 8, 8);
	}
	else{
		// Clear the area where the charging symbol would be
		ssd1306_FillRectangle(99, 12, 106, 20, Black);                 //ssd1306_ClearArea
	}

}


const uint8_t* GetBatteryIcon(float percentage) {
    if (percentage >= 75.0) {
        return battery_full;
    } else if (percentage >= 50.0) {
        return battery_half;
    } else if (percentage >= 25.0) {
        return battery_low;
    } else {
        return battery_empty;
    }
}


void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	if (TIM11->SR & TIM_SR_UIF) { // Check interrupt flag
		TIM11->SR &= ~TIM_SR_UIF; // Clear interrupt flag
		BatteryPercentage();      //calculate batt percentage , Every 5 second
	}
}
void TIM1_UP_TIM10_IRQHandler(void) {
	if (TIM10->SR & TIM_SR_UIF) { // Check interrupt flag
		TIM10->SR &= ~TIM_SR_UIF; // Clear interrupt flag
		dotCounter++;             // dot every 1 sec

		inactivityCounter++;      //StandBy Counter 1 sec
		if (inactivityCounter >= TimeToStandBy){           // standBy after 120 sec
			Enter_Standby_Mode();
		}
	}
}

//#######################################################################################################################

// Function to handle menu updates
void LCD_UpdateMenu(void) {
    LCD_DisplayMenu();
}

void MeasureAverage(void) {
for (int var = 1; var <= avgValue+1; var++)
{
	SumBil += BilArray[var];
}
AveragedBil = SumBil / avgValue;
}

// Function to reset the menu to the initial state
void LCD_Reset(void) {
    currentMenu = MENU_SET_AVG;
    currentCursor = CURSOR_ON_MENU;
    avgValue = 1;
    currentTest = 1;
    //************************************//
    for (int i = 0; i <= 12-1; i++) BilArray[i]=0;
    SumBil = 0;
    AveragedBil = 0;
    BilResult=0;
    testDone=0;
    //***********************************//
    HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_RESET); // Turn off the LED
    LCD_UpdateMenu();
}

// Call this function after each test
void DoesTestComplete(void) {

	if (testDone)    				 // if test btn pressed
	{
		testDone=0;
		BilArray[currentTest]=BilResult;
		BilResult=0;

		HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_RESET);
		HAL_Delay(Tests_Intratime);     // Time between Tests

		currentTest++;
		if (currentTest > avgValue)  currentMenu = MENU_SHOW_RESULT;
		LCD_UpdateMenu();
	}
}

void UserAction_Detected(void)
{
    // Reset inactivity counter to 0 when user performs an action
    inactivityCounter = 0;
}





