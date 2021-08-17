/**
 * @file iso15693.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief ISO15693协议，原作者lakun@qq.com
 * 未经过测试！！！
 * @version 0.1
 * @date 2021-08-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string.h>
#include "stdio.h"
#include "delay.h"
#include "spi.h"
#include "pn5180.h"
#include "phhalHw.h"
#include "phhalHw_Pn5180.h"
#include "phhalHw_Pn5180_Reg.h"
#include "phhalHw_Pn5180_Instr.h"
#include "phalICode_Int.h"
#include "iso15693.h"

static int ISO15693_CommandSendAndRecvData(RFID_Ordinal_t ordinal, const uint8_t* CmdFrame, uint8_t CmdFrameLength, uint8_t* RxBuffer, uint16_t* RxLength);




int ISO15693_GetSysInfo(RFID_Ordinal_t ordinal, const uint8_t UID[8], ISO15693CardInfo_Typedef* CardInfo)
{
    int ret = -1;
    uint16_t Length = 0;
    uint8_t Buffer[32];

    uint8_t Command[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_SEND_DATA,
        0x00,
        0X22,
        PHAL_ICODE_CMD_GET_SYSTEM_INFORMATION,
        UID[0],
        UID[1],
        UID[2],
        UID[3],
        UID[4],
        UID[5],
        UID[6],
        UID[7],
    };

    if((ret = ISO15693_CommandSendAndRecvData(ordinal, Command, sizeof(Command), Buffer, &Length)) == 0)
    {
        if(Length == 0X0F)
        {
            memset(CardInfo, 0X00, sizeof(ISO15693CardInfo_Typedef));
            CardInfo->Flags       = Buffer[1];
            CardInfo->DSFID       = Buffer[10];
            CardInfo->AFI         = Buffer[11];
            CardInfo->BlkNum      = Buffer[12] + 1;
            CardInfo->BlkSize     = Buffer[13] + 1;
            CardInfo->ICreference = Buffer[14];
            CardInfo->ViccMemSize = CardInfo->BlkNum * CardInfo->BlkSize;
            ret = 0;
        }
        else
        {
            ret = -2;
        }
    }

    return ret;
}

int ISO15693_ReadSingleBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], uint8_t blockData[4], uint8_t blockNo)
{
    int ret = -1;
    uint16_t Length = 0;
    uint8_t Buffer[32];

    uint8_t Command[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_SEND_DATA,
        0x00,
        0X22,
        PHAL_ICODE_CMD_READ_SINGLE_BLOCK,
        UID[0],
        UID[1],
        UID[2],
        UID[3],
        UID[4],
        UID[5],
        UID[6],
        UID[7],
        blockNo
    };

    if((ret = ISO15693_CommandSendAndRecvData(ordinal, Command, sizeof(Command), Buffer, &Length)) == 0)
    {
        if(Length == 0X05)
        {
            *(uint32_t *)blockData = *((uint32_t *)(Buffer + 1));
            ret = 0;
        }
        else
        {
            ret = -2;
        }
    }

    return ret;
}


int ISO15693_WriteSingleBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], const uint8_t blockData[4], uint8_t blockNo)
{
    uint16_t Length = 0;
    uint8_t Buffer[32];

    uint8_t Command[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_SEND_DATA,
        0x00,
        0X22,
        PHAL_ICODE_CMD_WRITE_SINGLE_BLOCK,
        UID[0],
        UID[1],
        UID[2],
        UID[3],
        UID[4],
        UID[5],
        UID[6],
        UID[7],
        blockNo,
        blockData[0],
        blockData[1],
        blockData[2],
        blockData[3]
    };

    return ISO15693_CommandSendAndRecvData(ordinal, Command, sizeof(Command), Buffer, &Length);
}

int ISO15693_ReadMultiBlock(RFID_Ordinal_t ordinal, const uint8_t UID[8], uint8_t FirstBlockNumber, uint8_t NumBlocksToRead, uint8_t* BlocksData)
{
    int ret = -1;
    uint16_t Length = 0;
    uint8_t  Buffer[512];

    uint8_t Command[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_SEND_DATA,
        0x00,
        0X22,
        PHAL_ICODE_CMD_READ_MULTIPLE_BLOCKS,
        UID[0],
        UID[1],
        UID[2],
        UID[3],
        UID[4],
        UID[5],
        UID[6],
        UID[7],
        FirstBlockNumber,
        NumBlocksToRead - 1,
    };

    if((ret = ISO15693_CommandSendAndRecvData(ordinal, Command, sizeof(Command), Buffer, &Length)) == 0)
    {
        if(((Length - 1) >> 2) == NumBlocksToRead)
        {
            memcpy(BlocksData, &Buffer[1], Length - 1);
            ret = 0;
        }
        else
        {
            ret = -2;
        }
    }

    return ret;
}

int ISO15693_InventorySingleSlot(RFID_Ordinal_t ordinal, uint8_t UID[8])
{
    int ret = -1;
    uint16_t Length = 0;
    uint8_t Command[5];
    uint8_t Buffer[32];

    /* Clears the interrupt register IRQ_STATUS */
    PN5180_ClearIRQStatus(ordinal, PHHAL_HW_PN5180_IRQ_SET_CLEAR_ALL_MASK);

    /* Sets the PN5180 into IDLE state */
    PN5180_WriteRegisterAndMask(ordinal, SYSTEM_CONFIG, (uint32_t)~SYSTEM_CONFIG_COMMAND_MASK);

    /* Activates TRANSCEIVE routine */
    PN5180_WriteRegisterOrMask(ordinal, SYSTEM_CONFIG, SYSTEM_CONFIG_START_SEND_POS);

    /* Sends an inventory Command with 1 slot */
    Command[0] = PHHAL_HW_PN5180_SET_INSTR_SEND_DATA;     // 发送数据命令
    Command[1] = 0X00;                                    // 发送完整数据
    Command[2] = 0X26;                                    // ISO15693协议请求格式中的Flasg字段，0X26为1个slot盘存命令
    Command[3] = PHAL_ICODE_CMD_INVENTORY;                // ISO15693盘存命令
    Command[4] = 0X00;                                    // 掩码长度

    /* Send it to chip */
    if((ret = ISO15693_CommandSendAndRecvData(ordinal, Command, sizeof(Command), Buffer, &Length)) == 0)
    {
        if(Length == 0X0A)
        {
            memcpy(UID, &Buffer[2], 8);  // 拷贝出UID
            ret = 0;
        }
        else
        {
            ret = -2;                    // 读取出错
        }
    }
    return ret;
}

