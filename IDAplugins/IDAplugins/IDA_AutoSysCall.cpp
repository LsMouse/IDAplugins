#define __IDA_AutoSysCall_C_
#include"IDA_Header.H"
#include "./Util_LIB/Linux/__NR_SysCall.H"
const char ASK_SYSCALL_UI[] = "STARTITEM  0\n"
						"输入系统调用的参数\n"
						"<~S~ys Num(Hex):N:32:16::>\n";

/*
*/
void AutoSysCall_Run() {
	int Sys_No = 0;
	ea_t ea = get_screen_ea();
	//自动获取处理
	Sys_No = get_32bit(ea) & 0xFFF;
	if (AskUsingForm_c(ASK_SYSCALL_UI, &Sys_No) == 0)return;
	if (Sys_No >= 0x0F0000) {//__ARM_NR_BASE 
		Sys_No &= 0xFF;
		if (Sys_No > 5) {
			set_cmt(ea, "No ARM CALL", 1);
			set_cmt(ea, "No ARM CALL", 0);
			return;
			set_cmt(ea, Con_ARM_Call[Sys_No], 1);
			set_cmt(ea, Con_ARM_Call[Sys_No], 0);
		}
		else {//__NR_SYSCALL_BASE 
			if (Sys_No > 400) {
				set_cmt(ea, "No Sys CALL", 1);
				set_cmt(ea, "No Sys CALL", 0);
				return;
			}
			set_cmt(ea, Con_SysCall[Sys_No], 1);
			set_cmt(ea, Con_SysCall[Sys_No], 0);
		}
	}
}