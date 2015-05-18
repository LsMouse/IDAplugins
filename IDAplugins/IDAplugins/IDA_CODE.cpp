#define __IDA_CODE_C_
#include"IDA_Header.H"
const char ASK_CODE_UI[] = "Ls CODE \n\n\n"
			"<Change_16BIT:R>\n"
			"<Change_32BIT:R>\n"
			"<Set BaseOff:R>>\n"
			"<Change B $:R>\n"
			"<Change NOP:R>>\n";		
enum{
	MODE_CHANGE_16BIT,
	MODE_CHANGE_32BIT,
	MODE_SETBASE,
};
enum{
	MODE_CHANGE_B,
	MODE_CHANGE_NOP,
};
const char ASK_SETBASE_UI[] = "SetBase\n\n\n"
						"<~A~ddress:N:32:16::>\n";
/*

*/
void IDA_CODE_Run(){
	int mode = 0, mode_Change = 0;
	if (AskUsingForm_c(ASK_CODE_UI, &mode, &mode_Change) == 0)return;
	ea_t ea = get_screen_ea();
	switch (mode){
	case MODE_CHANGE_16BIT:
		ea &= 0xFFFFFFFE; 
		switch (mode_Change){
		case MODE_CHANGE_NOP:
			patch_word(ea, 0xC046);
		break;
		case MODE_CHANGE_B:
			patch_long(ea, 0xE7FE);
		break;
		}
	break;
	case MODE_CHANGE_32BIT:
		ea &= 0xFFFFFFFC;
		switch (mode_Change){
		case MODE_CHANGE_NOP:
			patch_long(ea, 0xE1A00000);
		break;
		case MODE_CHANGE_B:
			patch_long(ea, 0xEAFFFFFE);
		break;
		}
	break;
	case MODE_SETBASE:
		ulong m_i = 0;
		if (AskUsingForm_c(ASK_SETBASE_UI, &m_i) == 0)return;
		ulong set_off = get_first_seg()->startEA;
		msg("set_off:0x%x\n", set_off);
		rebase_program(m_i - set_off, MSF_NOFIX);
	break;
	}


}