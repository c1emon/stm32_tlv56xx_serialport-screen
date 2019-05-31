#include "TLV56xx.h"

#define VOLTAGE_MAP(x, Y) ((x) * (Y / 10.0) * 2500.0 / 100.0)

extern Group_setting group_setting_t[2];
extern u_float32 amp_voltage;

void TLV56xx_GPIO_Init()
{
    GPIO_InitTypeDef TLV_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_DeInit(GPIOTLV, GPIO_DIN | GPIO_SCK | GPIO_FS | GPIO_LOAD | GPIO_FRE);

    TLV_InitStruct.Pin = GPIO_DIN | GPIO_SCK | GPIO_FS | GPIO_LOAD | GPIO_FRE;
    TLV_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    TLV_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    TLV_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOTLV, &TLV_InitStruct);

    GPIO_DIN_ReSet;
    GPIO_SCK_ReSet;
    GPIO_FS_ReSet;
    GPIO_LOAD_ReSet;
    HAL_Delay(50);

    GPIO_LOAD_Set;
    GPIO_FRE_Set;

    HAL_Delay(50);

    TLV56xx_WriteAll(2047);
}

void TLV56xx_Write(const u_uint16 cmd)
{
    u_uint16 cmd_t = cmd & 0xffff;

    // GPIO_LOAD_Set;
    GPIO_FS_Set;
    GPIO_FS_ReSet;
    for (int i = 0; i < 16; ++i)
    {
        GPIO_SCK_Set;
        if (cmd_t & 0x8000)
            GPIO_DIN_Set;
        else
            GPIO_DIN_ReSet;
        cmd_t <<= 1;
        GPIO_SCK_ReSet;
    }
    GPIO_FS_Set;
    GPIO_LOAD_ReSet;
    // GPIO_LOAD_Set;
}

void TLV56xx_Update(void)
{
    GPIO_FS_Set;
    GPIO_LOAD_ReSet;
}

void TLV56xx_WriteAll(const u_uint16 val)
{
    TLV56xx_WriteChannelDAC(ch1, val);
    TLV56xx_WriteChannelDAC(ch2, val);
    TLV56xx_WriteChannelDAC(ch3, val);
    TLV56xx_WriteChannelDAC(ch4, val);
    TLV56xx_WriteChannelDAC(ch5, val);
    TLV56xx_WriteChannelDAC(ch6, val);
    TLV56xx_WriteChannelDAC(ch7, val);
    TLV56xx_WriteChannelDAC(ch8, val);
}

void TLV56xx_WriteChannelDAC(enum channel ch, const u_uint16 val)
{
    u_uint16 cmd = 0;
    cmd = ((ch & 0xf) << 12) & 0xf000;
    if ((val <= 0xfff) && (val >= 0))
        cmd |= (val & 0xfff);
    TLV56xx_Write(cmd);
}

void TLV56xx_WriteChannelVoltage(enum channel ch, const u_uint16 voltage_mv)
{
    u_uint16 voltage_t = voltage_mv;
    if (voltage_t > 5000)
        voltage_t = 5000;
    u_uint16 dac_val = (u_uint16)(((float)(voltage_t * 0xfff)) / VOLTAGE_REF_VAL);
    TLV56xx_WriteChannelDAC(ch, dac_val);
}

void TLV56xx_WriteCmd()
{
    u_uint16 ctrl0 = 0;
    u_uint16 ctrl1 = 0;
    ctrl0 |= 0x8000;
    ctrl1 |= 0x9000;

    struct TLV56xx_SettingTypeDef setting = {0};
    setting.isPowerOn = fullDeviceOn;
    setting.isDigitalOutPut = false;
    setting.inPutMode = inPutModeStraightBin;
    setting.isDACPairPowerOn = powerDAC_AB | powerDAC_CD | powerDAC_EF | powerDAC_GH;
    setting.DACPairSpeed = speedHigh_DAC_AB | speedHigh_DAC_CD | speedHigh_DAC_EF | speedHigh_DAC_GH;

    ctrl0 |= (0x01 & setting.inPutMode) | (setting.isDigitalOutPut & 0x08) | (0x10 & setting.isPowerOn);

    ctrl1 |= (setting.DACPairSpeed & 0X0F) | ((~setting.isDACPairPowerOn) & 0xf0);

    TLV56xx_Write(ctrl0);
    TLV56xx_Write(ctrl1);
}

