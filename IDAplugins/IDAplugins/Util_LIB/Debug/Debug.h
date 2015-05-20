#include "Util_LIB.H"
//
static int Debug_En = 0;
//
class Debug{
public:

	static void SetEnable(int inEn){
		Debug_En = inEn;
	}
	static void MSG(const char *format, ...){
		if (Debug_En == 0)return;
		msg(format);
	}
};

