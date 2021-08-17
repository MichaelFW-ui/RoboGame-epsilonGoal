/**
 * @file pn5180.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PN5180驱动，原作者lakun@qq.com
 * 未经过测试！！！
 * @version 0.1
 * @date 2021-08-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "pn5180.h"
#include "delay.h"
#include "spi.h"
#include "phhalHw.h"
#include "phhalHw_Pn5180.h"
#include "phhalHw_Pn5180_Reg.h"
#include "phhalHw_Pn5180_Instr.h"
#include "phalICode_Int.h"
#include <string.h>
#include "stdio.h"

//////////////////////////////////////////////////// SPI移植修改区 ///////////////////////////////////////////////////////


static uint8_t PN5180_SPI_ReadWriteByte(RFID_Ordinal_t ordinal, uint8_t TxData)
{
    uint8_t RxData = 0;
    if(HAL_SPI_TransmitReceive(pn5180_spi_handle, &TxData, &RxData, 1, 2) != HAL_OK)
    {
        RxData = 0XFF;
    }
    return RxData;
}


void PN5180_Reset(RFID_Ordinal_t ordinal)
{
    if (ordinal == RFID_A) {
        PN5180_RST_A_GPIO_Port->ODR |= PN5180_RST_A_Pin;
        Delay_ms(2);
        PN5180_RST_A_GPIO_Port->ODR &= ~PN5180_RST_A_Pin;
        Delay_ms(2);
        PN5180_RST_A_GPIO_Port->ODR |= PN5180_RST_A_Pin;
        Delay_ms(2);
    } else {
        PN5180_RST_B_GPIO_Port->ODR |= PN5180_RST_B_Pin;
        Delay_ms(2);
        PN5180_RST_B_GPIO_Port->ODR &= ~PN5180_RST_B_Pin;
        Delay_ms(2);
        PN5180_RST_B_GPIO_Port->ODR |= PN5180_RST_B_Pin;
        Delay_ms(2);
    }
}

int PN5180_BusyPinIsHigh(RFID_Ordinal_t ordinal)
{
    if (ordinal == RFID_A) {
        return HAL_GPIO_ReadPin(PN5180_BUSY_A_GPIO_Port, PN5180_BUSY_A_Pin) == GPIO_PIN_SET ? 1 : 0;
    } else {
        return HAL_GPIO_ReadPin(PN5180_BUSY_B_GPIO_Port, PN5180_BUSY_B_Pin) == GPIO_PIN_SET ? 1 : 0;
    }
}

int PN5180_BusyPinIsLow(RFID_Ordinal_t ordinal)
{
    if (ordinal == RFID_A) {
        return HAL_GPIO_ReadPin(PN5180_BUSY_A_GPIO_Port, PN5180_BUSY_A_Pin) == GPIO_PIN_RESET ? 1 : 0;
    } else {
        return HAL_GPIO_ReadPin(PN5180_BUSY_B_GPIO_Port, PN5180_BUSY_B_Pin) == GPIO_PIN_RESET ? 1 : 0;
    }
}

void PN5180_Select(RFID_Ordinal_t ordinal)
{
    if (ordinal == RFID_A) {
        VA_CS_GPIO_Port->ODR &=~VA_CS_Pin;
    } else {
        VB_CS_GPIO_Port->ODR &=~VB_CS_Pin;
    }
}

void PN5180_DisSelect(RFID_Ordinal_t ordinal)
{
    if (ordinal == RFID_A) {
        VA_CS_GPIO_Port->ODR |= VA_CS_Pin;
    } else {
        VB_CS_GPIO_Port->ODR |= VB_CS_Pin;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////












void PN5180_WriteBytes(RFID_Ordinal_t ordinal, const void* Buffer, uint32_t len)
{
    int i = 0;
    uint8_t* p = (uint8_t *)Buffer;

    /* Wait for the Busy to be low */
    while(PN5180_BusyPinIsHigh(ordinal)) {}

    /* Enable chip select connected to reader IC by pulling NSS low. */
    PN5180_Select(ordinal);

    /* Build the Command frame and send it */
    for(i = 0; i < len; i++)
    {
        PN5180_SPI_ReadWriteByte(ordinal, *p++);
    }

    /* Disable chip select connected to reader IC by pulling NSS high. */
    PN5180_DisSelect(ordinal);
}

