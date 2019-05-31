#ifndef __hmi_driver_H
#define __hmi_driver_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "array_queue.h"

#define DEBUG 0
#define _CRC16 0

#define MAX_MSG_LENGTH 1024



    //message prase
    typedef struct _msg_type
    {
        quint8 _head;
        quint16 _cmd_type;
        quint16 _screen_id;
        quint16 _widget_id;
        quint8 _widget_type;
        quint8 _msg[MAX_MSG_LENGTH];
#if _CRC16
        quint16 _crc_val;
#endif
        quint32 _tail;
    } Hmi_msg;

    quint16 queue_find_frame(Queue *queue, quint8 *frame);
    void process(quint8 *msg, const quint16 len);

#ifdef __cplusplus
}

#endif
#endif