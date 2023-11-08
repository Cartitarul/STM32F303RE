/*
 * UART.h
 *
 *  Created on: Oct 16, 2023
 *      Author: liviu
 */

#ifndef INC_UART_H_
#define INC_UART_H_

extern uint8_t rx_indx;
extern uint8_t rx_data[2];
extern uint8_t rx_buffer[100];
extern uint8_t data_buffer[100];
extern uint8_t cleaned_buffer[100];
extern uint8_t cleaned2buffer[100];
extern uint8_t transfer_cplt;
extern uint8_t data_send;
extern char x,y;
int isNewInputReceived(void);
char* uint8ArrayToString(uint8_t* array, size_t arrayLength);
void cleanBuffer_UART(uint8_t *Buffer);
extern char uartBuffer[100];
int asciiToHex(char char1, char char2);
void UART_TransmitData(uint8_t *data, uint16_t size);
void Transmit_NRC(uint8_t Service,uint8_t NRC);
void Transmit_PRC(uint8_t Service,uint8_t PRC);

#endif /* INC_UART_H_ */