void ISO15693_MultiInventoryDemo(RFID_Ordinal_t ordinal)
{
    int i = 0;
    uint8_t Buffer[128];
    uint16_t NumOfCards = 0;
    uint8_t UID[8];

    /* Reset Hardware */
    PN5180_Reset(ordinal);

__start:

    /* Loads the ISO 15693 protocol into the RF registers */
    PN5180_LoadRFConfiguration(ordinal, HHAL_HW_PN5180_PROTOCOL_ISO15693);

    /* Switches the RF field ON. */
    PN5180_FieldOn(ordinal);

    if((NumOfCards = ISO15693_Inventory16Slots(ordinal, Buffer)) != 0)
    {
        printf("\r\n\r\n######################### %d Card detected ! #########################\r\n", NumOfCards);
        for(i = 0; i < NumOfCards; i++)
        {
            memcpy(UID, &Buffer[i * 8], 8);
            printf(" ------> CARD:%d    UID:[%02X%02X%02X%02X%02X%02X%02X%02X]    ", i + 1, UID[0],UID[1],UID[2],UID[3],UID[4],UID[5],UID[6],UID[7]);
            switch((UID[4] >> 4) & 0X03)
            {
            case HHAL_HW_PN5180_ICODE_SLI:
                printf(" CardType :ICODE_SLI\r\n");
                break;
            case HHAL_HW_PN5180_ICODE_SLIX:
                printf(" CardType :ICODE_SLIX\r\n");
                break;
            case HHAL_HW_PN5180_ICODE_SLIX2:
                printf(" CardType :ICODE_SLIX2\r\n");
                break;
            default:
                printf(" CardType :Unknown!\r\n");
                break;
            }
        }
    }
    else
    {
        printf(" Inventory Failed!\r\n");
    }

    /* Switches the RF field OFF. */
    PN5180_FieleOff(ordinal);

    Delay_ms(1000);

    goto __start;
}

