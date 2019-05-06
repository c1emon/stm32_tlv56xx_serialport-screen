#ifndef __array_queue_H
#define __array_queue_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define QUEUE_MAX_SIZE 1024

    typedef uint8_t quint8;
    typedef uint16_t quint16;
    typedef uint32_t quint32;

    typedef enum _queue_statu
    {
        queue_full = 0,
        queue_empty,
        queue_ok
    } Queue_State;

    typedef struct _queue
    {
        quint16 _head; //头
        quint16 _tail; //尾
        quint8 _data[QUEUE_MAX_SIZE];
        Queue_State _state;
    } Queue;

    extern Queue myqueue;

    void queue_Init(Queue *queue);
    quint8 queue_isEmpty(const Queue queue);
    quint8 queue_isFull(const Queue queue);
    quint16 queue_getSize(const Queue queue);
    quint8 queue_pop(Queue *queue);
    void queue_push(Queue *queue, const quint8 _data);

#ifdef __cplusplus
}
#endif
#endif