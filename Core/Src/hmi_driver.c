#include "hmi_driver.h"
#include "usart.h"
#include "utility.h"
#include <stdlib.h>
// #include "crc16.h"

//group1
float phase = 0;
float voltage = 0;
quint8 ctrl = 0;
//frame args
#define FRAME_HEAD 0XEE
#define FRAME_TAIL 0XFFFCFFFF

//com args
#define NOTIFY_TOUCH_PRESS 0X01       //触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE 0X03     //触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK 0X0C    //写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD 0X0D //写FLASH失败
#define NOTIFY_READ_FLASH_OK 0X0B     //读FLASH成功
#define NOTIFY_READ_FLASH_FAILD 0X0F  //读FLASH失败
#define NOTIFY_MENU 0X14              //菜单事件通知
#define NOTIFY_TIMER 0X43             //定时器超时通知
#define NOTIFY_CONTROL 0XB1           //控件更新通知
#define NOTIFY_READ_RTC 0XF7          //读取RTC时间
#define MSG_GET_CURRENT_SCREEN 0X01   //画面ID变化通知
#define MSG_GET_DATA 0X11             //控件数据通知
#define NOTIFY_HandShake 0X55         //握手通知

//wedget type
enum CtrlType
{
    kCtrlUnknown = 0x00,
    kCtrlButton = 0x10, //按钮
    kCtrlText,          //文本
    kCtrlProgress,      //进度条
    kCtrlSlider,        //滑动条
    kCtrlMeter,         //仪表
    kCtrlDropList,      //下拉列表
    kCtrlAnimation,     //动画
    kCtrlRTC,           //时间显示
    kCtrlGraph,         //曲线图控件
    kCtrlTable,         //表格控件
    kCtrlMenu,          //菜单控件
    kCtrlSelector,      //选择控件
    kCtrlQRCode,        //二维码
};

//message prase
typedef struct _msg_type
{
    quint8 _head;
    quint16 _cmd_type;
    quint16 _screen_id;
    quint16 _widget_id;
    quint8 _widget_type;
    quint8 _msg[MAX_MSG_LENGTH];
    quint32 _tail;
} Hmi_msg;
/********************************************************************************/
extern UART_HandleTypeDef huart3;
#define pserialPort (&huart3)
static HAL_StatusTypeDef send_word_uint8(const quint8 ch)
{
    quint8 ch_t = ch & 0xff;
    return HAL_UART_Transmit(pserialPort, (uint8_t *)(&ch_t), 1, 1);
}

void send_string_uint8(quint8 *str, quint16 len)
{
    for (int i = 0; i < len; ++i)
    {
        send_word_uint8(str[i]);
    }
}

void send_string_uint16(quint16 *str, quint16 len)
{
    for (int i = 0; i < len; ++i)
    {
        send_word_uint8(GET_HEIGH_8BITS(str[i]));
        send_word_uint8(GET_LOW_8BITS(str[i]));
    }
}
/********************************************************************************/

//获取帧
quint16 queue_find_frame(Queue *queue, quint8 *frame)
{
    static quint16 pFrame = 0;
    static quint32 _tail = 0;
    quint8 _data = 0;

    while (queue_getSize(*queue) > 0)
    {
        _data = queue_pop(queue);
        if ((pFrame == 0) && (_data != FRAME_HEAD)) //丢失帧头全部丢弃
        {
            continue;
        }

        frame[pFrame++] = _data; //保存

        _tail = ((_tail << 8) | _data) & 0xFFFFFFFF; //获取最后四位,组合成32bit数据

        if (_tail == FRAME_TAIL) //获取到帧尾
        {
            _tail = 0;
            quint16 frame_size = pFrame;
            pFrame = 0;
            //crc16
            return frame_size;
        }
    }

    return 0;
}

static void hmi_button_press(const Hmi_msg msg_t)
{
    switch (msg_t._screen_id)
    {
    case 0:
        if (msg_t._widget_id == 3)
        {
            if (msg_t._msg[1])
            { //按钮按下
                ctrl = 1;
                printf("output enable.\n");
            }
            else
            { //按钮弹起
                ctrl = 0;
                printf("output disable.\n");
            }
        }
        break;

    default:
        break;
    }
}

