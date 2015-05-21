#include "__NR_SysCall.H"
/*
*			
*主要功能.
*	1.获取SysCall的名称
*/
class SysCall{
public:
	static const char* getName(int inSysNo){
		if (inSysNo >= 0x0F0000) {
			//__ARM_NR_BASE 
			inSysNo &= 0xFF;
			return Con_ARM_Call[inSysNo];
		}
		else {
			//__NR_SYSCALL_BASE 
			if (inSysNo > 400)return NULL;
			return Con_ARM_Call[inSysNo];
		}
		return NULL;
	}
};