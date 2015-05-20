#include "Util_LIB.H"
//
class Debug{
public:
static int En;
	static void SetEnable(int inEn){
		En = inEn;
	}
	static void MSG(const char *format, ...){
		if (En == 0)return;
		msg(format);
	}
};

