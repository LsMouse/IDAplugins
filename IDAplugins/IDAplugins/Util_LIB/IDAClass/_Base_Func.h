#include "Util_LIB.H"
class _Base_Func{
public:
	ea_t StartEA;
	char* Name;
	char* Cmt;
	char* ReCmt;
/**
* @See		获取IDA 函数名称
* @Param　	inFun -> IDA函数类
* @Return　	Out_Sec -> 函数名称
*/
	static char* GetFuncName(func_t* inFun){
		char* Out = NULL;
		char* Buf = (char*)Util_Base::Alloc(10240);
		if (get_func_name(inFun->startEA, Buf, 10240) != NULL){
			Out = strdup(Buf);
		}
		free(Buf);
		return Out;
	}
/**
* @See		将数据转化成INI节模式
* @Param　	inIni -> INI类
* @Return　	inSecName -> 节名
*/
	void ToIni(INI* inIni, char* inSecName){
		inIni->addIntValue(inSecName, "StartEA", StartEA);
		if (Name != NULL)
			inIni->addStrValue(inSecName, "Name", Name);
		if (Cmt != NULL)
			inIni->addStrValue(inSecName, "Cmt", Cmt);
		if (ReCmt != NULL)
			inIni->addStrValue(inSecName, "ReCmt", ReCmt);
	}
/**
* @See		初始化_Base_Func
* @Param　	inSeg -> IDA 锻类
* @Param　	inFun -> IDA 函数类
*/
	_Base_Func(segment_t* inSeg,func_t* inFun){
		StartEA = 0;
		Name = Cmt = ReCmt = NULL;
		if (inFun == NULL)return;
		//获取偏移
		StartEA = inFun->startEA - inSeg->startEA;
		//获取名字、注释
		Name = GetFuncName(inFun);
		Cmt = _Base_Cmt::GetCmt(inFun->startEA,0);
		ReCmt = _Base_Cmt::GetCmt(inFun->startEA, 0);
	}
/**
* @See		初始化_Base_Func
* @Param　	inSection -> INI文件 段
*/
	_Base_Func(C_INI_Section* inSection){
		if (inSection == NULL)return;
		StartEA = inSection->GetInt("StartEA");
		Name = inSection->GetString("Name");
		Cmt = inSection->GetString("Cmt");
		ReCmt = inSection->GetString("ReCmt");
	}
};

