#define __IDAOP_Export_C_
#include <Util_LIB.HPP>
/*
*			IDA插件Export模块
*主要功能.
*	1.普通DUMP功能
*	2.文件更新功能
*/
//以下定义UI和模式枚举
const char ASK_EXPORT_UI[] = "Export Module\n\n\n"
			"<#普通DUMP# ~Dump~:R>\n"
			"<#文件更新# ~FileUpdate~:R>"
			"<#连续内存# 连续内存DEX:R:32:16:>\n"
			"<#非连续内存# 非连续内存:R:32:16:>\n"
			"<#Helper# 输出DEX信息:R:32:16:>>\n";
enum{
	MODE_Dump,
	MODE_FileUpdate,
	MODE_ConDex,
	MODE_UnConDex
};
//Dump UI定义
const char ASK_DUMP_UI[] = "Export Dump\n"
				"<~S~tart Mem:N:32:16::>>\n"
				"<~E~nd Mem:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
//FileUpdate UI定义
const char ASK_FILEDUMP_UI[] = "Export ASK_FILEDUMP_UI\n"
				"<~S~tart Addr:N:32:16::>>\n"
				"<~F~ile Start Addr:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
/**
* @See	Common_Dump -> 普通Dump功能
* @Return NULL
*/
int Common_Dump(int inFlag){
	//初始化变量，
	ulong mem_st = get_screen_ea(), mem_ed = getseg(get_screen_ea())->endEA, mszie = mem_ed - mem_st;
	//调用UI,设置内容
	if (AskUsingForm_c(ASK_DUMP_UI, &mem_st, &mem_ed, &mszie) == 0)return -1;
	//判断长度是否改变，长度改变则以长度设置内容为准
	if (mszie == 0){
		if (mem_ed < mem_st)return -1; 
		mszie = mem_ed - mem_st;
	}
	//获取保存文件路径
	char* m_filepath = askfile_c(1, "*.*", "保存为");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "wb");
	char* m_buf = (char*)malloc(mszie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	//写入文件
	qfwrite(mFile, m_buf, mszie);
	fclose(mFile);
	//回收内存
	free(m_buf);
	return 0;
}
/**
* @See	File_Dump -> 文件Dump更新功能
* @Return NULL
*/
int File_Dump(int inFlag){
	//初始化变量，
	ulong mem_st = get_screen_ea(), file_st = mem_st, mszie = getseg(get_screen_ea())->endEA - get_screen_ea();
	if (AskUsingForm_c(ASK_FILEDUMP_UI, &mem_st, &file_st, &mszie) == 0)return -1;
	if (mszie == 0)return -1;
	char* m_filepath = askfile_c(0, "*.*", "打开文件");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "rb+");
	fseek(mFile, 0, SEEK_END); //定位到文件末 
	ulong fileSzie = qftell(mFile);
	fseek(mFile, 0, SEEK_SET); //定位到文件初
	//判断原本文件大，还是我们要更新的文件要大
	ulong _AllocMem = fileSzie;
	if (fileSzie <(file_st + mszie)){
		_AllocMem = file_st + mszie;
	}
	char* m_buf = (char*)malloc(_AllocMem);
	memset(m_buf, 0, _AllocMem);
	qfread(mFile, m_buf, fileSzie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + file_st + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	char* newFilename = (char*)malloc(1024); memset(newFilename, 0, 1024);
	sprintf(newFilename, 1024, "%s.dump", m_filepath);
	FILE* mSaveFile = fopen(newFilename, "wb+");
	//写入数据
	qfwrite(mSaveFile, m_buf, _AllocMem);
	fclose(mSaveFile);
	fclose(mFile);
	//回收内存
	free(newFilename);
	free(m_buf);
	return 0;
}
/*
*				模式说明
*	1、MODE_Dump -> 普通DUMP
*	2、MODE_FileUpdate -> 文件更新
*/
int Export_Module(int inFlag){
	static int Main_Mode = 0;
	if (inFlag != Flag_Again)
		if(AskUsingForm_c(ASK_EXPORT_UI, &Main_Mode) == 0)
			return -1;
	if (MODE_Dump == Main_Mode){
		Common_Dump(inFlag);
	}
	else if (MODE_FileUpdate == Main_Mode){
		File_Dump(inFlag);
	}
	return 0;
}
/*
*/