#include <../include/sysBase/flash.h>
#include <../include/sysBase/utility_timer.h>
#include <stdint-gcc.h>
#include <stm32f10x.h>
#include <stm32f10x_flash.h>
#include <cstring>

/*****************************************************************************
 * Allows to use the internal flash to store non volatile data. To initialize
 * the functionality use the FEE_Init() function. Be sure that by reprogramming
 * of the controller just affected pages will be deleted. In other case the non
 * volatile data will be lost.
******************************************************************************/

/* Private macro -------------------------------------------------------------*/
// CAN BE CHANGED
#define FEE_DENSITY_PAGES		4	 	   		// how many pages are used
#define FEE_PAGE_SIZE			1024	    			// can be 1k or 2k check manual for used device

#define FEE_PAGE_BASE_ADDRESS		0x0801F000			//FLASH_WRProt_Pages60to63  	// choose location for the first EEPROMPage address on the top of flash

// DONT CHANGE
#define FEE_DENSITY_BYTES		((FEE_PAGE_SIZE / 2) * FEE_DENSITY_PAGES - 1)
#define FEE_EMPTY_WORD			((uint16_t)0xFFFF)
#define FEE_ADDR_OFFSET(Address)	(Address * 2)			// 1Byte per Word will be saved to preserve Flash


// PARAMETROS PARA NOVAS FUNÇÕES
#define PAGE(PG)			(FEE_DENSITY_BYTES + 1) - ((FEE_PAGE_SIZE / 2) * (FEE_DENSITY_PAGES-PG))


/* Private variables ---------------------------------------------------------*/
static uint8_t DataBuf[FEE_PAGE_SIZE];



/* Private Functions -----------------------------------------------------------------*/
uint32_t addrPage(uint32_t page);



/*****************************************************************************
*  Delete Flash Space used for user Data, deletes the whole space between
*  RW_PAGE_BASE_ADDRESS and the last uC Flash Page
******************************************************************************/
uint16_t flash_OpenWrite(uint8_t page) {
	if(page >= FEE_DENSITY_PAGES) return 0;
	page = addrPage(page);

	FLASH_Unlock();									// unlock flash
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 	// Clear Flags

	// delete all pages from specified start page to the last page
	FLASH_ErasePage(FEE_PAGE_BASE_ADDRESS + page);

	return FEE_DENSITY_BYTES;
}

/*****************************************************************************
 * bolequear flash para escrita
 *****************************************************************************/
void flash_LockWrite(void){
	FLASH_Lock();
}


/*****************************************************************************
*  Writes once data byte to flash on specified address. If a byte is already
*  written, the whole page must be copied to a buffer, the byte changed and
*  the manipulated buffer written after PageErase.
*******************************************************************************/
uint16_t flash_WriteDataByte (uint16_t Address, uint8_t DataByte) {

	FLASH_Status FlashStatus = FLASH_COMPLETE;

	uint32_t page	= 0;

	// exit if desired address is above the limit (e.G. under 2048 Bytes for 4 pages)
	if (Address > FEE_DENSITY_BYTES) {
		return 0;
	}

	// calculate which page is affected (Pagenum1/Pagenum2...PagenumN)
	page = (FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(Address)) & 0x00000FFF;

	if (page % FEE_PAGE_SIZE) page = page + FEE_PAGE_SIZE;
	page = (page / FEE_PAGE_SIZE) - 1;

	// if current data is 0xFF, the byte is empty, just overwrite with the new one
	if ((*(uint16_t*)(FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(Address))) == FEE_EMPTY_WORD) {

		FlashStatus = FLASH_ProgramHalfWord(FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(Address), (uint16_t)(0x00FF & DataByte));
	}
	else {

		// Copy Page to a buffer
		memcpy(DataBuf, (uint8_t*)FEE_PAGE_BASE_ADDRESS + (page * FEE_PAGE_SIZE), FEE_PAGE_SIZE); // !!! Calculate base address for the desired page

		// check if new data is differ to current data, return if not, proceed if yes
		if (DataByte == *(uint8_t*)(FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(Address))) {
			return 0;
		}

		// manipulate desired data byte in temp data array if new byte is differ to the current
		DataBuf[FEE_ADDR_OFFSET(Address)] = DataByte;

		//Erase Page
		FlashStatus = FLASH_ErasePage(FEE_PAGE_BASE_ADDRESS + page);

		// Write new data (whole page) to flash if data has beed changed
		for(uint32_t i=0; i < (FEE_PAGE_SIZE / 2); i++) {
			if ((uint16_t)(0xFF00 | DataBuf[FEE_ADDR_OFFSET(i)]) != 0xFFFF) {
				FlashStatus = FLASH_ProgramHalfWord((FEE_PAGE_BASE_ADDRESS + (page * FEE_PAGE_SIZE)) + (i * 2), (uint16_t)(0xFF00 | DataBuf[FEE_ADDR_OFFSET(i)]));
			}
		}
	}
	return FlashStatus;
}


