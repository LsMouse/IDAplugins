#define __IDA_Import_C_
#include"IDA_Header.H"
const char ASK_Import_UI[] = "STARTITEM  0\n"
				"設置DUMP地址,当SIZE为0时才会使用END\n"
				"<IDA START:N:32:16::>\n"
				"<IDA END:N:32:16::>\n"
				"<IDA SIZE:N:32:16::>\n"
				"<FILE START:N:32:16::>\n";

void Import_Run(){
	char* m_filepath = askfile_c(0, "*.*", "需要导入的文件");
	if (m_filepath == NULL)return;
	FILE* mFile = fopen(m_filepath, "rb");
	fseek(mFile, 0, SEEK_END); //定位到文件末 
	ulong fileSzie = qftell(mFile);
	fseek(mFile, 0, SEEK_SET); //定位到文件初
//
	ulong Mem_Start = get_screen_ea();
	ulong Mem_End = get_screen_ea() + fileSzie;
	ulong Mem_Size = fileSzie;
	ulong File_Start = 0;
	if (AskUsingForm_c(ASK_Import_UI, &Mem_Start, &Mem_End, &Mem_Size,&File_Start) == 0)return;
	if (Mem_Size == 0){
		if (Mem_Start >= Mem_End){
			msg("文件地址配置错误\n");
			return;
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
}