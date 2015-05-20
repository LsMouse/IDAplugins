#include <Util_LIB.H>
//
class Debug{
public:
static bool En;
	static void SetEnable(bool inEn){
		En = inEn;
	}
	static void MSG(const char *format, ...){
		if (!En)return;
		msg(format);
	}
};

