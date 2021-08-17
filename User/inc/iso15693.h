/**
 * @file iso15693.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief ISO15693协议，原作者lakun@qq.com
 * 未经过测试！！！
 * @version 0.1
 * @date 2021-08-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __IOS_15693_H
#define __IOS_15693_H

#include <stdint.h>
#include "rfid.h"

typedef struct
{
    uint8_t Flags;
    uint8_t BlkSize;
    uint8_t BlkNum;
    uint8_t DSFID;
    uint8_t AFI;
    uint8_t ICreference;
    uint16_t ViccMemSize;
} ISO15693CardInfo_Typedef;

int ISO15693_GetSysInfo(RFID_Ordinal_t ordinal, const uint8_t UID[8], ISO15693CardInfo_Typedef* CardInfo);
int ISO15693_ReadSingleBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], uint8_t blockData[4], uint8_t blockNo);
int ISO15693_WriteSingleBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], const uint8_t blockData[4], uint8_t blockNo);
int ISO15693_ReadMultiBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], uint8_t FirstBlockNumber, uint8_t NumBlocksToRead, uint8_t* BlocksData);

void ISO15693_SingleInventoryDemo(RFID_Ordinal_t ordinal);
void ISO15693_MultiInventoryDemo(RFID_Ordinal_t ordinal);

int ISO15693_InventorySingleSlot(RFID_Ordinal_t ordinal, uint8_t UID[8]);
uint16_t ISO15693_Inventory16Slots(RFID_Ordinal_t ordinal, uint8_t* UID);

#endif

