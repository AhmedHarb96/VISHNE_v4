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
    MENU_SET_AVG,
    MENU_START_TEST,
    MENU_SHOW_RESULT,
	MENU_EDIT_RTC,
    MENU_TOTAL
} MenuState;

// Cursor positions within a menu
typedef enum {
    CURSOR_ON_MENU,
    CURSOR_ON_VALUE,
    CURSOR_TOTAL
} CursorPosition;

// Variables to keep track of current state
static MenuState currentMenu = MENU_SET_AVG;
static CursorPosition currentCursor = CURSOR_ON_MENU;
static int avgValue = 1;  // Initial average value
static int currentTest = 1;
static uint8_t holdNavigateBtn = 0;
//static float bilResult = 0.0f;
int cnt=1;
float BilArray[12] = {0};
float AveragedBil = 0;
float SumBil = 0;
int Debounce_Delay = 150;
int Tests_Intratime = 500;

int menu_line_X = 35;   //35
int menu_line_Y = 22;   //22

int set_line_X = 40;
int set_line_Y = 44;   //30

int StartTestMenuFlag = 0;
int TimeSetDone=0;

//**//
static uint8_t hours = 0, minutes = 0, seconds = 0, day = 1, date = 1, month = 1, year = 21;
char buffer[20];

void LCD(void)
{
	//LCD_Init();      //LOGO
	LCD_DisplayMenu();
    LCD_HandleButtonPress();
}

// Function to initialize the OLED
/*void LCD_Init(void) {
    //ssd1306_Init();
    //ssd1306_Fill(Black);
    //ssd1306_UpdateScreen();
    LCD_DisplayMenu();
}*/

// Function to display the current menu
void LCD_DisplayMenu(void) {
    ssd1306_Fill(Black);
    HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_RESET); // Turn off Indication LED
    RTC_DisplayTime();
    TimeSetDone=0;

    switch (currentMenu)
    {
        case MENU_SET_AVG:
        	//ssd1306_FillRectangle(126, 0, 128, 128, Black);
        	RTC_DisplayTime();
        	testDone=0;                       // Do not read any result here
        	StartTestMenuFlag = 2;            // Not in StartMenu

        	if (currentCursor == CURSOR_ON_MENU) {
				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 35, White);
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Set AVG", Font_7x10, Black);
        	}else{
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Set AVG", Font_7x10, White);
        	}


            if (currentCursor == CURSOR_ON_VALUE) {
            	ssd1306_FillRectangle(set_line_X, set_line_Y, 75, 52, White);
            	ssd1306_SetCursor(set_line_X , set_line_Y);

            	 char avgStr[10];
				 snprintf(avgStr, sizeof(avgStr), "-%02d-", avgValue);
				 ssd1306_WriteString(avgStr, Font_11x18, Black);

            }else{
            	ssd1306_SetCursor(set_line_X , set_line_Y);

            	char avgStr[10];
				snprintf(avgStr, sizeof(avgStr), "-%02d-", avgValue);
				ssd1306_WriteString(avgStr, Font_11x18, White);
            }

            break;

        case MENU_START_TEST:
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

            DoesTestComplete();             // **if test btn pressed ==> ++currentTest****//

            if (currentTest > avgValue){
            	currentTest = 1;
            	//*********AVG***********//
            	MeasureAverage();
            	HAL_Delay(10);
            	//********************//
                currentMenu = MENU_SHOW_RESULT;
                LCD_DisplayMenu();
            } else {
                // Turn on the indication LED (READY)
                HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_SET); // ***** IND LED ***//
            }
            break;

        case MENU_SHOW_RESULT:
        	RTC_DisplayTime();
        	StartTestMenuFlag = 2;						// Not in StartMenu

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
        	RTC_DisplayTime();
        	StartTestMenuFlag = 2;						// Not in StartMenu
        	if (currentCursor == CURSOR_ON_MENU){

				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 34, White);

				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Edit RTC", Font_7x10, Black);
				//*****************************************//
			    ssd1306_SetCursor(menu_line_X, menu_line_Y+16);
			    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
			    ssd1306_WriteString(buffer, Font_7x10, White);

			    ssd1306_SetCursor(menu_line_X, menu_line_Y+32);
			    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d", date, month, year);
			    ssd1306_WriteString(buffer, Font_7x10, White);
				//*****************************************//
			}else{
				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString("Edit RTC", Font_7x10, White);
        	}

        	if (currentCursor == CURSOR_ON_VALUE){
        		EditRTC();
        	}

			break;
    }

    ssd1306_UpdateScreen();
}

