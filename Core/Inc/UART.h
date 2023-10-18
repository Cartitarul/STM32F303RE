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

int asciiToHex(char char1, char char2);
void UART_TransmitData(uint8_t *data, uint16_t size);

#endif /* INC_UART_H_ */

