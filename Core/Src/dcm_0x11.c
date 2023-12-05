#include "stm32f3xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "RTE.h"
#include "UART.h"
#include "DCM.h"
#include "dcm_0x11.h"
uint8_t SWReset=0;
void DCM_0x11_Init (uint8_t error){
	SWReset=0;
	int subF = GetFunction_DCM();
	if(subF==0x01){
		uint8_t SWReset=1;
	}
	else
		error=0x12;
}