// Function to handle button presses
void LCD_HandleButtonPress(void) {
    // Assume button GPIOs are connected and configured
    if ((HAL_GPIO_ReadPin(GPIOB, NAVIGATE_BTN_Pin) == GPIO_PIN_RESET)) { // Navigate Button
    	RTC_DisplayTime();
        holdNavigateBtn++;
        HAL_Delay(Debounce_Delay); // Debounce delay

        if (holdNavigateBtn >= 12) { // Assuming hold for 2000 ms
            LCD_Reset();
        }
        else {
        	 if (currentMenu == MENU_SET_AVG)
        	 {
				// Move cursor within the menu
				currentCursor = (currentCursor + 1) % CURSOR_TOTAL;
				if(currentCursor>=2)  currentCursor = (currentCursor - 1) % CURSOR_TOTAL;
				LCD_UpdateMenu();
        	 }
        	 if (currentMenu == MENU_EDIT_RTC)
			 {
				// Move cursor within the menu
				currentCursor = (currentCursor + 1) % CURSOR_TOTAL;
				if(currentCursor>=2)  currentCursor = (currentCursor - 1) % CURSOR_TOTAL;
				LCD_UpdateMenu();
			 }
        }
    } else if (HAL_GPIO_ReadPin(GPIOE, NEXT_BTN_Pin) == GPIO_PIN_RESET) { // Next Button
    	RTC_DisplayTime();
        HAL_Delay(Debounce_Delay); // Debounce delay
        if (currentCursor == CURSOR_ON_MENU) {
        	//HAL_Delay(50);
            currentMenu = (currentMenu + 1) % MENU_TOTAL;
            LCD_UpdateMenu();
        } else if (currentCursor == CURSOR_ON_VALUE) {
            if (currentMenu == MENU_SET_AVG) {
                avgValue++;
                if (avgValue > 9) avgValue = 9; // Max AVG value
                LCD_UpdateMenu();
            }
        }
    } else if (HAL_GPIO_ReadPin(GPIOE, PREV_BTN_Pin) == GPIO_PIN_RESET) { // Prev Button
    	RTC_DisplayTime();
        HAL_Delay(Debounce_Delay); // Debounce delay
        if (currentMenu == MENU_SHOW_RESULT) {SumBil = 0;AveragedBil = 0;BilResult=0;testDone=0;}  //*//
        if (currentCursor == CURSOR_ON_MENU) {
            currentMenu = (currentMenu - 1 + MENU_TOTAL) % MENU_TOTAL;
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

		    // Display RTC settings
		    //ssd1306_Fill(Black);

		    //ssd1306_SetCursor(menu_line_X-4, menu_line_Y-8);
		    //ssd1306_WriteString(" Edit RTC", Font_7x10, White);

		    // Display and highlight the component being edited
		    ssd1306_SetCursor(menu_line_X, menu_line_Y+16);
		    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
		    ssd1306_WriteString(buffer, Font_7x10, White);

		    ssd1306_SetCursor(menu_line_X, menu_line_Y+32);
		    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d", date, month, year);
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
		        //case 3:
		            //HighlightNumber(menu_line_X, menu_line_Y+32, "%d", day);
		           // break;
		        case 3:
		            HighlightNumber(menu_line_X, menu_line_Y+32, "%02d", date);
		            break;
		        case 4:
		            HighlightNumber(menu_line_X+22, menu_line_Y+32, "%02d", month);
		            break;
		        case 5:
		            HighlightNumber(menu_line_X+44-2, menu_line_Y+32, "%02d", year);
		            break;
		    }

		/*ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 35, White);
		ssd1306_SetCursor(menu_line_X, menu_line_Y);
		ssd1306_WriteString("Edit RTC", Font_7x10, Black);
		//ssd1306_SetCursor(5, 0);
		//ssd1306_WriteString("Menu> Edit RTC", Font_7x10, White);
		ssd1306_SetCursor(5, 44);
		ssd1306_WriteString(buffer, Font_7x10, White);*/

		// Handle button presses for editing
		if (HAL_GPIO_ReadPin(GPIOE, NEXT_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			switch (editIndex) {
				case 0: hours = (hours + 1) % 24; break;
				case 1: minutes = (minutes + 1) % 60; break;
				case 2: seconds = (seconds + 1) % 60; break;
				//case 3: day = (day % 7) + 1; break;
				case 3: date = (date % 31) + 1; break;
				case 4: month = (month % 12) + 1; break;
				case 5: year = (year + 1) % 100; break;
			}
		} else if (HAL_GPIO_ReadPin(GPIOE, PREV_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			switch (editIndex) {
				case 0: hours = (hours == 0) ? 23 : hours - 1; break;
				case 1: minutes = (minutes == 0) ? 59 : minutes - 1; break;
				case 2: seconds = (seconds == 0) ? 59 : seconds - 1; break;
				//case 3: day = (day == 1) ? 7 : day - 1; break;
				case 3: date = (date == 1) ? 31 : date - 1; break;
				case 4: month = (month == 1) ? 12 : month - 1; break;
				case 5: year = (year == 0) ? 99 : year - 1; break;
			}
		} else if (HAL_GPIO_ReadPin(GPIOB, NAVIGATE_BTN_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(Debounce_Delay); // Debounce delay
			editIndex = (editIndex + 1) % 6;
			holdNavigateBtn++;
			ssd1306_UpdateScreen();

			//if (holdNavigateBtn >= 15) NVIC_SystemReset();

			if (editIndex == 0) { // Completed editing all components
				RTC_SetTime(hours, minutes, seconds, day, date, month, year);

				HAL_Delay(10);
				ssd1306_Fill(Black);
				ssd1306_SetCursor(menu_line_X+5, menu_line_Y+5);
				ssd1306_WriteString("DONE", Font_11x18, White);
				ssd1306_UpdateScreen();
				HAL_Delay(2000);

				TimeSetDone=1;
				//holdNavigateBtn=0;
			}
		}

		ssd1306_UpdateScreen();

	}  //while

    currentMenu = MENU_SET_AVG;
	LCD();
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

		HAL_GPIO_WritePin(GPIOE, READY_LED_Pin, GPIO_PIN_RESET);
		HAL_Delay(Tests_Intratime);     // Time between Tests

		currentTest++;
		if (currentTest > avgValue)  currentMenu = MENU_SHOW_RESULT;
		LCD_UpdateMenu();
	}
}










