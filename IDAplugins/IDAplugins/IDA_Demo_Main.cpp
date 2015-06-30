#define __IDA_DEMO_MAIN_C_
#include"IDA_Header.H"
#include"IDA_Demo_Main.h"
using namespace Util;
int _stdcall IDAP_init(void) {
//	Debug_Run(msg("IDA_Demo Run IDAP_init\n"));
//	SetTimer(NULL, 0, 1000, (TIMERPROC)IDA_TIMER);
	_MSG("Test Mode IDA_Demo_Main IDAP_init !\n");
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
	"<#注释# ~N~ote:R:32:16:>\n"
	"<#Llvm Run# ~L~lvm 花指令自动运行:R:32:16:>\n"
	"<#上一次程序运行# ~O~ld Mode:R:32:16:>>"
	"<##调试选择##是否打印信息:C>>\n";
enum{
	MAIN_Export,
	MAIN_Import,
	MAIN_ARM,
	MAIN_Debug,
	MAIN_Notes,
	MAIN_LlvmRun,
	MAIN_OldMode
}MAIN_MODE_ENUM;
/*
*				模式说明
*	1、MAIN_Export -> 导出模块
*	2、MAIN_Import -> 导入模块
*	3、MAIN_ARM    -> ARM模块
*	4、MAIN_Debug  -> 调试模块
*	5、MAIN_Note   -> 注释模块
*	6、MAIN_LlvmRun -> Llvm调试
*	7、Old_Mode	   ->最后运行模式
*/
void _stdcall IDAP_run(int arg) {
	//主面板模式选择，定在全局变量是为能自动保存模式
	static int Main_Mode = 0;
	int mMode = MAIN_OldMode;
	ushort EnDebug = Util::GetEnable();
	if (AskUsingForm_c(ASK_MAIN_UI, &mMode, &EnDebug) == 0)return;
	Util::SetEnable(EnDebug);
	//设置运行模式和运行状态
	if (mMode != MAIN_OldMode){
		Main_Mode = mMode;
		mMode = 0;
	}
	else{
		mMode = Flag_Again;
	}
	//选择子模块
	if (MAIN_Export == Main_Mode){
		Export_Module(mMode);
	}
	else if (MAIN_Import == Main_Mode){
		Import_Module(mMode);
	}
	else if (MAIN_ARM == Main_Mode){
		Arm_Moudle(mMode);
	}
	else if (MAIN_Debug == Main_Mode){
		Debug_Moude(mMode);
	}
	else if (MAIN_Notes == Main_Mode){
		Note_Moudle(mMode);
	}
	else if (MAIN_LlvmRun == Main_Mode){
		LlvmRun_Moudle(mMode);
	}
} 