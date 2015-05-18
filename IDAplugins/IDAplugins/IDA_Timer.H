#ifndef __IDA_TIMER_H_
#define	__IDA_TIMER_H_
#include "IDA_Header.h"
/**/
#ifdef __IDA_TIMER_C_/**/
	#define _extern
#else/**/
	#define _extern extern 
#endif
_extern VOID CALLBACK IDA_TIMER(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

#undef _extern
#endif
