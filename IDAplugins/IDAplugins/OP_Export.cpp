#define __IDAOP_Export_C_
#include <Util_LIB.H>
/*
*			IDA插件Export模块
*主要功能.
*	1.普通DUMP功能
*	2.文件更新功能
*/
//以下定义UI和模式枚举
const char ASK_EXPORT_UI[] = "Export Module\n\n\n"
			"<#普通DUMP# ~Dump~:R>\n"
			"<#文件更新# ~FileUpdate~:R>>\n";
enum{
	MODE_Dump,
	MODE_FileUpdate,
};
//Dump UI定义
const char ASK_DUMP_UI[] = "Export Dump\n"
				"設置DUMP地址\n"
				"<~S~tart Mem:N:32:16::>>\n"
				"<~E~nd Mem:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
//FileUpdate UI定义
const char ASK_FILEDUMP_UI[] = "Export FileUpdate\n"
			"設置DUMP地址\n"
			"<~S~tart Addr:N:32:16::>>\n"
			"<~F~ile Start Addr:N:32:16::>>\n"
			"<~S~ize:N:32:16::>>\n";
/*
*				Dump功能
*/
void Dump(){
	//初始化变量，
	ulong mem_st = get_screen_ea(), mem_ed = getseg(get_screen_ea())->endEA, mszie = mem_ed - mem_st;
	//调用UI,设置内容
	if (AskUsingForm_c(ASK_DUMP_UI, &mem_st, &mem_ed, &mszie) == 0)return;
	//判断长度是否改变，长度改变则以长度设置内容为准
	if (mszie == 0){
		if (mem_ed < mem_st)return;
		mszie = mem_ed - mem_st;
	}
	//获取保存文件路径
	char* m_filepath = askfile_c(1, "*.*", "保存为");
	if (m_filepath == NULL)return;
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
}
/*
*			FileUpdate
*/
void FileUpdate(){
	//初始化变量，
	ulong mem_st = get_screen_ea(), file_st = mem_st, mszie = getseg(get_screen_ea())->endEA - get_screen_ea();
	if (AskUsingForm_c(ASK_FILEDUMP_UI, &mem_st, &file_st, &mszie) == 0)return;
	if (mszie == 0)return;
	char* m_filepath = askfile_c(0, "*.*", "打开文件");
	if (m_filepath == NULL)return;
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
}
/*
*				模式说明
*	1、MODE_Dump -> 普通DUMP
*	2、MODE_FileUpdate -> 文件更新
*/
int Export_Module(){
	int mode = 0;
	if (AskUsingForm_c(ASK_EXPORT_UI, &mode) == 0)return -1;
	switch (mode){
	case MODE_Dump:
		Dump();
	break;
	case MODE_FileUpdate:
		FileUpdate();
	break;
	}
	return 0;
}
/*
*/