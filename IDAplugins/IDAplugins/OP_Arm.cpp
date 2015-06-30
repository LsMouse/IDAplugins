#define ___NR_SysCall_H_
#include <Util_LIB.HPP>
/*
*			IDA插件ARM选项
*主要功能.
*	1.ARM Code设置
*	2.ARM 系统调用设置
*/
//以下定义UI和模式枚举
const char ASK_ARM_UI[] = "ARM Options\n\n\n"
			"<#ARM Code设置# ~C~ode:R>\n"
			"<#ARM 系统调用注释# ~S~ysCall:R>>\n";
enum{
	MODE_ARMOP_Code,
	MODE_ARMOP_SysCall,
};
//Code定义
const char ASK_CODE_UI[] = "ARM CODE\n\n\n"
			"<#Thumb# Code_16Bit JMP $:R>\n"
			"<#Thumb# Code_16Bit NOP:R>\n"
			"<#ARM# Code_32Bit JMP $:R>\n"
			"<#ARM# Code_32Bit NOP:R>\n"
			"<#Thumb# Mode 16Bit:R>\n"
			"<#ARM# Mode 32Bit:R>>\n";
enum{
	MODE_ARMOPC16_JMP,
	MODE_ARMOPC16_NOP,
	MODE_ARMOPC32_JMP,
	MODE_ARMOPC32_NOP,
};
//SysCallUI定义
const char ASK_SYSCALL_UI[] = "STARTITEM  0\n"
				"输入系统调用的参数\n"
				"<~S~ys Num(Hex):N:32:16::>\n";

/*
*				模式说明
*	1、MODE_ARMOP_Code -> 使用ARM指令修改CODE
*	2、MODE_ARMOP_SysCall -> 注释系统调用
*/
int Arm_Moudle(int inFlag){
	static int Mode_Bit = MODE_ARMOPC32_JMP;
	static int ModeOption = 0;
	ea_t _ThisEa = get_screen_ea();
	if (inFlag != Flag_Again)
		if (AskUsingForm_c(ASK_ARM_UI, &ModeOption) == 0)
			return NULL;
	if (MODE_ARMOP_Code == ModeOption){
		if (inFlag != Flag_Again)
			if (AskUsingForm_c(ASK_CODE_UI, &Mode_Bit) == 0)
				return NULL;
		if (MODE_ARMOPC16_JMP == Mode_Bit){
			_ThisEa &= 0xFFFFFFFE;
			patch_long(_ThisEa, 0xE7FE);
		}
		else if (MODE_ARMOPC16_NOP == Mode_Bit){
			_ThisEa &= 0xFFFFFFFE;
			patch_word(_ThisEa, 0xC046);
		}
		else if (MODE_ARMOPC32_JMP == Mode_Bit){
			_ThisEa &= 0xFFFFFFFC;
			patch_long(_ThisEa, 0xEAFFFFFE);
		}
		else if (MODE_ARMOPC32_NOP == Mode_Bit){
			_ThisEa &= 0xFFFFFFFC;
			patch_long(_ThisEa, 0xE1A00000);
		}
	}
	else if(MODE_ARMOP_SysCall == ModeOption){
		//自动获取，
		ulong Sys_No = get_32bit(get_screen_ea()) & 0xFFF;
		if (AskUsingForm_c(ASK_SYSCALL_UI, &Sys_No) == 0)return 0;
		if (SysCall::getName(Sys_No) != NULL){
			set_cmt(get_screen_ea(), SysCall::getName(Sys_No), 1);
		}
	}
	return NULL;
}
