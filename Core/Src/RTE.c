// RTE Real Time Enviornment
#include "RTE.h"
#include "main.h"
#include "cmsis_os.h"

uint8_t data_send[6]={0x4F,0x22,0x33,0x48,0x21,0x42};
uint16_t size = sizeof(data_send);
void RTE_Task_1ms(){
    
}

void RTE_Task_5ms(){
    
}

void RTE_Task_10ms(){

}

void RTE_Task_100ms(){
    
}

void RTE_Task_500ms(){
    
}

void RTE_Task_1000ms(){
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
    UART_TransmitData(data_send,size);
}

