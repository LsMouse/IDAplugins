#include <Util_LIB.HPP>
namespace IDA_Note{
class _Base_Bpt{
public:
	ulong Flags;
	ulong Size;
	ulong Type;
	ea_t StartEA;
/**
* @See		将数据转化成INI节模式
* @Param　	inIni -> INI类
* @Return　	inSecName -> 节名
*/
	void To_Ini(INI* inIni, char* inSecName){
		inIni->addIntValue(inSecName, "StartEA", StartEA);
		inIni->addIntValue(inSecName, "Flags", Flags);
	}
/**
* @See		将数据加载到IDA内存中
* @Param　	inStartEA -> 段其实地址
*/
	void To_IDAMem(ea_t inStartEA){
		if (check_bpt(inStartEA + StartEA) != BPTCK_NONE)return;
		add_bpt(inStartEA + StartEA);
	}
/**
* @See		初始化_Base_Cmt
*/
	_Base_Bpt(segment_t* inSeg, ea_t inStart){
		StartEA = inStart - inSeg->startEA;
		Flags = check_bpt(inStart);
	}
/**
* @See		初始化_Base_Cmt
* @Param　	inSection -> INI文件 段
*/
	_Base_Bpt(C_INI_Section* inSection){
		if (inSection == NULL)return;
		StartEA = inSection->GetInt("StartEA");
		Flags = inSection->GetInt("Flags");
	}
/**
* @See		初始化_Base_Cmt
* @Param　	inIni -> INI类
* @Param　	inSecName -> 段名
*/
	_Base_Bpt(INI* inIni, char* inSecName){
		if (inIni == NULL)return;
		StartEA = inIni->GetIntValue(inSecName, "StartEA");
		Flags = inIni->GetIntValue(inSecName, "Flags");
	}
};
};

