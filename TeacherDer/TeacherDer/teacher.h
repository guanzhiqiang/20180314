#ifndef _TEACHER_H
#define _TEACHER_H
#include "itcast_asn1_der.h"
#include "keymng_msg.h"

// 编码结构体
int encodeTeacher(Teacher* p, ITCAST_ANYBUF** node);
// 解码结构体
int decodeTeacher(ITCAST_ANYBUF* node, Teacher**p);
// 释放内存函数
void freeTeacher(Teacher** p);

#endif	// _TEACHER_H