void PN5180_ReadBytes(RFID_Ordinal_t ordinal, void* Buffer, uint32_t len)
{
    int i = 0;
    uint8_t* p = (uint8_t *)Buffer;

    /* Wait for the Busy to be low */
    while(PN5180_BusyPinIsHigh(ordinal)) {}

    /* Enable chip select connected to reader IC by pulling NSS low. */
    PN5180_Select(ordinal);

    /* Build the Command frame and send it */
    for(i = 0; i < len; i++)
    {
        *p++ = PN5180_SPI_ReadWriteByte(ordinal, 0XFF);
    }

    /* Disable chip select connected to reader IC by pulling NSS high. */
    PN5180_DisSelect(ordinal);
}

void PN5180_ReadE2Prom(RFID_Ordinal_t ordinal, uint8_t Address, void* Buffer, uint8_t Length)
{
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_GET_INSTR_READ_E2PROM,
        Address,
        Length
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));  // 写操作命令
    PN5180_ReadBytes(ordinal, Buffer, Length);         // 读取返回数据
}

void PN5180_WriteE2Prom(RFID_Ordinal_t ordinal, uint8_t Address, const void* Buffer, uint8_t Length)
{
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_WRITE_E2PROM,
        Address,
        Length
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));  // 写操作命令
    PN5180_WriteBytes(ordinal, Buffer, Length);        // 发送数据
}

void PN5180_WriteRegisterAndMask(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Mask)
{
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER_AND_MASK,
        Reg,
        (uint8_t)(Mask & 0xFFU),
        (uint8_t)((Mask >> 8U) & 0xFFU),
        (uint8_t)((Mask >> 16U) & 0xFFU),
        (uint8_t)((Mask >> 24U) & 0xFFU)
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
}

void PN5180_WriteRegisterOrMask(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Mask)
{
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER_OR_MASK,
        Reg,
        (uint8_t)(Mask & 0xFFU),
        (uint8_t)((Mask >> 8U) & 0xFFU),
        (uint8_t)((Mask >> 16U) & 0xFFU),
        (uint8_t)((Mask >> 24U) & 0xFFU)
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
}

void PN5180_WriteRegister(RFID_Ordinal_t ordinal, uint8_t Reg, uint32_t Value)
{
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_SET_INSTR_WRITE_REGISTER,
        Reg,
        (uint8_t)(Value & 0xFFU),
        (uint8_t)((Value >> 8U) & 0xFFU),
        (uint8_t)((Value >> 16U) & 0xFFU),
        (uint8_t)((Value >> 24U) & 0xFFU)
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
}

uint32_t PN5180_ReadRegister(RFID_Ordinal_t ordinal, uint8_t Reg)
{
    uint32_t value = 0;
    uint8_t frame[] =
    {
        PHHAL_HW_PN5180_GET_INSTR_READ_REGISTER,
        Reg
    };
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
    PN5180_ReadBytes(ordinal, &value, 4);
    return value;
}

int PN5180_Init(RFID_Ordinal_t ordinal)
{
    uint16_t FwVersion = 0X0000;

    uint8_t DigitalDelayCfg = 0X00;

    /* Reset Hardware */
    PN5180_Reset(ordinal);

    /* Get Firmware Version */
    PN5180_ReadE2Prom(ordinal, PHHAL_HW_PN5180_FIRMWARE_VERSION_ADDR, &FwVersion, 2);
    if(FwVersion == 0XFFFF)
    {
        printf(" pn5180 init failed!\r\n");
        return -1;
    }
    printf(" pn5180 init ok,firmware version:%d.%d\r\n",(FwVersion >> 8 ) & 0XFF, FwVersion & 0XFF);

    /* Disable Idle IRQ */
    PN5180_WriteRegisterAndMask(ordinal, IRQ_ENABLE, (uint32_t)~IRQ_SET_CLEAR_IDLE_IRQ_CLR_MASK);

    /* Clear all IRQs  */
    PN5180_WriteRegister(ordinal, IRQ_SET_CLEAR, PHHAL_HW_PN5180_IRQ_SET_CLEAR_ALL_MASK);

    /* Apply HAL Digital delay when pn5180 FW version is less than 3.8. */
    if(FwVersion >= 0X308)
    {
        PN5180_ReadE2Prom(ordinal, PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_ADDR, &DigitalDelayCfg, PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_LEN);

        /* Apply FW Digital delay and enable timer 1 for the use of FDT/FWT for FW version 3.8 onwards. */
        if (((0U == ((DigitalDelayCfg & PHHAL_HW_PN5180_DIGITAL_DELAY_ENABLE)))) ||
                (!((DigitalDelayCfg & PHHAL_HW_PN5180_FDT_TIMER_USED_MASK) == ((PHHAL_HW_PN5180_FDT_TIMER_USED) << 1U))))
        {
            /* Clear timer bits. */
            DigitalDelayCfg &= (uint8_t)~(PHHAL_HW_PN5180_FDT_TIMER_USED_MASK);

            /* Enable FW digital delay and timer 1 for FDT/FWT. */
            DigitalDelayCfg |= (uint8_t)(PHHAL_HW_PN5180_DIGITAL_DELAY_ENABLE | ((PHHAL_HW_PN5180_FDT_TIMER_USED) << 1U));

            /* Write back MISC_CONFIG value */
            PN5180_WriteE2Prom(ordinal, PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_ADDR, &DigitalDelayCfg, PHHAL_HW_PN5180_DIGITAL_DELAY_CONFIG_LEN);
        }
    }

    return 0;
}

