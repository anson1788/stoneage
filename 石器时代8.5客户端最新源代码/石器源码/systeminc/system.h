/************************/
/*	system.h			*/
/************************/
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/**** SYSTEM INCLUDE ****/
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdio.h>

/**** DirectDraw Include ****/
#include <ddraw.h>

/**** USER INCLUDE ****/
#include "directDraw.h"
#include "main.h"
#include "gamemain.h"
#include "sprmgr.h"
#include "init.h"
#include "process.h"
#include "action.h"
#include "sprdisp.h"
#include "math2.h"
#include "../oft/work.h"
#include "chat.h"
#include "font.h"
#include "mouse.h"
#include "radar.h"
#include "gemini.h"
#include "pattern.h"

#ifdef _STONDEBUG_
extern unsigned int iTotalUseTime;
extern unsigned int iTotalProcTime;
extern unsigned int iTotalRunCount;
extern LARGE_INTEGER tf;
#endif

#endif
