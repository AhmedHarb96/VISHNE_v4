#include "../INC/generalHeaders.h"
#include "stm32f4xx_hal.h"

#define FLASH_START_SECTOR FLASH_SECTOR_5  // Starting sector for storage
#define MAX_BIL_READINGS 7                 // Store up to 10 readings
#define MAGIC_NUMBER 0xA5A5A5A5             // Magic number to mark valid entries

typedef struct {
    uint32_t magic;
    float bilValue;
    char patientID[8];  // Patient ID (8 characters)
} FlashEntry;

// Extern BilResult from another .c file
extern float BilResult;

extern char ID[8];

// Internal variables
static uint32_t flash_index = 0;  // Index for the circular buffer

// Function to find the last valid BIL reading index across multiple sectors
void FindLastBilResultIndex(void) {
	flash_index = 0;

    for (int i = 0; i < MAX_BIL_READINGS; i++) {
        uint32_t sectorBaseAddress = GetSectorAddress(i + FLASH_START_SECTOR);
        FlashEntry* entry = (FlashEntry*)sectorBaseAddress;

        if (entry->magic != MAGIC_NUMBER) {
            break;                                //IDLE flash index
        }
        flash_index++;
    }
    if (flash_index >= MAX_BIL_READINGS) {
    	flash_index = 0;  // Circular buffer wrap-around
    }
}

// Function to save BIL result to flash at the next sector
void SaveBilResultToFlash(void) {
    uint32_t sector = FLASH_START_SECTOR + flash_index;

    // Erase the current sector before writing
    EraseFlashSector(sector);

    FlashEntry entry;
    entry.magic = MAGIC_NUMBER;
    entry.bilValue = BilResult;
    // Copy the patient ID and BilResult into the FlashEntry structure
    strncpy(entry.patientID, ID, sizeof(entry.patientID));  // Copy the ID

    uint32_t sectorBaseAddress = GetSectorAddress(sector);

    // Write the new BIL result to the current sector
    HAL_FLASH_Unlock();

    // Write the magic number and float value as 32-bit words
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress, entry.magic);          // magic number ==> First idx(4 size)
    // Write the patient ID (two 32-bit words for 8 characters)						    // ID ==> Second and Third idx(4 + 4 size)
	for (int i = 0; i < sizeof(entry.patientID); i += 4) {								// bilValue ==> Fourth idx(4 size)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 4 + i, *(uint32_t*)&entry.patientID[i]);
	}
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 12, *(uint32_t*)&entry.bilValue);

    HAL_FLASH_Lock();

    // Update index for the next write
    flash_index = (flash_index + 1) % MAX_BIL_READINGS;
}

// Function to read all BIL results from flash into a provided array
void ReadBilResultsFromFlash(float* readings) {
    for (int i = 0; i < MAX_BIL_READINGS; i++) {
        uint32_t sectorBaseAddress = GetSectorAddress(i + FLASH_START_SECTOR);
        FlashEntry* entry = (FlashEntry*)sectorBaseAddress;

        if (entry->magic != MAGIC_NUMBER) {
            break;
        }
        readings[i] = entry->bilValue;
        //readings[i] = *entry;
    }
}

// Function to erase a specific flash sector
void EraseFlashSector(uint32_t sector) {
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t SectorError = 0;

    eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInitStruct.Sector = sector;
    eraseInitStruct.NbSectors = 1;
    eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &SectorError) != HAL_OK) {
        // Handle error
    }
    HAL_FLASH_Lock();
}

// Function to get the base address of a given sector
uint32_t GetSectorAddress(uint32_t sector) {
    switch (sector) {
		//case FLASH_SECTOR_1: return 0x08004001;  // Sector 1 (16 KB)
		//case FLASH_SECTOR_2: return 0x08008001;  // Sector 2 (16 KB)
		//case FLASH_SECTOR_3: return 0x0800C000;  // Sector 3 (16 KB)
		//case FLASH_SECTOR_4: return 0x0801FFFF;  // Sector 4 (64 KB)
	    case FLASH_SECTOR_5: return 0x08020000;  // Sector 5 (128 KB)
		case FLASH_SECTOR_6: return 0x08040000;  // Sector 6 (128 KB)
		case FLASH_SECTOR_7: return 0x08060000;  // Sector 6 (128 KB)
		case FLASH_SECTOR_8: return 0x08080000;  // Sector 6 (128 KB)
		case FLASH_SECTOR_9: return 0x080A0000;  // Sector 6 (128 KB)
		case FLASH_SECTOR_10: return 0x080C0000;  // Sector 6 (128 KB)
		case FLASH_SECTOR_11: return 0x080E0000;  // Sector 6 (128 KB)
		// Add more sectors based on your memory layout
		//default: return 0x08000000;  // Invalid sector (default fallback)
		////default: return 0xFFFFFFFF;  // Invalid sector
    }
}

//############################## ERASE FLASH SECTORS AND WR INDEX RESET ################################

// Function to erase all sectors used for BIL storage
void EraseAllBilFlashSectors(void) {
    HAL_FLASH_Unlock();  // Unlock the flash for erase operation

    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t SectorError = 0;

    eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInitStruct.NbSectors = 1;
    eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    // Erase all sectors used for storing BIL results (from sector 1 to sector 10)
    for (uint32_t sector = FLASH_START_SECTOR; sector < FLASH_START_SECTOR + MAX_BIL_READINGS; sector++) {
        eraseInitStruct.Sector = sector;
        if (HAL_FLASHEx_Erase(&eraseInitStruct, &SectorError) != HAL_OK) {
            // Handle erase error (for example, log it or blink an error LED)
        }
    }
    HAL_FLASH_Lock();  // Lock the flash after erase operation
    // Reset the writing index
    flash_index = 0;
}
