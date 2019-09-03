// -------------------------------------------------------------------
//   Copyright (C) Pedro Igor B. S 2019
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
// ------------------------------------------------------------------- //


#include <core/protocol.h>
#include <cctype>
#include <cstring>

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const size_t 	lenToken	= sizeof(tokens);

/* Private Functions ---------------------------------------------------------*/

/*******************************************************************************
 * capiturar infomações seguida de uma label e salva em um bff:
 * 	usart = ALF=55,ALPACA=00,55,ACK=40,55,66,8468,99\n
 *					0  1  2  3
 * 	getLabelInfo(usart, "ACK=", 3, bff);
 * 	bff=8468
********************************************************************************/
bool getLabelInfo(CCHR *str, CCHR *label, const u16 arg , char *bff){
	if(strstr(str, label) == NULL) return false;				// procurar label
	CCHR *aux = strstr(str, label);
	u16 idxArg = 0;

	/* testar se a label está no inicio, e no caso de ser	*/
	/* uma sub string de str é virificado se há um tolken	*/
	/* isso garente que a lebel não seha uma sub string de	*/
	/* outra label Ex casa=5,casaco=6			*/
	if((size_t)(aux - str)){
		aux--;
		if(isToken(*aux) == false) return false;
		aux++;
	}


	/* posisionar o addr apos tolken  */
	while(isToken(*aux) == false){
		if(*label++ != *aux++) return false;				// verificar igualdade da label
	}

	str = aux;								// posicionar str
	aux = bff;								// salvar inicio de bff
	cleanStr(bff);								// limpar bff

	/* buscar token do argumento. Exemplo:	*/
	/* temp=0,1,2,3,4,5,5,6,7;		*/
	do{
		if(isToken(*str++)) idxArg++;
	}while(idxArg <= arg);

	/* copiar argumento se arg = 4		*/
	/* no caso de temp=0,1,2,3,4,5,5,6,7;	*/
	/* o valor copiado será 4		*/
	while(isToken(*str) == false){
		*bff++ = *str++;
	}
	*bff=0x00;
	if(strlen(aux) <= 0) return false;

	return true;
}

/*******************************************************************************
 * limpar e ou tratar strings
********************************************************************************/
void cleanStr(char *str){							// limpara str
	if(str == NULL) return;
	uint16_t len = strlen(str);
	for(uint16_t i=0; i<len; i++) str[i] = 0x00;
}

void cleanBlk(u8 *blk, u16 len){						// limpar bloco
	for(uint16_t i=0; i<len; i++) blk[i] = 0x00;
}


/*******************************************************************************
 * Remove caracteres de uma string
********************************************************************************/
void rmChr( char *str, CCHR rmChr ){						// remover um caracter de uma sttring de maneira porca
	unsigned char chrBff = 0x00;						// variavel auxiliar para troca de dados
	char          *addr  = str;						// ponteiro que apornta o endereço do ponteiro *str
	do{									// lopp para navegar na MEMORIA
		if(rmChr == *str){						// localizar caracter ha ser removido
			addr = str;						// capturar endereço desse caracter
			do{
				chrBff  = *++str;				// ir ao proximo endereço e  salver caracter
				*--str  = chrBff;				// voltar ao endereço  sobreescrever caracer
			}while(*str++);						// ir ao proximo endereço
			str = addr;
		}
		else str++;
	}
	while( *str );								// verificar final da str
}


/*******************************************************************************
 * Procurar termo str pertence  de STR
********************************************************************************/
bool belongsInStr(CCHR *STR, CCHR* belongsInSTR){
	if(strstr(STR, belongsInSTR) == NULL) return false;
	return true;
}

/*******************************************************************************
 * concatena O Buffer de envio com um char divisor e uma strin.
 * no buffer está salvo "42"
 * montBFF('=', "a resposta para a vida e tudo mais")
 * 	42=a resposta para a vida e tudo mais
 * montBFF(0x00, "a resposta para a vida e tudo mais")
 * 	se div for zero ele será iginorado
 * 	42a resposta para a vida e tudo mais
 * se o tamnho da buffer de emvio pé de SENDBFF, portando a soma de
 * srt+ sendBFF + 2 não pode ser maior que SENDBFF
********************************************************************************/
void montBuffer(char *swap, CCHR div, CCHR *str){
	size_t lenBFF = strlen(swap);
	size_t lenSTR = strlen(str);
	if(((lenBFF+lenSTR) + 2) > SWAP_LEN ) return;
	if(div != 0x00){
		swap[lenBFF++] = div;
		swap[lenBFF] = 0x00;
	}
	strcat(swap, str);
}

/*******************************************************************************
 * copia um bloco em qualquer posico de uma str para outra:
 * my_strcpy(str, "0123456789", 2, 6);
 * str irá conter "23456"
********************************************************************************/
void my_strcpy(char *str, CCHR *strCP,  u8 init, u8 end){
	u8 idx = 0;
	if(end < init) return;
	do str[idx++] = strCP[init];
	while(end >= init++);
	str[--idx] = 0x00;
}


