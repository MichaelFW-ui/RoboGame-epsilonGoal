/**
 * @file pn5180.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PN5180驱动，原作者lakun@qq.com
 * 未经过测试！！！
 * @version 0.1
 * @date 2021-08-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#ifndef __PN5180_H
#define __PN5180_H

#include "main.h"
#include "rfid.h"

#define USE_SPI_HAL_LIB    1    ///< 是否使用SPI的HAL库函数进行收发数据，=1：启用  =0：不启用

#define pn5180_spi_handle (&hspi2)

int PN5180_Init(RFID_Ordinal_t ordinal);
void PN5180_Reset(RFID_Ordinal_t ordinal);
void PN5180_FieleOff(RFID_Ordinal_t ordinal);
void PN5180_FieldOn(RFID_Ordinal_t ordinal);
uint8_t PN5180_GetFieleState(RFID_Ordinal_t ordinal);
uint32_t PN5180_GetIRQStatus(RFID_Ordinal_t ordinal);
uint32_t PN5180_GetRxStatus(RFID_Ordinal_t ordinal);
uint32_t PN5180_GetRfStatus(RFID_Ordinal_t ordinal);
uint32_t PN5180_GetTransceiveState(RFID_Ordinal_t ordinal);
void PN5180_LoadRFConfiguration(RFID_Ordinal_t ordinal, int protocol);
int PN5180_ClearIRQStatus(RFID_Ordinal_t ordinal, uint32_t Mask);
void PN5180_WriteBytes(RFID_Ordinal_t ordinal, const void* Buffer, uint32_t len);
void PN5180_ReadBytes(RFID_Ordinal_t ordinal, void* Buffer, uint32_t len);
void PN5180_ReadE2Prom(RFID_Ordinal_t ordinal, uint8_t Address, void* Buffer, uint8_t Length);
void PN5180_WriteE2Prom(RFID_Ordinal_t ordinal, uint8_t Address, const void* Buffer, uint8_t Length);
uint32_t PN5180_ReadRegister(RFID_Ordinal_t ordinal, uint8_t Reg);
void PN5180_WriteRegister(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Value);
void PN5180_WriteRegisterOrMask(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Mask);
void PN5180_WriteRegisterAndMask(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Mask);

#endif
