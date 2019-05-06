#include "array_queue.h"
#include <string.h>

Queue myqueue;

//清空某个队列
void queue_Init(Queue *queue)
{
    queue->_tail = queue->_head = 0;
    queue->_state = queue_ok;
    memset(queue, 0x00, QUEUE_MAX_SIZE);
}

//若队列为非空返回0，否则返回非0
quint8 queue_isEmpty(const Queue queue)
{
    if (queue._head != queue._tail)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//若队列非满返回0，否则返回非0
quint8 queue_isFull(const Queue queue)
{
    quint16 next_pos = ((queue._head) % QUEUE_MAX_SIZE);
    if (next_pos != queue._tail)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

quint16 queue_getSize(const Queue queue)
{
    switch (queue._state)
    {
    case queue_full:
        return QUEUE_MAX_SIZE;
        break;
    case queue_empty:
        return 0;
    case queue_ok:
        return ((queue._head - queue._tail + QUEUE_MAX_SIZE) % QUEUE_MAX_SIZE);
    default:
        break;
    }
    return 0;
}

quint8 queue_pop(Queue *queue)
{
    quint16 tail_t = queue->_tail;
    if (queue->_state != queue_empty)
    {
        queue->_tail = (tail_t + 1) % QUEUE_MAX_SIZE;
        if (queue_isEmpty(*queue))
        {
            queue->_state = queue_empty;
        }
        else
        {
            queue->_state = queue_ok;
        }

        return queue->_data[tail_t];
    }
    else
    {
        return 0;
    }
}

void queue_push(Queue *queue, const quint8 _data)
{
    if (queue->_state != queue_full)
    {
        queue->_data[queue->_head] = _data;
        queue->_head = (queue->_head + 1) % QUEUE_MAX_SIZE;
        if (queue_isFull(*queue))
        {
            queue->_state = queue_full;
        }
        else
        {
            queue->_state = queue_ok;
        }
    }
}
