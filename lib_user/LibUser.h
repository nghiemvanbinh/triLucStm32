#ifndef LibUser_H_
#define LibUser_H_
#include "stdint.h"

#define px_left   HAL_GPIO_ReadPin(tiem_can1_GPIO_Port,tiem_can1_Pin);// tiem can trai
#define px_right  HAL_GPIO_ReadPin(tiem_can2_GPIO_Port,tiem_can2_Pin);// tiem can phai
#define px_center HAL_GPIO_ReadPin(tiem_can3_GPIO_Port,tiem_can3_Pin);// tiem can giua
void json_esp32(char* data);
uint8_t position(void);
void ledSign(int i);
void airConditioner(uint16_t adc, uint16_t upperLimit);
#endif
