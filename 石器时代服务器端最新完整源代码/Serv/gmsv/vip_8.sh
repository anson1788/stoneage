#!/bin/sh
OUT=include/longzoro/longzoro.h

make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//特殊功能控制----------------------------------------
#define _ATTESTAION_ID 8

#endif" > $OUT
make

tar -zcvf /csa/saserver/gmsv/gmsv_vip_8.tar.gz /csa/saserver/gmsv/gmsvjt /csa/saserver/gmsv/setup.cf 
