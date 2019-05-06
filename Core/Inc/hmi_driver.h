#ifndef __hmi_driver_H
#define __hmi_driver_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "array_queue.h"

#define MAX_MSG_LENGTH 1024

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

    quint16 queue_find_frame(Queue *queue, quint8 *frame);
    void process(quint8 *msg, const quint16 len);

#ifdef __cplusplus
}

#endif
#endif