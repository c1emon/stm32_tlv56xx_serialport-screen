#ifndef __him_cmd_H
#define __him_cmd_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "hmi_driver.h"
#include <stdbool.h>
#include "utility.h"

    typedef struct _group_settings
    {
        bool enable;
        u_float32 freq[3];
        u_float32 voltage[3];
        u_float32 phase[3];
    } Group_setting;

    extern u_float32 amp_voltage;

    extern Group_setting group_setting_t[2];

    void hmi_button_press(const Hmi_msg msg_t);
    void hmi_text_set(const Hmi_msg msg_t);

#ifdef __cplusplus
}
#endif
#endif