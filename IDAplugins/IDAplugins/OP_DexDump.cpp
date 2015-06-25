#include <Util_LIB.HPP>
/*
*			IDA插件Dex Dump模块
*主要功能.实现
*	1.
*	2.
*/
//以下定义UI和模式枚举
const char ASK_MAIN_UI[] = "STARTITEM  0\n\n"
"<#连续内存# 连续内存DEX:R:32:16:>\n"
"<#非连续内存# 非连续内存:R:32:16:>\n"
"<#Helper# 输出DEX信息:R:32:16:>>\n";
enum{
	MAIN_ConDex,
	MAIN_UnConDex
};
int Mode = 0;
int Dex_Moudle(){
	if (AskUsingForm_c(ASK_MAIN_UI, &Mode) == 0)return -1;
	ea_t mStart = get_screen_ea();
	sval_t mSize = 0;
	if (asklong(&mSize,"输入DEX宽度") == 0)return -1;
	IDA_Dex::IDA_DunmDex(mStart, mSize);
//




	return 0;
}