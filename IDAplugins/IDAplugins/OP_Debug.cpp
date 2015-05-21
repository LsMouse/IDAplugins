#define __IDA_MUL_DEBUG_C_
#include"IDA_Header.H"
//声明共用内存，
#pragma data_seg("Debug-Module")
ea_t PageSize = 0;
ea_t Server_StartEA = NULL;
ea_t Server_EndEA = NULL;
ea_t Server_ThisEA = NULL;
ea_t Client_StartEA = NULL;
ea_t Client_EndEA = NULL;
#pragma data_seg() 
//
int DebugMode = 0;
int Handler_Time = 0;
boolean RealTime = FALSE;
boolean ForceZero = FALSE;
//以下定义UI和模式枚举
const char ASK_DEBUG_UI[] ="Debug Module\n\n\n"
						"<##模式选择## #设置地址#~S~erver:R:32:16:>\n"
						"<#客户端地址# ~C~lient:R:32:16::>>\n"
						"<##配置## #实时调试# ~R~eal-time:C>\n"
						"<#地址强行对齐# ~F~orce:C>>\n"
						"<##对齐地址设置#对齐大小#对齐大小Hex:M:8:10::>>\n";
enum{
	MODE_MULD_SAVE,
	MODE_MULD_READ,
};
/**
* @See	设置服务器程序偏移地址
*/
void Set_ServerThis(){
	ea_t _EA = get_screen_ea();
	if ((_EA < Server_StartEA) || (_EA > Server_EndEA))return;
	if ((_EA - Server_StartEA) == Server_ThisEA)return;
	Debug::MSG("Set_ServerThis()@ Update\n");
	Server_ThisEA = _EA - Server_StartEA;
}
/**
* @See	更新客户端地址
*/
void Client_Update(){
	Debug::MSG("Client_Update()@ Update\n");
	jumpto(Client_StartEA + Server_ThisEA);
}
/**
* @See	服务端定时器
*/
VOID CALLBACK TIMER_Server(
	HWND hWnd,	/* handle of window for timer messages*/	UINT uMsg,	/* WM_TIMER message*/
	UINT idEvent,/*timer identifier*/	DWORD dwTime /*current system time*/){
	Set_ServerThis();
}
/**
* @See	客户端定时器
*/
VOID CALLBACK TIMER_Client(
	HWND hWnd,	/* handle of window for timer messages*/	UINT uMsg,	/* WM_TIMER message*/
	UINT idEvent,/*timer identifier*/	DWORD dwTime /*current system time*/){
	Client_Update();
}
/*
*			IDA插件调试模块
*主要功能.
*		1.设置地址
*		2.读取地址
*主要配置
*
*/
int Debug_Moude(){
	//  恢复上次设置的配置数据
	ushort _CONFIG = 0;
	if (RealTime)_CONFIG |= 0x01;
	if (ForceZero)_CONFIG |= 0x02;
	Debug::MSG("Debug_Moude()\n");
	KillTimer(NULL, Handler_Time);
	if (AskUsingForm_c(ASK_DEBUG_UI, &DebugMode, &_CONFIG, &PageSize) == 0)return 0;
	Debug::MSG("_CONFIG:%d,PageSize:0x%08x\n", _CONFIG, PageSize);
	//设置Check获取到数据参数
	if (_CONFIG & 0x01) RealTime = TRUE;
	else RealTime = FALSE;
	if (_CONFIG & 0x02) ForceZero = TRUE;
	else ForceZero = FALSE;
	//模式选择
	ea_t _Size = ~PageSize;
	switch (DebugMode){
	case MODE_MULD_SAVE:
		if (_Size == 0)Server_StartEA = 0;
		else{
			Server_StartEA = getseg(get_screen_ea())->startEA & _Size;
		}
		Server_EndEA = getseg(get_screen_ea())->endEA;
		Set_ServerThis();
		if (RealTime){
			Handler_Time = SetTimer(NULL, 0, 1000, TIMER_Server);
		}
		break;
	case MODE_MULD_READ:
		if (_Size == 0)Server_StartEA = 0;
		else{
			Server_StartEA = getseg(get_screen_ea())->startEA & _Size;
		}
		Client_Update();
		if (RealTime){
			Handler_Time = SetTimer(NULL, 0, 1000, TIMER_Client);
		}
		break;
	}
	return 0;
}