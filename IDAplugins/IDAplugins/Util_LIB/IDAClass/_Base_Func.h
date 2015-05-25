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
* @See		获取IDA 函数名称
* @Param　	inFun -> IDA函数类
* @Return　	Out_Sec -> 函数名称
*/
	static char* GetFuncCmt(func_t* inFun,bool rptble){
		if (get_func_cmt(inFun, rptble) != NULL){
			return strdup(get_func_cmt(inFun, rptble));
		}
		return NULL;
	}


/**
* @See		将数据转化成INI节模式
* @Param　	inIni -> INI类
* @Return　	inSecName -> 节名
*/
	void To_Ini(INI* inIni, char* inSecName){
		inIni->addIntValue(inSecName, "StartEA", StartEA);
		if (Name != NULL)
			inIni->addStrValue(inSecName, "Name", Name);
		if (Cmt != NULL)
			inIni->addStrValue(inSecName, "Cmt", Cmt);
		if (ReCmt != NULL)
			inIni->addStrValue(inSecName, "ReCmt", ReCmt);
	}
/**
* @See		将数据加载到IDA内存中
* @Param　	inStartEA -> 段其实地址
*/
	void To_IDAMem(ea_t inStartEA){
		ua_code(inStartEA + StartEA);
		add_func(inStartEA + StartEA, BADADDR);
		if (Cmt != NULL)
			set_cmt(inStartEA + StartEA, Cmt, 0);
		if (ReCmt != NULL)
			set_cmt(inStartEA + StartEA, ReCmt, 1);
		func_t* _func = get_func(inStartEA + StartEA);
		if (_func == NULL)return;
		if (Cmt != NULL)
			set_func_cmt(_func, Cmt, 0);
		if (ReCmt != NULL)
			set_func_cmt(_func, ReCmt, 1);
		if (memcmp(Name, "sub_", 4) == 0)return;
		if (memcmp(Name, "loc_", 4) == 0)return;
		if (memcmp(Name, "_", 1) == 0)return;
		set_name(inStartEA + StartEA, Name, 1);
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
		Cmt = GetFuncCmt(inFun, 0);
		ReCmt = GetFuncCmt(inFun, 1);
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
/**
* @See		初始化_Base_Func
* @Param　	inIni -> INI类
* @Param　	inSecName -> 段名
*/
	_Base_Func(INI* inIni, char* inSecName){
		if (inIni == NULL)return;
		StartEA = inIni->GetIntValue(inSecName, "StartEA");
		Name = inIni->GetStrValue(inSecName, "Name");
		Cmt = inIni->GetStrValue(inSecName, "Cmt");
		ReCmt = inIni->GetStrValue(inSecName, "ReCmt");
	}
};

