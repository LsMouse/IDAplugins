#define __IDA_Export_C_
#include"IDA_Header.H"
const char ASK_EXPORT_UI[] = "Ls EXPORT \n\n\n"
			"<普通DUMP:R>\n"
			"<文件更新DUMP:R>>\n";
enum{
	MODE_DUMP,
	MODE_FILEDUMP,
	MODE_DEBUG_PRINT
};
const char ASK_DUMP_UI[] = "STARTITEM  0\n"
				"設置DUMP地址\n"
				"<~S~tart Mem:N:32:16::>>\n"
				"<~E~nd Mem:N:32:16::>>\n"
				"<~S~ize:N:32:16::>>\n";
const char ASK_FILEDUMP_UI[] = "STARTITEM  0\n"
			"設置DUMP地址\n"
			"<~S~tart Addr:N:32:16::>>\n"
			"<~F~ile Start Addr:N:32:16::>>\n"
			"<~S~ize:N:32:16::>>\n";
/*
*/
void DUMP_Run(){
	ulong mem_st = get_screen_ea(), mem_ed = getseg(get_screen_ea())->endEA, mszie = mem_ed - mem_st;
	if (AskUsingForm_c(ASK_DUMP_UI, &mem_st, &mem_ed, &mszie) == 0)return;
	if (mszie == 0){
		if (mem_ed < mem_st)return;
		mszie = mem_ed - mem_st;
	}
	char* m_filepath = askfile_c(1, "*.*", "保存为");
	if (m_filepath == NULL)return;
	FILE* mFile = fopen(m_filepath, "wb");
	char* m_buf = (char*)malloc(mszie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	qfwrite(mFile, m_buf, mszie);
	fclose(mFile);
}
/*
*/
void FILEDUMP_Run(){
	ulong mem_st = get_screen_ea(), file_st = get_screen_ea() - getseg(get_screen_ea())->startEA, mszie = getseg(get_screen_ea())->endEA - get_screen_ea();
	if (AskUsingForm_c(ASK_FILEDUMP_UI, &mem_st, &file_st, &mszie) == 0)return;
	if (mszie == 0)return;
	char* m_filepath = askfile_c(0, "*.*", "打开文件");
	if (m_filepath == NULL)return;
	FILE* mFile = fopen(m_filepath, "rb");
	fseek(mFile, 0, SEEK_END); //定位到文件末 
	ulong fileSzie = qftell(mFile);
	fseek(mFile, 0, SEEK_SET); //定位到文件初
	char* m_buf = (char*)malloc(fileSzie);
	qfread(mFile, m_buf, fileSzie);
	ulong m_i = 0;
	while (m_i < mszie){
		*(m_buf + file_st + m_i) = get_full_byte(mem_st + m_i);
		m_i++;
	}
	char* newFilename = (char*)malloc(1024); memset(newFilename, 0, 1024);
	sprintf(newFilename, 1024, "%s.dump", m_filepath);
	FILE* mSaveFile = fopen(newFilename, "wb");
	qfwrite(mSaveFile, m_buf, fileSzie);
	fclose(mSaveFile);
	fclose(mFile);
	free(newFilename);
	free(m_buf);
}
/*
*/
void IDA_Export_Run(){
	int mode = 0;
	if (AskUsingForm_c(ASK_EXPORT_UI, &mode) == 0)return;
	switch (mode){
	case MODE_DUMP:
		Debug_Run(msg("IDA_Debug_ALL IDA_Export_Run MODE_DUMP\n"));
		DUMP_Run();

	break;
	case MODE_FILEDUMP:
		Debug_Run(msg("IDA_Debug_ALL IDA_Export_Run MODE_FILEDUMP\n"));
		FILEDUMP_Run();
	break;
	default:return;
	}
}
/*
*/