uint16_t ISO15693_Inventory16Slots(RFID_Ordinal_t ordinal, uint8_t* UID)
{
    uint16_t SlotCounter = 0, CardCounter = 0;
    uint16_t RxLength = 0, TxLength = 0;
    uint8_t Command[16];
    uint8_t Buffer[32];
    uint8_t* p = (uint8_t *)UID;

    /* Sends an inventory Command with 16 slots */
    Command[TxLength++] = PHHAL_HW_PN5180_SET_INSTR_SEND_DATA;     // 发送数据命令
    Command[TxLength++] = 0X00;                                    // 发送完整数据
    Command[TxLength++] = 0X06;                                    // ISO15693协议请求格式中的Flasg字段，0X06为16个slot盘存命令
    Command[TxLength++] = PHAL_ICODE_CMD_INVENTORY;                // ISO15693盘存命令
    Command[TxLength++] = 0X00;                                    // 掩码长度

    /* Send it to chip */
    for(SlotCounter = 0; SlotCounter < 16; SlotCounter++)
    {
        /* Clears the interrupt register IRQ_STATUS */
        PN5180_ClearIRQStatus(ordinal, PHHAL_HW_PN5180_IRQ_SET_CLEAR_ALL_MASK);

        /* Sets the PN5180 into IDLE state */
        PN5180_WriteRegisterAndMask(ordinal, SYSTEM_CONFIG, (uint32_t)~SYSTEM_CONFIG_COMMAND_MASK);

        /* Activates TRANSCEIVE routine */
        PN5180_WriteRegisterOrMask(ordinal, SYSTEM_CONFIG, SYSTEM_CONFIG_START_SEND_POS);

        if(ISO15693_CommandSendAndRecvData(ordinal, Command, TxLength, Buffer, &RxLength) == 0)
        {
            if(RxLength == 0X0A)
            {
                memcpy(p, &Buffer[2], 8);  // 拷贝出UID
                p += 8;
                CardCounter++;
            }
        }

        /* Send only EOF (End of Frame) without data at the next RF communication */
        PN5180_WriteRegisterAndMask(ordinal, TX_CONFIG, 0XFFFFFB3F);

        /* Send EOF */
        TxLength = 2;
    }

    return CardCounter;
}

void ISO15693_SingleInventoryDemo(RFID_Ordinal_t ordinal)
{
    int i = 0;
    int ret = 0;
    uint8_t RxBuffer[128];
    uint8_t TxBuffer[128];
    uint8_t UID[8];
    ISO15693CardInfo_Typedef CardInfo;

    /* Reset Hardware */
    PN5180_Reset(ordinal);

    /* Loads the ISO 15693 protocol into the RF registers */
    PN5180_LoadRFConfiguration(ordinal, HHAL_HW_PN5180_PROTOCOL_ISO15693);

    /* Switches the RF field ON. */
    PN5180_FieldOn(ordinal);

__start:

    if((ret = ISO15693_InventorySingleSlot(ordinal, UID)) == 0)
    {
        printf(" ##################################### Card detected ! #####################################\r\n");
        printf(" UID:[%02X%02X%02X%02X%02X%02X%02X%02X]\r\n", UID[0],UID[1],UID[2],UID[3],UID[4],UID[5],UID[6],UID[7]);

        switch((UID[4] >> 4) & 0X03)
        {
        case HHAL_HW_PN5180_ICODE_SLI:
            printf(" CardType :ICODE_SLI\r\n");
            break;
        case HHAL_HW_PN5180_ICODE_SLIX:
            printf(" CardType :ICODE_SLIX\r\n");
            break;
        case HHAL_HW_PN5180_ICODE_SLIX2:
            printf(" CardType :ICODE_SLIX2\r\n");
            break;
        default:
            printf(" CardType :Unknown!\r\n");
            break;
        }

        if(ISO15693_GetSysInfo(ordinal, UID, &CardInfo) == 0)
        {
            if(CardInfo.Flags & 0X01)    // DSFID Flag
            {
                printf(" DSFID:0X%02X\r\n", CardInfo.DSFID);
            }

            if(CardInfo.Flags & 0X02)    // AFI Flag
            {
                printf(" AFI:0X%02X - ", CardInfo.AFI);
                switch(CardInfo.AFI >> 4)
                {
                case 0:
                    printf("All families");
                    break;
                case 1:
                    printf("Transport");
                    break;
                case 2:
                    printf("Financial");
                    break;
                case 3:
                    printf("Identification");
                    break;
                case 4:
                    printf("Telecommunication");
                    break;
                case 5:
                    printf("Medical");
                    break;
                case 6:
                    printf("Multimedia");
                    break;
                case 7:
                    printf("Gaming");
                    break;
                case 8:
                    printf("Data storage");
                    break;
                case 9:
                    printf("Item management");
                    break;
                case 10:
                    printf("Express parcels");
                    break;
                case 11:
                    printf("Postal services");
                    break;
                case 12:
                    printf("Airline bags");
                    break;
                default:
                    printf("Unknown");
                    break;
                }
                printf("\r\n");
            }

            if(CardInfo.Flags & 0X04)    // VICC Memory size
            {
                printf(" Block Size :%dBytes\r\n", CardInfo.BlkSize);
                printf(" Block Num  :%d\r\n", CardInfo.BlkNum);
                printf(" ViccMemSize:%dbytes\r\n", CardInfo.ViccMemSize);
            }

            if(CardInfo.ICreference & 0X08)  // IC reference
            {
                printf(" IC Reference:0X%02X\r\n", CardInfo.ICreference);
            }
        }
        else
        {
            printf(" Read System Information Failed, ErrCode:%d\r\n", ret);
        }

        if((ret = ISO15693_ReadMultiBlock(ordinal, UID, 0, 28, RxBuffer)) == 0)
        {
            printf(" Read Multi Blocks Sccessfully!\r\n");
            for(i = 0; i < 28; i++)
            {
                printf(" BLOCK%2d:[%02X%02X%02X%02X]  ", i, RxBuffer[(i << 2) + 0], RxBuffer[(i << 2) + 1], RxBuffer[(i << 2) + 2], RxBuffer[(i << 2) + 3]);
                if((i + 1) % 4 == 0)
                    printf("\r\n");
            }
        }
        else
        {
            printf(" Read Multi Block Failed, ErrCode:%d\r\n", ret);
        }

        printf(" Write Multi Block Satrt...\r\n");
        for(i = 0; i < 28; i++)
        {
            TxBuffer[0] = i + 1;
            TxBuffer[1] = i + 1;
            TxBuffer[2] = i + 1;
            TxBuffer[3] = i + 1;
            printf("  Block %2d %s.\r\n", i, ISO15693_WriteSingleBlock(ordinal, UID, TxBuffer, i) == 0 ? "OK" : "FAILED");
        }

        printf("==========================================================\r\n\r\n\r\n");
    }
    else
    {
        printf(" Inventory Failed!\r\n");
    }

    Delay_ms(1000);

    goto __start;
}

