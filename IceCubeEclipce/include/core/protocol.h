// -------------------------------------------------------------------
//   Copyright (C) Pedro Igor B. S 2019
// -------------------------------------------------------------------
//   Arquivo: USART.h
//   Descrição: Esse progrma é uma bibliotéca para facilitar o uso do
//   periferio USART do microcontrolador AVR ATMEGA328P
//   Autores: Pedro Igor Borçatti da Silva
//
//   Licença: GNU GPL 2
// -------------------------------------------------------------------
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; version 2 of the
//   License.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// -------------------------------------------------------------------


#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stddef.h>
#include <stdint.h>

#define SWAP_LEN	512

typedef const char 	(CCHR);
typedef uint16_t	(u16);
typedef uint8_t		(u8);
typedef uint32_t 	(u32);
typedef uint64_t 	(u64);



typedef const uint16_t	(cu16);
typedef const uint8_t	(cu8);
typedef const uint32_t 	(cu32);
typedef const uint64_t 	(cu64);


static CCHR tokens[] 		= {',', ':', '=', ';', '&', '\n', '\r', 0x00};

/* captura de elementos  */
bool getLabelInfo(CCHR *str, CCHR *label, const u16 arg , char *bff);
bool getValueInCSV(CCHR *strCSV, const size_t where, char *saveTo);
bool getValueInStr(CCHR *str, const size_t where, CCHR div, char *saveTo);


/* edição de protocolo e strings  */
void rmChr( char *str, CCHR rmChr);
void charToChar(char *str, CCHR chr, CCHR toChr);
void upperCaseStr(char *str);
char *zeroLeft(char *strToAdd, const size_t sizeWithZeros);

/* testes e checagem */
bool belongsInStr(CCHR *STR, CCHR* belongsInSTR);
bool isHEXnum(CCHR *str);
bool isDECnum(CCHR *str);
bool isToken(CCHR tok);

/* utilitários  */
void montBuffer(char *swap, CCHR div, CCHR *str);
void my_strcpy(char *str, CCHR *strCP,  u8 init, u8 end);
void cleanStr(char *str);
void cleanBlk(u8 *blk, u16 len);
void charToChar(char *str, CCHR chr, CCHR toChr);


#endif /* PROTOCOL_H_ */
