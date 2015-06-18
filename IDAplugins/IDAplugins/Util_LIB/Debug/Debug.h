#include <Util_LIB.HPP>
//
static int Debug_En = TRUE;
//
namespace Util{
	static int GetEnable(){
		return Debug_En;
	}
	static void SetEnable(int inEn){
		Debug_En = inEn;
	}
	static void MSG(const char *format, ...){
		if (GetEnable() == 0)return;
		va_list va;
		va_start(va, format);
		vmsg(format, va);
		va_end(va);
	}
};