/*****************************************************************************
 * gravar um dado novo no baffer
*****************************************************************************/
void flash_PutDataBFF(uint16_t addr, uint8_t data){
	if(addr >= (FEE_PAGE_SIZE / 2)) return;
	DataBuf[FEE_ADDR_OFFSET(addr)] = data;
}

/*****************************************************************************
 * Gravar buffer na flash
 * page indentifica qual setor vai ser gravado
*****************************************************************************/
void flash_UpgradeBFF(uint32_t page){
	if(page >= FEE_DENSITY_PAGES) return;
	page = addrPage(page);


	flash_OpenWrite(page);

	//Erase Page
	//FLASH_ErasePage(FEE_PAGE_BASE_ADDRESS + page);

	// Write new data (whole page) to flash if data has beed changed
	for(uint32_t i=0; i < (FEE_PAGE_SIZE / 2); i++) {
		if ((uint16_t)(0xFF00 | DataBuf[FEE_ADDR_OFFSET(i)]) != 0xFFFF) {
			FLASH_ProgramHalfWord((FEE_PAGE_BASE_ADDRESS + (page * FEE_PAGE_SIZE)) + (i * 2), (uint16_t)(0xFF00 | DataBuf[FEE_ADDR_OFFSET(i)]));
			delay_ms(1);
		}
	}
	flash_LockWrite();
}


/*******************************************************************************
*  Atualizar buffer com o que já exite na flahs
*******************************************************************************/
void flash_UpdateBFF(uint32_t page){
	if(page >= FEE_DENSITY_PAGES) return;
	page = addrPage(page);
	memcpy(DataBuf, (uint8_t*)FEE_PAGE_BASE_ADDRESS + (page * FEE_PAGE_SIZE), FEE_PAGE_SIZE);
}

/*******************************************************************************
*  Read once data byte from a specified address
*******************************************************************************/
uint8_t flash_ReadDataByte (uint16_t Address){
	if(Address > FEE_DENSITY_BYTES) return 0xFF;
	// Get Byte from specified address
	return (*(uint8_t*)(FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(Address)));
}


/*******************************************************************************
*  Ler dado do buffer
*******************************************************************************/
uint8_t flash_GetDataBFF(uint16_t addr){
	if(addr >= (FEE_PAGE_SIZE / 2)) return 0xFF;
	return DataBuf[FEE_ADDR_OFFSET(addr)];
}


void flash_CleanBFF(void){
	for(u16 i=0;i<1024; i++) DataBuf[i] = 0x00;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*******************************************************************************
 * Private Functions
*******************************************************************************/


/*******************************************************************************
*  calcular addr do setor
*******************************************************************************/
uint32_t addrPage(uint32_t page){
	// calculate addr page
	page = PAGE(page);

	// calculate which page is affected (Pagenum1/Pagenum2...PagenumN)
	page = (FEE_PAGE_BASE_ADDRESS + FEE_ADDR_OFFSET(page)) & 0x00000FFF;
	if (page % FEE_PAGE_SIZE) page = page + FEE_PAGE_SIZE;
	page = (page / FEE_PAGE_SIZE) - 1;

	return page;
}
