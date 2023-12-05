/*
 * uds.c
 *
 *  Created on: Oct 20, 2023
 *      Author: liviu
 */
#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include "RTE.h"
#include "main.h"
#include "UART.h"
#include "DCM.h"

//Servicii
#include "dcm_0x11.h"
#include "dcm_0x10.h"
#include "dcm_0x22.h"
#define NULL_VALUE 0x0
void DCM_Response(uint8_t nrc, uint8_t serviceIndex);
//Variabile

uint8_t Response_Buffer[100];
typedef void (*InitFunction)(uint8_t *);
const struct {
    uint8_t Service;
    uint8_t Subfunctions[255];
    _Bool HasSubfunction;
    uint8_t Size;
    InitFunction servicePtr;
} DCM_Config[] = {
    {0x10, {0x01, 0x02, 0x03}, 1, 2, &DCM_0x10_Init},  // Row 1
	{0x11, {0x01}, 1,2,&DCM_0x11_Init},   // Row 2
    {0x22, {}, 0,3,&DCM_0x22_RDBI}// Row 2
};
//////////////////////////////////////////////////////////////////////////////

// Functie de verificare serviciu disponibil
_Bool DCM_CheckService(uint8_t service,uint8_t index) {
    for (uint8_t i = 0; i < 2; i++) {
        if (service == DCM_Config[i].Service) {
        	index=i;
        	Response_Buffer[0]=service + 0x40;
            return true;
        }
    }
    return false;
}

//Functie de verificare format NULL
_Bool DCM_CheckFormat(uint8_t function) {
	if(function == NULL_VALUE)
		return false;
	else
		return true;
}

//Functie de verificare SubFunctiilor disponibile
_Bool DCM_CheckFunction(uint8_t service, uint8_t function) {
	uint8_t serviceIndex;
	_Bool RetVal=false;
	for (uint8_t i = 0; i < 2; i++) {
	    if (service == DCM_Config[i].Service) {
	        serviceIndex = i;
	        break;
	    }
	}
	if(DCM_Config[serviceIndex].HasSubfunction){
		for (uint8_t j = 0; j < 2; j++) {
			if (function == DCM_Config[serviceIndex].Subfunctions[j]) {
				RetVal=true;
				Response_Buffer[1]=function;
			}
		}
	}else
		RetVal=true;
	return RetVal;
}

//Functie de verificare size
_Bool DCM_CheckSize(uint8_t service, uint8_t lenght){
	uint8_t serviceIndex;
	for (uint8_t i = 0; i < 2; i++) {
		if (service == DCM_Config[i].Service) {
			serviceIndex = i;
	        break;
	    }
	}
	if(lenght==DCM_Config[serviceIndex].Size)
		return true;
	else
		return false;

}
//Functii
void DCM_Service_Init(){
	resetResponse();
	uint8_t nrc=0;
	uint8_t serviceIndex;
	uint8_t ServiceBuffer = GetService_DCM();
	uint8_t FunctionBuffer = GetFunction_DCM();
	uint8_t FunctionBuffer2 = GetFunction2_DCM();
	uint8_t LenghtBuffer = GetLenght_DCM();
	// Checking if the service is supported

	if(!DCM_CheckService(ServiceBuffer,serviceIndex))
	{
		nrc=0x11;
	}
	else if(!DCM_CheckFormat(FunctionBuffer)){
		nrc=0x13;
	}
	else if(!DCM_CheckFunction(ServiceBuffer,FunctionBuffer)){
			nrc=0x12;
	}
	else if(!DCM_CheckSize(ServiceBuffer,LenghtBuffer)){
		nrc=0x13;
	}
	else
	{
		DCM_Config[serviceIndex].servicePtr(&nrc);
	}
	DCM_Response(nrc,serviceIndex);
}

void DCM_Response(uint8_t nrc, uint8_t serviceIndex){

	if(nrc==0){
		Transmit_Response(Response_Buffer,DCM_Config[serviceIndex].Size);
	}else{
		Response_Buffer[0]= 0x7F;
		Response_Buffer[GetLenght_DCM()-1]= nrc;
		Transmit_Response(Response_Buffer,DCM_Config[serviceIndex].Size);
	}
}

void resetResponse(){
	for(int i =0; i<100;i++)
		Response_Buffer[i]=0;
}
