/*
 * uds.c
 *
 *  Created on: Oct 20, 2023
 *      Author: liviu
 */
#include "stm32f3xx_hal.h"
#include "RTE.h"
#include "main.h"
#include "cmsis_os.h"
#include "UART.h"
#include "DCM.h"

const struct {
    uint8_t Service;
    uint8_t Subfunctions[3];
    uint8_t Size;
    uint8_t *servicePtr;
} DCM_Config[] = {
    {0x10, {0x01, 0x02, 0x03}, 2, &DiagnosticSessionControl},  // Row 1
    {0x11, {0x01}, 2, &EcuReset}   // Row 2
};
void DCM_Service_Init(){
	int i,j,k;
	int ServiceIndex;
	uint8_t ServiceBuffer = GetService_DCM();
	uint8_t FunctionBuffer = GetFunction_DCM();
	uint8_t LenghtBuffer = GetLenght_DCM();
	_Bool ServiceCheck=0,FunctionCheck=0,SizeCheck=0;
	// Checking if the service is supported
	for(i=0;i<2;i++){
		if(ServiceBuffer==DCM_Config[i].Service){
			ServiceIndex=i;
			ServiceCheck=1;
		}
	}
	if(ServiceCheck==0){
		Transmit_NRC(ServiceBuffer,0x11);
		return 0;
	}

	// Checking if the funtions is supported
	for(j=0;j<sizeof(DCM_Config[ServiceIndex].Subfunctions[j]);j++)
		if(FunctionBuffer==DCM_Config[ServiceIndex].Subfunctions[j]){
			FunctionCheck=1;
		}
	if(FunctionCheck==0){
			Transmit_NRC(ServiceBuffer,0x12);
			return 0;
		}

	// Checking if the size is correct
	if(LenghtBuffer==DCM_Config[ServiceIndex].Size){
		SizeCheck=1;
	}
	else{
		Transmit_NRC(ServiceBuffer,0x13);
		return 0;
	}
	if(ServiceCheck==1 && FunctionCheck==1 && SizeCheck==1){
		Transmit_PRC(ServiceBuffer,FunctionBuffer);
		return 0;
	}
}

void DiagnosticSessionControl(){}
void EcuReset(){}
