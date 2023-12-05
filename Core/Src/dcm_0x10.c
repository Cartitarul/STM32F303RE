#include "stm32f3xx_hal.h"
#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
#include "RTE.h"
#include "UART.h"
#include "DCM.h"
#include "dcm_0x10.h"

int Session=1;
void DCM_0x10_Init(uint8_t *error){
	int subF = GetFunction_DCM();
	if(subF==0x01)
		Session=1;
	else if(subF==0x03)
		Session=3;
	else if(subF==0x02 && Session==3)
		Session=2;
	else{
		*error = 0x7E;
	}
}

void ResetDefaultState_0x10(){
	int subF = GetFunction_DCM();
	if(subF==0x03)
		Session=1;
}

uint8_t GetCurrentSession_0x10(){
	return Session;
}
