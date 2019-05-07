#ifndef __utility_H
#define __utility_H
#ifdef __cplusplus
extern "C"
{
#endif

//实用tools
#include "main.h"

    typedef uint8_t u_uint8;   //8bits
    typedef uint16_t u_uint16; //16bits
    typedef uint32_t u_uint32; //32bits

#define GET_LOW_8BITS(c) ((u_uint8)((c)&0XFF))
#define GET_HEIGH_8BITS(c) ((u_uint8)(((c) >> 8) & 0xFF))
#define GET_16BITS(H, L) ((u_uint16)(((H & 0xff) << 8) | (L & 0xff)) & 0xffff)
#define GET_32BITS(HH, HL, LH, LL) ((u_uint32)(((HH & 0XFF) << 24) | ((HL & 0XFF) << 16) | ((LH & 0XFF) << 8) | ((LL & 0XFF) << 0)))

#define M_PI 3.1415926
#define RAD2DEG(x) ((x)*180.0 / M_PI) //弧度转角度
#define DEG2RAD(x) ((x)*M_PI / 180.0) //角度转弧度

#define VOLTAGE_CONV(x) ((x)*2500.0 / 100.0)

#ifdef __cplusplus
}
#endif
#endif