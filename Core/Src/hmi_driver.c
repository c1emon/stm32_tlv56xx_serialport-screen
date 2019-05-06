#include "hmi_driver.h"
#include "usart.h"
#include "utility.h"
// #include "crc16.h"

#define FRAME_HEAD 0XEE
#define FRAME_TAIL 0XFFFCFFFF
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

static void process_notify(const Hmi_msg msg_t)
{
    if (GET_LOW_8BITS(msg_t._cmd_type)==MSG_GET_CURRENT_SCREEN)//屏幕切换通知
    {
        printf("change screen\n");
    }
    else
    {
        // switch (GET_LOW_8BITS(msg_t._cmd_type))
        switch (msg_t._widget_type)
        {
        case kCtrlButton: //按钮控件
            printf("kCtrlButton\n");
            break;
        case kCtrlText: //文本控件
            printf("kCtrlText\n");
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

    // printf(" msg_t._head = %x\n", msg_t._head);
    // printf(" msg_t._cmd_type = %x\n", msg_t._cmd_type);
    // printf(" msg_t._screen_id = %x\n", msg_t._screen_id);
    // printf(" msg_t._widget_id = %x\n", msg_t._widget_id);
    // printf(" msg_t._widget_type = %x\n", msg_t._widget_type);
    // printf("msg_t._msg[%d] = ", len - 12);
    // for (int i = 0; i < (len - 12); i++)
    // {
    //     printf("%x ", msg_t._msg[i]);
    // }
    // printf("\n");
    // printf(" msg_t._tail = %lx\n", msg_t._tail);

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