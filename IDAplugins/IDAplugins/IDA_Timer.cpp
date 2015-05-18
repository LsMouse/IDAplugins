#define __IDA_TIMER_C_
#include "IDA_Timer.H"
VOID CALLBACK IDA_TIMER(
	HWND hWnd,	/* handle of window for timer messages*/	UINT uMsg,	/* WM_TIMER message*/
	UINT idEvent,/*timer identifier*/	DWORD dwTime /*current system time*/){
	Debug_Run(msg("Timer2Proc Run Timer2Proc\n"));
	return;
}