#include <Util_LIB.H>
/*
*			IDA插件Note模块
*主要功能.
*	1、MODE_NOTE_UP -> 将已加载的段全部更新,并保存成文件
*	2、MODE_NOTE_ADD -> 加载当前段
*	3、MODE_NOTE_FILE -> 从文件中加载
*	4、MODE_NOTE_SAVE -> 保存成文件
*/
//以下定义UI和模式枚举
int NoteMode = 0;
HANDLE HandlerNo = 0;
boolean MulThread = FALSE;
boolean WorkIng = FALSE;
char* SaveFile = NULL;
char* LoadFile = NULL;
Online_Save OnSave;
const char ASK_NOTE_UI[] = "Note Options\n\n\n"
						"<#将已有的段全部更新,并保存成文件# ~U~pAll:R>\n"
						"<#加载当前段# ~A~ddSegment:R>\n"
						"<#从文件中加载# ~F~orm File:R>\n"
						"<#保存成文件# ~S~ave File:R>\n"
						"<#打印段数据# ~P~rintf :R>>\n"
						"<##配置选择##是否启动多线程处理:C>>\n";
enum{
	MODE_NOTE_UP,
	MODE_NOTE_ADD,
	MODE_NOTE_FILE,
	MODE_NOTE_SAVE,
	MODE_NOTE_PRINTF,
};
/*

*/
void RunMode(int inMode){
	ea_t _ea = get_screen_ea();
	char* _AutoBuf = NULL;
	switch (inMode){
	case MODE_NOTE_UP:
		if (OnSave.Seg.GetLength() == 0)return;
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "保存注释文件");
		_AutoBuf = (char*)Util_Base::Alloc(1024);
		OnSave.UpAllSegment();
		sprintf(_AutoBuf, 1024, "%s_Auto", SaveFile);
		OnSave.Save(_AutoBuf);
		free(_AutoBuf);
	break;
	case MODE_NOTE_ADD:
		OnSave.AddSegment(getseg(_ea));
	break;
	case MODE_NOTE_FILE:
		if (!MulThread)	LoadFile = askfile_c(1, "*.ini", "导入注释文件");
		if (LoadFile == NULL)return;
		OnSave.Online_Load(LoadFile);
	break;
	case MODE_NOTE_SAVE:
		if (!MulThread)	SaveFile = askfile_c(1, "*.ini", "保存注释文件");
		OnSave.Save(SaveFile);
	break;
	case MODE_NOTE_PRINTF:


	break;
	}

}
/**
* @See	WINAPI多线程
*/
DWORD WINAPI Mul_Hander(LPVOID lpParam){
	Debug::MSG("Mul_Hander(%d)@ is Run \n", NoteMode);
	RunMode(NoteMode);
	WorkIng = FALSE;
	Debug::MSG("Mul_Hander(%d)@ is End \n", NoteMode);
	CloseHandle(HandlerNo);
	return 0;
}
/*
*模式说明
*	1、MODE_NOTE_UP -> 将已加载的段全部更新,并保存成文件
*	2、MODE_NOTE_ADD -> 加载当前段
*	3、MODE_NOTE_FILE -> 从文件中加载
*	4、MODE_NOTE_SAVE -> 保存成文件
*配置说明
*	0x01 ->	是否启用多线程
*/
int Note_Moudle(){
	int _Config = MulThread;
	if (WorkIng){
		_MSG("之前操作未处理完毕，请稍后再试\n");
		return 0;
	}
	if (AskUsingForm_c(ASK_NOTE_UI, &NoteMode,&_Config) == 0)return -1;
	MulThread = _Config;
	if (_Config == 0){
		Debug::MSG("不启用多线程\n");
		MulThread = FALSE;
		RunMode(NoteMode);
	}
	else{
		Debug::MSG("启用多线程\n");
		if (NoteMode == MODE_NOTE_SAVE){
			SaveFile = askfile_c(1, "*.ini", "保存注释文件");
			if (SaveFile == NULL)return 0;
		}else if (NoteMode == MODE_NOTE_FILE){
			LoadFile = askfile_c(0, "*.ini", "导入注释文件");
			if (LoadFile == NULL)return 0;
		}
		else if (NoteMode == MODE_NOTE_UP){
			if (SaveFile == NULL){
				SaveFile = askfile_c(1, "*.ini", "保存注释文件");
				if (SaveFile == NULL)return 0;
			}
		}
		MulThread = TRUE;
		WorkIng = TRUE;
		HandlerNo = CreateThread(NULL, 0, Mul_Hander, NULL, 0, NULL);
	}
	return 0;
}