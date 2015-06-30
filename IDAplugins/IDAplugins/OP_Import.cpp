#define __IDA_Import_C_
#include"IDA_Header.H"
//以下定义UI和模式枚举
const char ASK_Import_UI[] = "Import Options\n"
				"<IDA START:N:32:16::>\n"
				"<IDA END:N:32:16::>\n"
				"<IDA SIZE:N:32:16::>\n"
				"<FILE START:N:32:16::>\n";
/*
* @See 模式说明
*/
int Import_Module(int inFlag){
	char* m_filepath = askfile_c(0, "*.*", "需要导入的文件");
	if (m_filepath == NULL)return -1;
	FILE* mFile = fopen(m_filepath, "rb");
//定位到文件末,获取文件长度
	fseek(mFile, 0, SEEK_END);
	ulong fileSzie = qftell(mFile);
//定位到文件初
	fseek(mFile, 0, SEEK_SET);
//
	ulong Mem_Start = get_screen_ea();
	ulong Mem_End = get_screen_ea() + fileSzie;
	ulong Mem_Size = fileSzie;
	ulong File_Start = 0;
	if (AskUsingForm_c(ASK_Import_UI, &Mem_Start, &Mem_End, &Mem_Size,&File_Start) == 0)return 0;
	if (Mem_Size == 0){
		if (Mem_Start >= Mem_End){
			msg("文件地址配置错误\n");
			return -1;
		}
		Mem_Size = Mem_End - Mem_Start;
	}
//申请内存，并清空
	char* ReadBuf = (char*)malloc(Mem_Size);
	memset(ReadBuf, 0, Mem_Size);
	qfseek(mFile, File_Start, SEEK_SET);
	qfread(mFile, ReadBuf, Mem_Size);
	qfclose(mFile);
//设置IDA内存
	ulong m_i = 0;
	while (m_i < Mem_Size){
		patch_byte(Mem_Start + m_i, ReadBuf[m_i]);
		m_i++;
	}
	return 0;
}