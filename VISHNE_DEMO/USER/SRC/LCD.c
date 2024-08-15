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

int menu_line_X = 35;
int menu_line_Y = 22;   //30

int set_line_X = 40;
int set_line_Y = 44;   //30


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

    switch (currentMenu)
    {
        case MENU_SET_AVG:
        	testDone=0;                       // Do not read any result here

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

                //ssd1306_WriteString("<-", Font_7x10, White);
            }else{
            	ssd1306_SetCursor(set_line_X , set_line_Y);
            	//ssd1306_WriteString("<-", Font_7x10, White);

            	char avgStr[10];
				snprintf(avgStr, sizeof(avgStr), "-%02d-", avgValue);
				ssd1306_WriteString(avgStr, Font_11x18, White);
            }

            //ssd1306_SetCursor(set_line_X, set_line_Y);
            /*char avgStr[10];
            snprintf(avgStr, sizeof(avgStr), "-%02d-", avgValue);
            ssd1306_WriteString(avgStr, Font_11x18, White);*/
            break;

        case MENU_START_TEST:

            /*if (currentCursor == CURSOR_ON_VALUE) {
            	ssd1306_SetCursor(100, 14);
                ssd1306_WriteString("<-", Font_7x10, White);
            }*/
            if (currentCursor == CURSOR_ON_MENU){
				//ssd1306_SetCursor(menu_line_X+70 , menu_line_Y);
				//ssd1306_WriteString("<-", Font_7x10, White);
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
            	//********************//
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
            //ssd1306_SetCursor(menu_line_X, menu_line_Y);
            //ssd1306_WriteString("Result", Font_7x10, White);
            if (currentCursor == CURSOR_ON_MENU){
				//ssd1306_SetCursor(menu_line_X+70, menu_line_Y);
				//ssd1306_WriteString("<-", Font_7x10, White);

				ssd1306_FillRectangle(menu_line_X-5, menu_line_Y-5, 90, 35, White);

				ssd1306_SetCursor(menu_line_X, menu_line_Y);
				ssd1306_WriteString(" Result", Font_7x10, Black);
			}

            ssd1306_SetCursor(set_line_X - 25, set_line_Y);
            char bilStr[20];
            snprintf(bilStr, sizeof(bilStr), "%.2f mg/dL", AveragedBil);
            ssd1306_WriteString(bilStr, Font_11x18, White);
            break;
    }

    ssd1306_UpdateScreen();
}

// Function to handle button presses
void LCD_HandleButtonPress(void) {
    // Assume button GPIOs are connected and configured
    if (HAL_GPIO_ReadPin(GPIOB, NAVIGATE_BTN_Pin) == GPIO_PIN_RESET) { // Navigate Button
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
        }
    } else if (HAL_GPIO_ReadPin(GPIOE, NEXT_BTN_Pin) == GPIO_PIN_RESET) { // Next Button
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










