/*
 * flash.h
 *
 *  Created on: 14 de set de 2018
 *      Pedro Igor: http://engsta.com/stm32-flash-memory-eeprom-emulator/
 *      Baseadno em: http://engsta.com/stm32-flash-memory-eeprom-emulator/
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

uint16_t flash_OpenWrite(uint8_t page);
void	 flash_LockWrite(void);

uint8_t flash_ReadDataByte(uint16_t Address);
uint16_t flash_WriteDataByte(uint16_t Address, uint8_t DataByte);

uint8_t	flash_GetDataBFF(uint16_t addr);
void flash_PutDataBFF(uint16_t addr, uint8_t data);

void flash_UpdateBFF(uint32_t page);
void flash_UpgradeBFF(uint32_t page);
void flash_CleanBFF(void);




#endif /* FLASH_H_ */
