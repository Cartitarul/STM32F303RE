#include "stm32f3xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "RTE.h"
#include "UART.h"
#include "DCM.h"
#include "dcm_0x22.h"
#include "dcm_0x11.h"
#include "dcm_0x10.h"
uint8_t Is_SessionAllowed(uint8_t DIDIndex);
void resetResponse();
void DCM_0x22_DIDRead_0xF186(uint8_t *nrc);
// 03 22 F1 86
// 04 2E F1 86 02
//
typedef void (*DIDFuntion)(uint8_t *);
const struct {
    uint16_t DID;
    uint8_t SessionAllowed;
    _Bool DIDWriteSuported;
    DIDFuntion DIDRead;
    DIDFuntion DIDWrite;
} DID_Config[] = {
    {0xF186, 0x07, 0, &DCM_0x22_DIDRead_0xF186, }  // Row 1
};


void DCM_0x22_RDBI (uint8_t error){
	_Bool DID_Exsists = 0;
	uint8_t *RxBuff=RxBufferPointer;
	uint8_t Lenght = RxBuff[0];
	uint16_t DID = ( RxBuff[2] << 8 ) + RxBuff[3];
	uint8_t DIDIndex;

	for (int i=0; i<sizeof(DID_Config[i].DID); i++){
		if(DID_Config[i].DID == DID){
			DID_Exsists = 1;
			DIDIndex = i;
		}
	}
	Response_Buffer[1] = (DID_Config[DIDIndex].DID >> 8) & 0xFF;
	Response_Buffer[2] = DID_Config[DIDIndex].DID & 0xFF;
	if(!DID_Exsists){
		error = 0x12;
	}
	else if(Is_SessionAllowed(DIDIndex)==0){
		error = 0x13;
	}else{
		DID_Config[DIDIndex].DIDRead(&error);
	}
}

void DCM_0x22_DIDRead_0xF186(uint8_t *nrc){
	if(nrc==0){
		Response_Buffer[3]=GetCurrentSession_0x10;
	}
}

uint8_t Is_SessionAllowed(uint8_t DIDIndex){
	_Bool ok=0;
	return DID_Config[DIDIndex].SessionAllowed & GetCurrentSession_0x10();
}



