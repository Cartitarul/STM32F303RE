/*
 * UART.c
 *
 *  Created on: Oct 16, 2023
 *      Author: liviu
 */
#include "main.h"
#include "cmsis_os.h"
#include "UART.h"
#include <string.h>

uint8_t rx_buffer[100];
uint8_t data_buffer[100];
uint8_t cleaned_buffer[100];
uint8_t cleaned2buffer[100];
uint8_t transfer_cplt;

uint8_t rx_indx;
uint8_t rx_data[2];
uint8_t blinkstatus=0;
uint8_t sizedatabuffer;
const static uint8_t ascii_hex[16]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
const static uint8_t hex_ascii[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};




int asciiToHex(char char1, char char2) {
    // Convert ASCII characters to hexadecimal values
	int index1=0,index2=0;
	uint8_t retval=0,retval2=0;
	for(int i =0; i<15;i++){
		if(char1==ascii_hex[i]){
			index1=i;

		}
	}
	for(int i =0; i<15;i++){
			if(char2==ascii_hex[i]){
				index2=i;
			}
		}
	retval = (hex_ascii[index1]<<4) | hex_ascii[index2];
    return retval;
	//return (uint8_t)((uint8_t)( hex_ascii[index1] << 8 ) | (uint8_t)hex_ascii[index2] ) ;
}

void UART_TransmitData(uint8_t *data, uint16_t size) {
    char uartBuffer[size]; // Adjust the size based on your needs

    for (uint16_t i = 0; i < size; ++i) {
        // Convert each byte to a string and transmit
        int len = snprintf(uartBuffer, sizeof(uartBuffer), "%u", data[i]);
        if (len > 0) {
            HAL_UART_Transmit(&huart2, (uint8_t *)uartBuffer, (uint16_t)len, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart2, " ", 1, HAL_MAX_DELAY);
        }
    }
    sprintf(uartBuffer, "\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *)uartBuffer, strlen(uartBuffer), HAL_MAX_DELAY);

}
int isNewInputReceived(void){
    if (newInputReceived)
    {
        newInputReceived = 0; // Reset the flag
        return 1;             // New input received
    }
    else
    {
        return 0; // No new input
    }
}
