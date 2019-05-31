#ifndef __utility_H
#define __utility_H
#ifdef __cplusplus
extern "C"
{
#endif

//实用tools
#include "main.h"
#include "arm_math.h"
    typedef uint8_t u_uint8;   //8bits
    typedef uint16_t u_uint16; //16bits
    typedef uint32_t u_uint32; //32bits
    typedef float32_t u_float32;
    typedef float64_t u_float64;

#define GET_LOW_8BITS(c) ((u_uint8)((c)&0XFF))
#define GET_HEIGH_8BITS(c) ((u_uint8)(((c) >> 8) & 0xFF))
#define GET_16BITS(H, L) ((u_uint16)(((H & 0xff) << 8) | (L & 0xff)) & 0xffff)
#define GET_32BITS(HH, HL, LH, LL) ((u_uint32)(((HH & 0XFF) << 24) | ((HL & 0XFF) << 16) | ((LH & 0XFF) << 8) | ((LL & 0XFF) << 0)))

#define U_PI 3.14159265358979f
#define RAD2DEG(x) ((x)*180.0 / U_PI) //弧度转角度
#define DEG2RAD(x) ((x)*U_PI / 180.0) //角度转弧度

#ifdef __cplusplus
}
#endif
#endif