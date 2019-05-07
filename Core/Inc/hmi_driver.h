#ifndef __hmi_driver_H
#define __hmi_driver_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "array_queue.h"

#define DEBUG 0

#define MAX_MSG_LENGTH 1024

    extern float phase;
    extern float voltage;
    extern quint8 ctrl;

    quint16
    queue_find_frame(Queue *queue, quint8 *frame);
    void process(quint8 *msg, const quint16 len);

#ifdef __cplusplus
}

#endif
#endif