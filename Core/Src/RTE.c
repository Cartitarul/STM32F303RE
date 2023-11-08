// RTE Real Time Enviornment
#include "RTE.h"
#include "main.h"
#include "cmsis_os.h"
#include "UART.h"
#include "DCM.h"

void RTE_Task_1ms(){
    
}

void RTE_Task_5ms(){
    
}

void RTE_Task_10ms(){
	 if(isNewInputReceived()){
	    	DCM_Service_Init();
	    }
}

void RTE_Task_100ms(){
    
}

void RTE_Task_500ms(){
    
}

void RTE_Task_1000ms(){

}

