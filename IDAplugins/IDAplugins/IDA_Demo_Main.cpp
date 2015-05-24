#define __IDA_DEMO_MAIN_C_
#include"IDA_Header.H"
#include"IDA_Demo_Main.h"
//主面板模式选择，定在全局变量是为能自动保存模式
int Main_Mode = 0;
int _stdcall IDAP_init(void) {
//	Debug_Run(msg("IDA_Demo Run IDAP_init\n"));
//	SetTimer(NULL, 0, 1000, (TIMERPROC)IDA_TIMER);
	_MSG("Test Mode IDA_Demo_Main IDAP_init !\n");
	Main_Mode = 0;
	return PLUGIN_KEEP;
}
void _stdcall IDAP_term(void) {	
	return;
}
//以下定义UI和模式枚举
const char ASK_MAIN_UI[] = "STARTITEM  0\n\n"
	"<#数据导出# ~E~xport:R:32:16:>\n"
	"<#数据导入# ~I~mport:R:32:16:>\n"
	"<#ARM相关功能# ~A~RM:R:32:16:>\n"
	"<#调试# ~D~ebug:R:32:16:>\n"
	"<#注释# ~N~ote:R:32:16:>>\n"
	"<##调试选择##是否打印信息:C>>\n";
enum{
	MAIN_Export,
	MAIN_Import,
	MAIN_ARM,
	MAIN_Debug,
	MAIN_Notes,
}MAIN_MODE_ENUM;
/*
*				模式说明
*	1、MAIN_Export -> 导出模块
*	2、MAIN_Import -> 导入模块
*	3、MAIN_ARM    -> ARM模块
*	4、MAIN_Debug  -> 调试模块
*	5、MAIN_Note   -> 注释模块
*/
void _stdcall IDAP_run(int arg) {
	ushort EnDebug = Debug::GetEnable();
	if (AskUsingForm_c(ASK_MAIN_UI, &Main_Mode, &EnDebug) == 0)return;
	Debug::SetEnable(EnDebug);
	Debug::MSG("EnDebug:%d\n", EnDebug);
	switch (Main_Mode){
	case MAIN_Export:
		Export_Module();
		break;
	case MAIN_Import:
		Arm_Moudle();
		break;
	case MAIN_ARM:
		Arm_Moudle();
		break;
	case MAIN_Debug:
		Debug_Moude();
		break;
	case MAIN_Notes:
		Note_Moudle();
		break;
		return;
	}
} 