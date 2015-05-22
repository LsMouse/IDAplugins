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
	static char* GetCmt(ea_t inAddr, bool rptble){
		char* Out = NULL;
		char* Buf = (char*)Util_Base::Alloc(10240);
		if (get_cmt(inAddr, rptble, Buf, 10240) > 0){
			Out = strdup(Buf);
		}
		free(Buf);
		return Out;
	}
/**
* @See	获取注释数据
*/
	static bool isCmt(ea_t inAddr){
		bool OutBool = FALSE;
		char* _Cmt = GetCmt(inAddr, 0);
		char* _ReCmt = GetCmt(inAddr, 1);
		if ((_Cmt == NULL) && (_ReCmt == NULL))return FALSE;
		char* _OldCmt = GetCmt(inAddr-1, 0);
		char* _OldReCmt = GetCmt(inAddr-1, 1);
		//判断之前数据是否存在
		if ((_OldCmt == NULL) && (_OldReCmt == NULL)){
			OutBool = TRUE;
		}
		else{
			//判断数据是否相同
			if (_Cmt != _OldCmt){
				if (strcmp(_Cmt, _OldCmt) == 0)OutBool = FALSE;
				else OutBool = TRUE;
			}
			else if (_ReCmt != _OldReCmt){
				if (strcmp(_ReCmt, _OldReCmt) == 0)OutBool = FALSE;
				else OutBool = TRUE;
			}
			else if ((strcmp(_Cmt, _OldReCmt) == 0) && (strcmp(_ReCmt, _OldReCmt) == 0)){
				OutBool = FALSE;
			}
			else OutBool = TRUE;
		}
		if (_Cmt != NULL)free(_Cmt);
		if (_ReCmt != NULL)free(_ReCmt);
		if (_OldCmt != NULL)free(_OldCmt);
		if (_OldReCmt != NULL)free(_OldReCmt);
		return OutBool;
	}
/**
* @See		将数据转化成INI节模式
* @Param　	inIni -> INI类
* @Return　	inSecName -> 节名
*/
	void To_Ini(INI* inIni, char* inSecName){
		inIni->addIntValue(inSecName,"StartEA", StartEA);
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
		if (Cmt != NULL)
			set_cmt(inStartEA + StartEA, Cmt, 0);
		if (ReCmt != NULL)
			set_cmt(inStartEA + StartEA, ReCmt, 1);
	}
/**
* @See		初始化_Base_Cmt
*/
	_Base_Cmt(segment_t* inSeg, ea_t inStart){
		StartEA = inStart - inSeg->startEA;
		Cmt = GetCmt(inStart,0);
		ReCmt = GetCmt(inStart, 1);
	}
/**
* @See		初始化_Base_Cmt
* @Param　	inSection -> INI文件 段
*/
	_Base_Cmt(C_INI_Section* inSection){
		if(inSection == NULL)return;
		StartEA = inSection->GetInt("StartEA");
		Cmt = inSection->GetString("Cmt");
		ReCmt = inSection->GetString("ReCmt");
	}
/**
* @See		初始化_Base_Cmt
* @Param　	inIni -> INI类
* @Param　	inSecName -> 段名
*/
	_Base_Cmt(INI* inIni, char* inSecName){
		if (inIni == NULL)return;
		StartEA = inIni->GetIntValue(inSecName, "StartEA");
		Cmt = inIni->GetStrValue(inSecName, "Cmt");
		ReCmt = inIni->GetStrValue(inSecName, "ReCmt");
	}
};

