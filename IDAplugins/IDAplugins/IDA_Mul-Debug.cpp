#define __IDA_MUL_DEBUG_C_
#include"IDA_Header.H"
#include"IDA_Mul-Debug.H"
const char ASK_MUL_DEBUG_UI[] = "Ls Mul-Debug\n\n\n"
						"<Read_EA:R:32:16:>\n"
						"<Save_EA:R:32:16::>>\n";
enum{
	MODE_MULD_READ,
	MODE_MULD_SAVE,
};
/*
*/
void Mul_Debug_Run(){
	int mode = 0;
	if (AskUsingForm_c(ASK_MUL_DEBUG_UI, &mode) == 0)return;
	switch (mode){
	case MODE_MULD_READ:
		Debug_Run(msg("IDA_Debug_ALL Mul_Debug_Run MODE_MULD_READ\n"));
		jumpto(Save_Ea + getseg(get_screen_ea())->startEA);
		break;
	case MODE_MULD_SAVE:
		Debug_Run(msg("IDA_Debug_ALL Mul_Debug_Run MODE_MULD_SAVE\n"));
		Save_Ea = get_screen_ea() - getseg(get_screen_ea())->startEA;
		break;
	default:return;
	}
}