static void hmi_text_set(const Hmi_msg msg_t)
{
    switch (msg_t._screen_id)
    {
    case 0:
        if (msg_t._widget_id == 1)
        {
            voltage = atof(msg_t._msg);
#if DEBUG
            printf("voltage = %.2lf V\n", voltage);
#endif
        }
        else if (msg_t._widget_id == 2)
        {
            phase = atof(msg_t._msg);
#if DEBUG
            printf("phase = %.2f deg\n", phase);
#endif
        }

        break;

    default:
        break;
    }
}

static void process_notify(const Hmi_msg msg_t)
{
    if (GET_LOW_8BITS(msg_t._cmd_type) == MSG_GET_CURRENT_SCREEN) //屏幕切换通知
    {
        printf("change screen\n");
    }
    else
    {
        // switch (GET_LOW_8BITS(msg_t._cmd_type))
        switch (msg_t._widget_type)
        {
        case kCtrlButton: //按钮控件
            hmi_button_press(msg_t);
            break;
        case kCtrlText: //文本控件
            hmi_text_set(msg_t);
            break;
        case kCtrlProgress: //进度条控件

            break;
        case kCtrlSlider: //滑动条控件

            break;
        case kCtrlMeter: //仪表控件

            break;
        case kCtrlMenu: //菜单控件

            break;
        case kCtrlSelector: //选择控件

            break;
        case kCtrlRTC: //倒计时控件

            break;
        default:
            break;
        }
    }
}

void process(quint8 *msg, const quint16 len)
{
    Hmi_msg msg_t = {0};
    //填充结构体
    msg_t._head = msg[0];
    msg_t._cmd_type = GET_16BITS(msg[1], msg[2]);
    msg_t._screen_id = GET_16BITS(msg[3], msg[4]);
    msg_t._widget_id = GET_16BITS(msg[5], msg[6]);
    msg_t._widget_type = msg[7];
    for (quint16 i = 0; i < (len - 12); i++)
    {
        msg_t._msg[i] = msg[i + 8];
    }
    msg_t._tail = GET_32BITS(msg[len - 4], msg[len - 3], msg[len - 2], msg[len - 1]);
    /************************************************************************************************/
#if DEBUG
    printf(" msg_t._head = %x\n", msg_t._head);
    printf(" msg_t._cmd_type = %x\n", msg_t._cmd_type);
    printf(" msg_t._screen_id = %x\n", msg_t._screen_id);
    printf(" msg_t._widget_id = %x\n", msg_t._widget_id);
    printf(" msg_t._widget_type = %x\n", msg_t._widget_type);
    printf("msg_t._msg[%d] = ", len - 12);
    for (int i = 0; i < (len - 12); i++)
    {
        printf("%x ", msg_t._msg[i]);
    }
    printf("\n");
    printf(" msg_t._tail = %lx\n", msg_t._tail);
#endif

    switch (GET_HEIGH_8BITS(msg_t._cmd_type))
    {
    case NOTIFY_TOUCH_PRESS: //触摸屏按下
        ;
        break;
    case NOTIFY_TOUCH_RELEASE: //触摸屏松开
        ;
        break;
    case NOTIFY_WRITE_FLASH_OK: //写FLASH成功
        ;
        break;
    case NOTIFY_WRITE_FLASH_FAILD: //写FLASH失败
        ;
        break;
    case NOTIFY_READ_FLASH_OK: //读取FLASH成功
        ;
        break;
    case NOTIFY_READ_FLASH_FAILD: //读取FLASH失败
        ;
        break;
    case NOTIFY_READ_RTC: //读取RTC时间
        ;
        break;
    case NOTIFY_CONTROL:
        process_notify(msg_t);
        break;
    case NOTIFY_HandShake: //握手通知
        // NOTIFYHandShake();
        break;
    default:
        break;
    }
}