static int ISO15693_CommandSendAndRecvData(RFID_Ordinal_t ordinal, const uint8_t* CmdFrame, uint8_t CmdFrameLength, uint8_t* RxBuffer, uint16_t* RxLength)
{
    int ret = 0;
    uint16_t delay = 2;
    uint16_t TempLength = 0;
    uint8_t TxBuffer[2];

    // 发送命令
    PN5180_WriteBytes(ordinal, CmdFrame, CmdFrameLength);

    //
    // 等待10ms的数据响应
    //
    // 这里的延时时间也是实际开发测试中测出来的，起初设置了5毫秒，发现找不到卡。
    //
    Delay_ms(10);

    // 检查是否检测到卡片
    if((PN5180_GetIRQStatus(ordinal) & IRQ_STATUS_RX_SOF_DET_IRQ_MASK) == 0)
    {
        ret = -1;
        goto __exit;
    }

    //
    // 等待2ms的数据响应
    //
    // 注意：这里的延时时间是不固定的，延时时间的长短取决于ISO15693协议的卡的响应数据大小。
    //
    // 实测测试发现的问题：
    //
    // 我测试时使用的是ICODE_SLIX类型的卡，这个卡的容量为28个块，当时设置的是10ms的延时时间，
    // 我使用函数ISO15693_ReadMultiBlock读取28个块，发现读取后的数据数量不对，改为30ms的延时时间后发现能够读取正确了。
    // （这个值可以这么设置：当读多块时，延时时间 = 读的块数 + 2ms。例如读取28块，延时时间就是30ms）
    //
    if(CmdFrame[3] == PHAL_ICODE_CMD_READ_MULTIPLE_BLOCKS)
    {
        delay += CmdFrame[13];
    }
    Delay_ms(delay);

    // 检查否有数据收到
    if((TempLength = PN5180_GetRxStatus(ordinal) & 0X1FFU) == 0)
    {
        ret = -2;
        goto __exit;
    }

    // 发送读取数据命令
    TxBuffer[0] = PHHAL_HW_PN5180_GET_INSTR_RETRIEVE_RX_DATA;
    TxBuffer[1] = 0X00;
    PN5180_WriteBytes(ordinal, TxBuffer, 2);

    // 读取数据
    PN5180_ReadBytes(ordinal, (uint8_t *)RxBuffer, TempLength);

    //
    // 接收到的字节0为错误标识，若等于0则无错误；若有错误，字节1为错误码
    //
    if((RxBuffer[0] & 0X01) != 0)
    {
        ret = -3;
        goto __exit;
    }

    *RxLength = TempLength;

__exit:
    PN5180_ClearIRQStatus(ordinal, PHHAL_HW_PN5180_IRQ_SET_CLEAR_ALL_MASK);
    return ret;
}
