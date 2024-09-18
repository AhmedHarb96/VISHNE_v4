/*
 * Flash.h
 *
 *  Created on: Sep 6, 2024
 *      Author: pc
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "generalHeaders.h"

void SaveBilResultToFlash(void);
//void ReadBilResultsFromFlash(FlashEntry* readings);
//void EraseFlashSector(void);
void EraseFlashSector(uint32_t sector);
void FindLastBilResultIndex(void);
uint32_t GetSectorAddress(uint32_t sector);
void EraseAllBilFlashSectors(void);

///////////////////////////////////////////////////
/*void EraseFlashSector(uint32_t sector);

void WriteBilResultToFlash(uint32_t sector, float result);

void BufferBilResult(float newBilResult);

void WriteBufferedDataToFlash(void);

void ResetFlashAndIndex(void);*/


#endif /* INC_FLASH_H_ */


