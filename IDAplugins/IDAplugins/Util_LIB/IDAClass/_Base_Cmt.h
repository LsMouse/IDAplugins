#include "Util_LIB.H"
/*
*	
*/
class _Base_Cmt{
public:
	ea_t StartEA;
	char* Cmt;
	char* ReCmt;
/**
* @See	获取注释数据
*/
	static char* GetCmt(ea_t inAddr,bool rptble){
		char* Out = NULL;
		char* Buf = (char*)Util_Base::Alloc(10240);
		if (get_cmt(inAddr, rptble, Buf, 10240) > 0){
			Out = strdup(Buf);
		}
		free(Buf);
		return Out;
	}
/**
* @See		将数据转化成INI节模式
* @Param　	inName -> 节名字
* @Return　	Out_Sec -> INI数据
*/
	C_INI_Section* ToIniSection(char* inName){
		C_INI_Section* Out_Sec = new C_INI_Section(inName);
		Out_Sec->AddInt("StartEA", StartEA);
		if (Cmt != NULL)
			Out_Sec->AddString("Cmt", Cmt);
		if (ReCmt != NULL)
			Out_Sec->AddString("ReCmt", ReCmt);
		return Out_Sec;
	}
/**
* @See		初始化_Base_Func
*/
	_Base_Cmt(segment_t* inSeg, ea_t inStart){
		StartEA = inStart - inSeg->startEA;
		Cmt = GetCmt(inStart,0);
		ReCmt = GetCmt(inStart, 1);
	}
};