/*******************************************************************************
 * add zero a esquerda se necessario
********************************************************************************/
char *zeroLeft(char *strToAdd, const size_t sizeWithZeros){
	while (sizeWithZeros > strlen(strToAdd)){
		size_t idx = strlen(strToAdd);
		do{
			strToAdd[idx+1] = strToAdd[idx];
			if(idx-- == 0) break;
		}while(true);
		strToAdd[0] = '0';
	}

	return strToAdd;
}


/*******************************************************************************
 * troca os caracteres de uma string
 * isso pode ser util para converter pagroes de protocolo
********************************************************************************/
void charToChar(char *str, CCHR chr, CCHR toChr){
	do if(*str == chr) *str = toChr;
	while(*str++);
}

/*******************************************************************************
 * Deixar uma String em CAIXA ALTA
********************************************************************************/
void upperCaseStr(char *str){							// por string em caixa alta
	do *str = toupper(*str);						// lopp para navegar na MEMORIA
	while(*str++);								// verificar final da str
}

/*******************************************************************************
 * verifica se o numo na string tem apenas caracteres para mumeros hexdecimais
********************************************************************************/
bool isHEXnum(CCHR *str){							// navegar pela memoria
	do{
		if(*str == 0x00) break;
		if(isxdigit(*str) == false)					// testar caracteres
			return false;
	}while(*str++);
	return true;
}

/*******************************************************************************
 * verifica se o numo na string tem apenas caracteres para mumeros decimais
********************************************************************************/
bool isDECnum(CCHR *str){							// navegar pela memoria
	do{
		if(*str == 0x00) break;
		if(isdigit(*str) == false)					// testar caracteres
			return false;
	}while(*str++);
	return true;
}

/*******************************************************************************
 * Retorna true caso div seja um caracter divisor ou terminador
********************************************************************************/
bool isToken(CCHR tok){
	for(size_t idx=0; idx<lenToken; idx++){
		if(tokens[idx] == tok) return true;
	}
	return false;
}


/*******************************************************************************
 * Retorna true caso char seja um caracter especial
********************************************************************************/


/*******************************************************************************
 * capitura calores em estruturas csv
 * strCSV: string que está no formato CSV
 * whereValue: onde está o valor que procura
 *	123;3455;alpacas;fff\n
 *	whereValue de alpcacas é '2'
 * saveTo: onde ira salvar o valor
********************************************************************************/
bool getValueInCSV(CCHR *strCSV, const size_t where, char *saveTo){
	for(size_t cont=0; cont<where; cont++){
		/* posicionar cursor no prozimo	; se	*/
		/* nulo não há ; ou where é maior que a	*/
		/* quantidade de ;			*/
		strCSV = strchr( strCSV, ';' );
		if(strCSV == NULL) return false;
		strCSV++;	// pular ; atual
	}

	/* destar se a mais um ;  */
	char *aux = (char*)strchr( strCSV, ';' );
	if((aux == NULL) && (where == 0) ){
		/* caso exixta ; sem strCSV */
		return false;
	}
	else if(aux == NULL){
		/* caso o ; selecionado	pelo	*/
		/* weher seja o ultimo		*/
		strcpy(saveTo, strCSV);
	}
	else{
		/*caos o ; selecionado pelo where nao seja o ultimo */
		memcpy ( saveTo, strCSV, (size_t)(aux - strCSV));
	}

	/* se saveTo estiver vazil  */
	if(strlen(saveTo) <= 0) return false;

	return true;
}

/*******************************************************************************
 * igual a getValueInCSV porem com divizor especificado pelo programador
 *
********************************************************************************/
bool getValueInStr(CCHR *str, const size_t where, CCHR div, char *saveTo){
	for(size_t cont=0; cont<where; cont++){
		/* posicionar cursor no prozimo	div se	*/
		/* nulo não há div ou where é maior que	*/
		/* a quantidade de ;			*/
		str = strchr( str, div );
		if(str == NULL) return false;
		str++;	// pular div atual
	}

	/* destar se a mais um   */
	char *aux = (char*)strchr( str, div );
	if((aux == NULL) && (where == 0) ){
		/* caso exixta div sem str */
		 return false;
	}
	else if(aux == NULL){
		/* caso o div selecionado pelo	*/
		/* weher seja o ultimo		*/
		strcpy(saveTo, str);
	}
	else{
		/*caos o div selecionado pelo where nao seja o ultimo */
		memcpy ( saveTo, str, (size_t)(aux - str));
	}

	/* se saveTo estiver vazil  */
	if(strlen(saveTo) <= 0) return false;

	return true;
}

/*########################################################################################################################################################*/
/*########################################################################################################################################################*/
/*########################################################################################################################################################*/
/* Private Functions ---------------------------------------------------------*/



