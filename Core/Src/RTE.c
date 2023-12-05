// RTE Real Time Enviornment
#include "RTE.h"
#include "main.h"
#include "cmsis_os.h"
#include "UART.h"
#include "DCM.h"
#include "dcm_0x11.h"
#include "dcm_0x10.h"
int i=0;
void RTE_Task_1ms(){
    
}

void RTE_Task_5ms(){
    
}

void RTE_Task_10ms(){
}

void RTE_Task_100ms(){
    
}

void RTE_Task_500ms(){
	 if(isNewInputReceived()){
	    	DCM_Service_Init();
	    }
}

void RTE_Task_1000ms(){
	i++;
	if(i==5){
		ResetDefaultState_0x10();
		i=0;
	}
}

void startup(){
	if(isNewInputReceived()){
		DCM_Service_Init();
	}
	if(SWReset==1)
	{
		HAL_NVIC_SystemReset();
	}
}
