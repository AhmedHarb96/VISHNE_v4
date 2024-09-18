#include "../INC/generalHeaders.h"
#include "stm32f4xx_hal.h"

#define FLASH_START_SECTOR FLASH_SECTOR_5  // Starting sector for storage
#define MAX_BIL_READINGS 7                 // Store up to 10 readings
#define MAGIC_NUMBER 0xA5A5A5A5             // Magic number to mark valid entries

typedef struct {
    uint32_t magic;
    uint32_t sequence;    // Sequence number to track the order of entries
    float bilValue;
    char patientID[8];  // Patient ID (8 characters)
    uint8_t hours;       // RTC hours
	uint8_t minutes;     // RTC minutes
	uint8_t date;        // RTC date
	uint8_t month;       // RTC month
	uint8_t year;        // RTC year
} FlashEntry;


// Extern BilResult from another .c file
//extern float BilResult;
//extern char ID[8];
//extern uint8_t hours, minutes, seconds, day, date, month, year;

// Internal variables
static uint32_t flash_index = 0;  // Index for the circular buffer
static uint32_t sequenceNumber = 0;  // Global sequence number for entries

// Function to find the last valid BIL reading index across multiple sectors
void FindLastBilResultIndex(void) {
    uint32_t maxSequence = 0;
    flash_index = 0;

    for (int i = 0; i < MAX_BIL_READINGS; i++) {
        uint32_t sectorBaseAddress = GetSectorAddress(i + FLASH_START_SECTOR);
        FlashEntry* entry = (FlashEntry*)sectorBaseAddress;

        if (entry->magic == MAGIC_NUMBER) {
            if (entry->sequence > maxSequence) {
                maxSequence = entry->sequence;
                flash_index = i + 1;
            }
        }
    }

    // Wrap around if the index exceeds the max entries
    if (flash_index >= MAX_BIL_READINGS) {
        flash_index = 0;
    }

    // Set the global sequenceNumber to maxSequence + 1
    sequenceNumber = maxSequence + 1;
}

// Function to save BIL result to flash at the next sector
void SaveBilResultToFlash(void) {
    uint32_t sector = FLASH_START_SECTOR + flash_index;

    // Erase the current sector before writing
    EraseFlashSector(sector);

    RTC_GetTime(&hours, &minutes, &seconds, &day, &date, &month, &year);   //**//
    FlashEntry entry;
    entry.magic = MAGIC_NUMBER;
    entry.sequence = sequenceNumber++;
    entry.bilValue = BilResult;
    strncpy(entry.patientID, ID, sizeof(entry.patientID));
    // Copy RTC values
	entry.hours = hours;
	entry.minutes = minutes;
	entry.date = date;
	entry.month = month;
	entry.year = year;

    uint32_t sectorBaseAddress = GetSectorAddress(sector);

    HAL_FLASH_Unlock();

    // Write the magic number, sequence number, patient ID, and BIL value
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress, entry.magic);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 4, entry.sequence);

    for (int i = 0; i < sizeof(entry.patientID); i += 4) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 8 + i, *(uint32_t*)&entry.patientID[i]);
    }

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 16, *(uint32_t*)&entry.bilValue);

    // Write RTC values (stored as bytes)
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 20,
					 (entry.hours << 24) | (entry.minutes << 16) | (entry.date << 8) | entry.month);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, sectorBaseAddress + 24, entry.year);  // year

    HAL_FLASH_Lock();

    // Update index for the next write
    flash_index = (flash_index + 1) % MAX_BIL_READINGS;
    for (int i = 0; i < 8; i++) ID[i] = 0;  // Reset ID or // memset(ID, 0, sizeof(ID));
}

// Function to read all BIL results from flash into a provided array
void ReadBilResultsFromFlash(FlashEntry* readings) {
    for (int i = 0; i < MAX_BIL_READINGS; i++) {
        uint32_t sectorBaseAddress = GetSectorAddress(i + FLASH_START_SECTOR);
        FlashEntry* entry = (FlashEntry*)sectorBaseAddress;

        if (entry->magic != MAGIC_NUMBER) {                //EMPTY flash index
            break;
        }
        //readings[i] = entry->bilValue;
        readings[i] = *entry;
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
