#include "Util_LIB.H"
//
static int Debug_En = 0;
//
class Debug{
public:
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

