#!/bin/sh
OUT=include/longzoro/longzoro.h

make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//特殊功能控制----------------------------------------
#define _ATTESTAION_ID 1

#endif" > $OUT
make