void output(const u_float64 t_ms, Group_setting *Group_setting_t)
{
    u_uint16 a, b, c;
    if (Group_setting_t[0].enable)
    {
        //ch00
        if (Group_setting_t[0].freq[0] != 0)
        {
            a = (VOLTAGE_MAP(Group_setting_t[0].voltage[0], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[0].freq[0] * t_ms + DEG2RAD(Group_setting_t[0].phase[0])) + 2500);
        }
        else
        {
            a = VOLTAGE_MAP(Group_setting_t[0].voltage[0], amp_voltage) + 2500;
        }
        //ch01
        if (Group_setting_t[0].freq[1] != 0)
        {
            b = (VOLTAGE_MAP(Group_setting_t[0].voltage[1], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[0].freq[1] * t_ms + DEG2RAD(Group_setting_t[0].phase[1])) + 2500);
        }
        else
        {
            b = VOLTAGE_MAP(Group_setting_t[0].voltage[1], amp_voltage) + 2500;
        }
        //ch02
        if (Group_setting_t[0].freq[2] != 0)
        {
            c = (VOLTAGE_MAP(Group_setting_t[0].voltage[2], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[0].freq[2] * t_ms + DEG2RAD(Group_setting_t[0].phase[2])) + 2500);
        }
        else
        {
            c = VOLTAGE_MAP(Group_setting_t[0].voltage[2], amp_voltage) + 2500;
        }

        TLV56xx_WriteChannelVoltage(ch1, a);
        TLV56xx_WriteChannelVoltage(ch2, b);
        TLV56xx_WriteChannelVoltage(ch3, c);
    }
    else
    {
        TLV56xx_WriteChannelVoltage(ch1, 2500);
        TLV56xx_WriteChannelVoltage(ch2, 2500);
        TLV56xx_WriteChannelVoltage(ch3, 2500);
    }

    if (Group_setting_t[1].enable)
    {
        //ch10
        if (Group_setting_t[1].freq[0] != 0)
        {
            a = (VOLTAGE_MAP(Group_setting_t[1].voltage[0], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[1].freq[0] * t_ms + DEG2RAD(Group_setting_t[1].phase[0])) + 2500);
        }
        else
        {
            a = VOLTAGE_MAP(Group_setting_t[1].voltage[0], amp_voltage) + 2500;

        }
        //ch11
        if (Group_setting_t[1].freq[1] != 0)
        {
            b = (VOLTAGE_MAP(Group_setting_t[1].voltage[1], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[1].freq[1] * t_ms + DEG2RAD(Group_setting_t[1].phase[1])) + 2500);
        }
        else
        {
            b = VOLTAGE_MAP(Group_setting_t[1].voltage[1], amp_voltage) + 2500;
        }
        //ch12
        if (Group_setting_t[1].freq[2] != 0)
        {
            c = (VOLTAGE_MAP(Group_setting_t[1].voltage[2], amp_voltage) * arm_sin_f32(2 * PI * Group_setting_t[1].freq[2] * t_ms + DEG2RAD(Group_setting_t[1].phase[2])) + 2500);
        }
        else
        {
            c = VOLTAGE_MAP(Group_setting_t[1].voltage[2], amp_voltage) + 2500;
        }

        TLV56xx_WriteChannelVoltage(ch4, a);
        TLV56xx_WriteChannelVoltage(ch5, b);
        TLV56xx_WriteChannelVoltage(ch6, c);
    }
    else
    {
        TLV56xx_WriteChannelVoltage(ch4, 2500);
        TLV56xx_WriteChannelVoltage(ch5, 2500);
        TLV56xx_WriteChannelVoltage(ch6, 2500);
    }
}