#ifndef __TVL56XX_H
#define __TVL56XX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdbool.h>
#include "utility.h"
#include "hmi_cmd.h"

//UNUSED PINS P.C-11.12.13
#define GPIOTLV GPIOC
#define GPIO_DIN GPIO_PIN_0
#define GPIO_SCK GPIO_PIN_1
#define GPIO_FS GPIO_PIN_2
#define GPIO_LOAD GPIO_PIN_3
#define GPIO_FRE GPIO_PIN_4

#define GPIO_DIN_Set HAL_GPIO_WritePin(GPIOTLV, GPIO_DIN, GPIO_PIN_SET)
#define GPIO_DIN_ReSet HAL_GPIO_WritePin(GPIOTLV, GPIO_DIN, GPIO_PIN_RESET)

#define GPIO_SCK_Set HAL_GPIO_WritePin(GPIOTLV, GPIO_SCK, GPIO_PIN_SET)
#define GPIO_SCK_ReSet HAL_GPIO_WritePin(GPIOTLV, GPIO_SCK, GPIO_PIN_RESET)

#define GPIO_FS_Set HAL_GPIO_WritePin(GPIOTLV, GPIO_FS, GPIO_PIN_SET)
#define GPIO_FS_ReSet HAL_GPIO_WritePin(GPIOTLV, GPIO_FS, GPIO_PIN_RESET)

#define GPIO_LOAD_Set HAL_GPIO_WritePin(GPIOTLV, GPIO_LOAD, GPIO_PIN_SET)
#define GPIO_LOAD_ReSet HAL_GPIO_WritePin(GPIOTLV, GPIO_LOAD, GPIO_PIN_RESET)

#define GPIO_FRE_Set HAL_GPIO_WritePin(GPIOTLV, GPIO_FRE, GPIO_PIN_SET)
#define GPIO_RRE_ReSet HAL_GPIO_WritePin(GPIOTLV, GPIO_FRE, GPIO_PIN_RESET)

    struct TLV56xx_SettingTypeDef
    {
        bool isPowerOn;
        bool isDigitalOutPut;
        bool inPutMode;
        uint8_t isDACPairPowerOn;
        uint8_t DACPairSpeed;
    };



#define VOLTAGE_REF_VAL (u_uint16)5000

#define fullDeviceOn false
#define fullDeviceOff true
#define inPutModeStraightBin false
#define inPutModeTowComplement true
#define powerDAC_AB 0x10
#define powerDAC_CD 0X20
#define powerDAC_EF 0X40
#define powerDAC_GH 0X80
#define speedHigh_DAC_AB 0X01
#define speedHigh_DAC_CD 0X02
#define speedHigh_DAC_EF 0X04
#define speedHigh_DAC_GH 0X08

    enum channel
    {
        ch1 = 0,
        ch2,
        ch3,
        ch4,
        ch5,
        ch6,
        ch7,
        ch8
    };

    void TLV56xx_GPIO_Init();
    void TLV56xx_Write(const u_uint16 cmd);
    void TLV56xx_Update(void);
    void TLV56xx_WriteAll(const u_uint16 val);
    void TLV56xx_WriteChannelDAC(enum channel ch, const u_uint16 val);
    void TLV56xx_WriteChannelVoltage(enum channel ch, const u_uint16 voltage_mv);
    void output(const u_float64 t_ms, Group_setting *Group_setting_t);

#ifdef __cplusplus
}
#endif
#endif