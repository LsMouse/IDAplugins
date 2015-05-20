#define __IDA_DEMO_MAIN_C_
#include"IDA_Header.H"
#include"IDA_Demo_Main.h"
#include"IDA_Timer.H"
/*
// Do checks here to ensure your plug-in is being used within
// an environment it was written for. Return PLUGIN_SKIP if the
// checks fail, otherwise return PLUGIN_KEEP.
*/
int _stdcall IDAP_init(void) {
//	Debug_Run(msg("IDA_Demo Run IDAP_init\n"));
//	SetTimer(NULL, 0, 1000, (TIMERPROC)IDA_TIMER);
	_MSG("Test Mode IDA_Demo_Main IDAP_init !\n");
//	Back = new IDA_Back();
//	msg("File : %s\n", AskUI::GetFileName());
	return PLUGIN_KEEP;
}
/*
// Stuff to do when exiting, generally you'd put any sort
*/
void _stdcall IDAP_term(void) {
	return;
}
/*
// The plugin can be passed an integer argument from[ SWZthe plugins.cfg 
// file. This can be useful when you want the one plug-in to do 
// something different depending on the hot-key pressed or menu
// item selected. 
*/
const char ASK_MAIN_UI[] = "STARTITEM  1\n\n"
						"<通用Code修改:R:32:16::>\n"
						"<导出:R:32:16:>\n"
						"<导入:R:32:16:>\n"
						"<ARM Syscall:R:32:16:>\n"
						"<多窗口调试:R:32:16:>\n"
						"<段备份:R:32:16:>>\n";
const char ASK_BackSeg_UI[] = "STARTITEM  1\n\n"
						"<加载当前段:R:32:16:>\n"
						"<整个压缩成一个段:R:32:16:>\n"
						"<加载段文件:R:32:16:>\n"
						"<保存段文件:R:32:16:>\n"
						"<清数据:R:32:16:>>\n";
enum{
	MAIN_Code,
	MAIN_Export,
	MAIN_Import,
	MAIN_AutoSysCall,
	MAIN_Mul_Debug,
	MAIN_BackSegment,
	MAIN_OVER
}MAIN_MODE_ENUM;
void _stdcall IDAP_run(int arg) {// The "meat" of your plug-in    
	int mMode = 5;
	if (AskUsingForm_c(ASK_MAIN_UI, &mMode) == 0)return;
	switch (mMode){
	case MAIN_Code:
		Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_Code\n"));
		IDA_CODE_Run();
	break;
	case MAIN_Export:
		Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_Export\n"));
		IDA_Export_Run();
	break;
	case MAIN_Import:
		Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_Export\n"));
		Import_Run();
	break;
	case MAIN_AutoSysCall:
		Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_AutoSysCall\n"));
		AutoSysCall_Run();
	break;
	case MAIN_Mul_Debug:
		Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_Mul_Debug\n"));
		Mul_Debug_Run();
	break;
	case MAIN_BackSegment:Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_BackSegment\n"));
		mMode = 0;
/*
..					进入BackSegment模式
..选择模式下的类型
..mMode:
..	0 -> 将当前段的添加到链表中(已有段则更新段数据)
..	1 -> 将当前文件全部加载到一个段中
..	2 -> 加载BSegm文件
..	3 -> 保存BSegm文件
..	4 -> 清全部数据
*/	
/*		if (AskUsingForm_c(ASK_BackSeg_UI, &mMode) == 0)return;
		if (mMode == 0) {
			_MSG("BackSegment this Segment\n");
			segment_t* mSeg = getseg(get_screen_ea());
			Back->addSegment(mSeg);
		}else if (mMode == 1) {
			_MSG("BackSegment All Segment \n");
			int m_i = 0;
			ea_t mStart = getnseg(0)->startEA;
			if (askaddr(&mStart,"起始地址输入") == 0)return;
			while (getnseg(++m_i) != NULL);
			Back->addSegment(getnseg(0)->startEA, getnseg(m_i-1)->endEA);
		}
		else if (mMode == 2) {
			_MSG("BackSegment Add BSegm File\n");
			char* mFile = askfile_c(0, "*.BSegm", "需要导入的文件");
			if (mFile == NULL)return;
			Back->Form_BSegm(mFile);
		}
		else if (mMode == 3) {
			_MSG("BackSegment Save  File\n");
			char* mSaveFile = askfile_c(1, "*.BSegm", "需要导入的文件");
			if (mSaveFile == NULL)return;
			Back->Save_BSegm(mSaveFile);
		}else{
			_MSG("BackSegment Clear Data\n");
			free(Back);
			Back = new IDA_Back();
		}
*/
	default:return;
	}
	return;
} 
/*
	Debug_Run(_MSG("IDA_Debug_ALL Run MAIN_Notes\n"));
	IDA_Note_Run();
*/