#include <Util_LIB.H>
/*
*			IDA插件Note模块
*主要功能.
*	1.注释保存
*/
//以下定义UI和模式枚举
const char ASK_ARM_UI[] = "Note Options\n\n\n"
						"<#ARM Code设置# ~C~ode:R>\n"
						"<#ARM 系统调用注释# ~S~ysCall:R>>\n";
enum{
	MODE_ARMOP_Code,
	MODE_ARMOP_SysCall,
};