/// 关闭射频场
void PN5180_FieleOff(RFID_Ordinal_t ordinal)
{
    uint8_t frame[2] = {PHHAL_HW_PN5180_GET_INSTR_FIELD_OFF, 0};

    // 发送关闭射频命令
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
}

/// 打开射频场
void PN5180_FieldOn(RFID_Ordinal_t ordinal)
{
    uint8_t frame[2] = {PHHAL_HW_PN5180_GET_INSTR_FIELD_ON,0};

    // 发送开启射频命令
    PN5180_WriteBytes(ordinal, frame, sizeof(frame));
}

/// 获取PN5180模块射频状态，return： =1：射频开  =0：射频关
uint8_t PN5180_GetFieleState(RFID_Ordinal_t ordinal)
{
    if((PN5180_ReadRegister(ordinal, RF_STATUS) & RF_STATUS_TX_RF_STATUS_MASK ) == RF_STATUS_TX_RF_STATUS_MASK)
    {
        return 1;
    }
    return 0;
}

uint32_t PN5180_GetIRQStatus(RFID_Ordinal_t ordinal)
{
    return PN5180_ReadRegister(ordinal, IRQ_STATUS);
}

uint32_t PN5180_GetRxStatus(RFID_Ordinal_t ordinal)
{
    return PN5180_ReadRegister(ordinal, RX_STATUS);
}

uint32_t PN5180_GetRfStatus(RFID_Ordinal_t ordinal)
{
    return PN5180_ReadRegister(ordinal, RF_STATUS);
}

int PN5180_ClearIRQStatus(RFID_Ordinal_t ordinal, uint32_t Mask)
{
    int ret = -1;
    PN5180_WriteRegister(ordinal, IRQ_SET_CLEAR, Mask);
    if((PN5180_GetIRQStatus(ordinal) & Mask) == 0)
    {
        ret = 0;
    }
    return ret;
}

/**
 * @return
 * TRANSCEIVE_STATEs:
 *  0 - idle
 *  1 - wait transmit
 *  2 - transmitting
 *  3 - wait receive
 *  4 - wait for data
 *  5 - receiving
 *  6 - loopback
 *  7 - reserved
 */
uint32_t PN5180_GetTransceiveState(RFID_Ordinal_t ordinal)
{
    return ((PN5180_GetRfStatus(ordinal) >> 24) & 0X07);
}

void PN5180_LoadRFConfiguration(RFID_Ordinal_t ordinal, int protocol)
{
    uint8_t Command[3] = {PHHAL_HW_PN5180_SET_INSTR_LOAD_RF_CONFIGURATION, 0X00, 0X00};

    switch(protocol)
    {
    case HHAL_HW_PN5180_PROTOCOL_ISO14443:
        Command[1] = PHHAL_HW_PN5180_RF_TX_ISO14443A_106_MILLER;
        Command[2] = PHHAL_HW_PN5180_RF_RX_ISO14443A_106_MANCH_SUBC;
        break;
    case HHAL_HW_PN5180_PROTOCOL_ISO15693:
        Command[1] = PHHAL_HW_PN5180_RF_TX_ISO15693_26_1OF4_ASK100;
        Command[2] = PHHAL_HW_PN5180_RF_RX_ISO15693_26_1OF4_SC;
        break;
    case HHAL_HW_PN5180_PROTOCOL_ISO18000:
        break;
    default:
        break;
    }

    /* Send it to chip */
    PN5180_WriteBytes(ordinal, Command, 3);
}
