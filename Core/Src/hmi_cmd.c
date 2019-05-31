#include "hmi_cmd.h"
#include "TLV56xx.h"
#include <stdlib.h>

Group_setting group_setting_t[2] = {{0}, {0}};
u_float32 amp_voltage = 10.000;
u_float32 amp_voltage_temp = 0;

/**************************************************************************************************/
static void screen1_button(const Hmi_msg msg_t)
{
    switch (msg_t._widget_id)
    {
    case 29:
        if (msg_t._msg[1])
        { //按钮按下
            group_setting_t[0].enable = 1;
            printf("output1 enable.\n");
        }
        else
        { //按钮弹起
            group_setting_t[0].enable = 0;
            printf("output1 disable.\n");
        }
        break;

    case 30:
        if (msg_t._msg[1])
        { //按钮按下
            group_setting_t[1].enable = 1;
            printf("output2 enable.\n");
        }
        else
        { //按钮弹起
            group_setting_t[1].enable = 0;
            printf("output2 disable.\n");
        }
        break;

    default:
        break;
    }
}

static void screen2_button(const Hmi_msg msg_t)
{
    switch (msg_t._widget_id)
    {
    case 3:
        if (msg_t._msg[1] == 0)
        {
            amp_voltage = amp_voltage_temp;
            printf("amp_voltage=%f\n", amp_voltage);
        }
        break;

    default:
        break;
    }
}

void hmi_button_press(const Hmi_msg msg_t)
{
    switch (msg_t._screen_id)
    {
    case 1:
        screen1_button(msg_t);
        break;
    case 2:
        screen2_button(msg_t);
        break;

    default:
        break;
    }
}

/**************************************************************************************************/

static void screen1_text(const Hmi_msg msg_t)
{
    switch (msg_t._widget_id)
    {
    case 11:
        group_setting_t[0].voltage[0] = atof(msg_t._msg);
        printf("group_setting_t[0].voltage[0]=%f\n", group_setting_t[0].voltage[0]);
        break;
    case 12:
        group_setting_t[0].voltage[1] = atof(msg_t._msg);
        printf("group_setting_t[0].voltage[1]=%f\n", group_setting_t[0].voltage[1]);
        break;
    case 13:
        group_setting_t[0].voltage[2] = atof(msg_t._msg);
        printf("group_setting_t[0].voltage[2]=%f\n", group_setting_t[0].voltage[2]);
        break;
    case 14:
        group_setting_t[1].voltage[0] = atof(msg_t._msg);
        printf("group_setting_t[1].voltage[0]=%f\n", group_setting_t[1].voltage[0]);
        break;
    case 15:
        group_setting_t[1].voltage[1] = atof(msg_t._msg);
        printf("group_setting_t[1].voltage[1]=%f\n", group_setting_t[1].voltage[1]);
        break;
    case 16:
        group_setting_t[1].voltage[2] = atof(msg_t._msg);
        printf("group_setting_t[1].voltage[2]=%f\n", group_setting_t[1].voltage[2]);
        break;
    case 17:
        group_setting_t[0].freq[0] = atof(msg_t._msg);
        printf("group_setting_t[0].freq[0]=%f\n", group_setting_t[0].freq[0]);
        break;
    case 18:
        group_setting_t[0].freq[1] = atof(msg_t._msg);
        printf("group_setting_t[0].freq[1]=%f\n", group_setting_t[0].freq[1]);
        break;
    case 19:
        group_setting_t[0].freq[2] = atof(msg_t._msg);
        printf("group_setting_t[0].freq[2]=%f\n", group_setting_t[0].freq[2]);
        break;
    case 20:
        group_setting_t[1].freq[0] = atof(msg_t._msg);
        printf("group_setting_t[1].freq[0]=%f\n", group_setting_t[1].freq[0]);
        break;
    case 21:
        group_setting_t[1].freq[1] = atof(msg_t._msg);
        printf("group_setting_t[1].freq[1]=%f\n", group_setting_t[1].freq[1]);
        break;
    case 22:
        group_setting_t[1].freq[2] = atof(msg_t._msg);
        printf("group_setting_t[1].freq[2]=%f\n", group_setting_t[1].freq[2]);
        break;
    case 23:
        group_setting_t[0].phase[0] = atof(msg_t._msg);
        printf("group_setting_t[0].phase[0]=%f\n", group_setting_t[0].phase[0]);
        break;
    case 24:
        group_setting_t[0].phase[1] = atof(msg_t._msg);
        printf("group_setting_t[0].phase[1]=%f\n", group_setting_t[0].phase[1]);
        break;
    case 25:
        group_setting_t[0].phase[2] = atof(msg_t._msg);
        printf("group_setting_t[0].phase[2]=%f\n", group_setting_t[0].phase[2]);
        break;
    case 26:
        group_setting_t[1].phase[0] = atof(msg_t._msg);
        printf("group_setting_t[1].phase[0]=%f\n", group_setting_t[1].phase[0]);
        break;
    case 27:
        group_setting_t[1].phase[1] = atof(msg_t._msg);
        printf("group_setting_t[1].phase[1]=%f\n", group_setting_t[1].phase[1]);
        break;
    case 28:
        group_setting_t[1].phase[2] = atof(msg_t._msg);
        printf("group_setting_t[1].phase[2]=%f\n", group_setting_t[1].phase[2]);
        break;
    default:
        break;
    }
}

static void screen2_text(const Hmi_msg msg_t)
{
    switch (msg_t._widget_id)
    {
    case 2:
        amp_voltage_temp = atof(msg_t._msg);
        break;

    default:
        break;
    }
}

void hmi_text_set(const Hmi_msg msg_t)
{
    switch (msg_t._screen_id)
    {

    case 1:
        screen1_text(msg_t);
        break;
    case 2:
        screen2_text(msg_t);
        break;

    default:
        break;
    }
}