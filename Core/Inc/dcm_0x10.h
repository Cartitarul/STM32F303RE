/*
 * dcm_10.h
 *
 *  Created on: Nov 8, 2023
 *      Author: liviu
 */

#ifndef INC_SERVICES_DCM_0X10_H_
#define INC_SERVICES_DCM_0X10_H_



extern void DCM_0x10_Init(uint8_t *error);
extern void ResetDefaultState_0x10();
extern uint8_t GetCurrentSession_0x10();
extern int Session;

#endif /* INC_SERVICES_DCM_0X10_H_ */
