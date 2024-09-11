/*
 * Flash.h
 *
 *  Created on: Sep 6, 2024
 *      Author: pc
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

void SaveBilResultToFlash(void);
void ReadBilResultsFromFlash(float* readings);
//void EraseFlashSector(void);
void EraseFlashSector(uint32_t sector);
void FindLastBilResultIndex(void);
uint32_t GetSectorAddress(uint32_t sector);
void EraseAllBilFlashSectors(void);

#endif /* INC_FLASH_H_ */
