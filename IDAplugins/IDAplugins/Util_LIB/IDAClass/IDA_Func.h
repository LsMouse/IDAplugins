#include "Util_LIB.H"
/*
..								INI文件数据存储说明
..1.在Function_Config节中存放全部配置，比如名字、开始地址、结束地址.
..__________________________________________________________________________________
..[Seg.No.Fun.No] 指向第No个Segment的第No个函数数据
	Name -> 函数名字
	Start_EA -> 函数开始地址
	End_EA ->	函数结束地址
	Cmt -> 不可重复数据
	ReCmt -> 可重复数据
	Code_S -> 开始码(NoUsed)
	Code_E -> 结束码(NoUsed)
..__________________________________________________________________________________
*/
class IDA_Func{
#define IDAF_Name		"Name"
#define IDAF_Start_EA	"Start_EA"
#define IDAF_End_EA		"End_EA"
#define IDAF_Cmt		"Cmt"
#define IDAF_ReCmt		"ReCmt"
public:
	func_t* Fun = NULL;
	ea_t Start_EA = 0;
	ea_t End_EA = 0;
	ulong Code_S = 0;
	ulong Code_E = 0;
	char* Name = NULL;
	char* Cmt = NULL;
	char* ReCmt = NULL;
/*
..	Name : IDA_Func
..	Function :　初始化func_t
*/
	char* getName(){
		if (Fun == NULL)return NULL;
		char* mName = (char*)malloc(1024);
		memset(mName,0,1024);
		get_func_name(Fun->startEA, mName, 1024);
		return mName;
	}
/*
..	Name : getCmt
..	Function :　初始化func_t
*/
	char* getCmt(){
		if (Fun == NULL)return NULL;
		return get_func_cmt(Fun, 0);
	}
/*
..	Name : getReCmt
..	Function :　初始化func_t
*/
	char* getReCmt(){
		if (Fun == NULL)return NULL;
		return get_func_cmt(Fun, 1);
	}
/*
..	Name : Save_Ini
..	Function :　初始化func_t
*/
	void Save_Ini(C_INI* inIni,char* inName) {
		char* mFunSection = strdup(inName);
		inIni->addStrValue(mFunSection, IDAF_Name, Name);
		inIni->addIntValue(mFunSection, IDAF_Start_EA, Start_EA);
		inIni->addIntValue(mFunSection, IDAF_End_EA, End_EA);
		if (Cmt != NULL)
			inIni->addStrValue(mFunSection, IDAF_Cmt, Cmt);
		if (ReCmt != NULL)
			inIni->addStrValue(mFunSection, IDAF_ReCmt, ReCmt);
	}
/*
..	Name : UpToIDA
..	Function :　
*/
	void UpToIDA(ea_t inOld, ea_t inNew) {
		ea_t mThis_Start = Start_EA - inOld + inNew;
		ea_t mThis_End = End_EA - inOld + inNew;
		add_func(mThis_Start, mThis_End);
		func_t* mFun = get_func(mThis_Start);
		if (mFun == NULL)return;
		set_func_cmt(mFun,Cmt,0);
		set_func_cmt(mFun,ReCmt, 1);
		if (memcmp(Name, "sub_",4) == 0)return;
		if (memcmp(Name, "loc_", 4) == 0)return;
		set_name(mThis_Start, Name, 1);
	}
/*
..	Name : IDA_Func
..	Function :　初始化func_t
*/
	IDA_Func(func_t* in_Func){
		Name = (char*)malloc(1024);
		memset(Name,0,1024);
		Fun = in_Func;
		Start_EA = in_Func->startEA;
		End_EA = in_Func->endEA;
		get_func_name(in_Func->startEA, Name, 1024);
		Cmt = get_func_cmt(Fun, 0);
		ReCmt = get_func_cmt(Fun, 1);
	}
/*
..	Name : IDA_Func
..	Function :　初始化func_t
*/
	IDA_Func(ea_t inStart,ea_t inEnd){
		add_func(inStart, inEnd);
		IDA_Func(get_func(inStart));
	}
/*
..	Name : IDA_Func
..	Function :　初始化func_t
*/
	IDA_Func(C_INI* in_Ini, char* inName) {
		char* mSection = strdup(inName);
		Name = in_Ini->GetStrValue(mSection, IDAF_Name);
		Start_EA = in_Ini->GetIntValue(mSection, IDAF_Start_EA);
		End_EA = in_Ini->GetIntValue(mSection, IDAF_End_EA);
		Cmt = in_Ini->GetStrValue(mSection, IDAF_Cmt);
		ReCmt = in_Ini->GetStrValue(mSection, IDAF_ReCmt);
	}
/*
..	Name : IDA_Func
..	Function :　保存之前已有的数据
*/
	IDA_Func(ea_t inStart, ea_t inEnd,int inCodeS,int inCodeE,
			char* inName,char* inCmt,char* inReCmt){
		Start_EA = inStart;
		End_EA = inEnd;
		Code_S = inCodeS;
		Code_E = inCodeE;
		Name = inName;
		Cmt = inCmt;
		ReCmt = inReCmt;
	}
};

class Lis_Fun{
public:
#define	CIDA_FunLen		"FunLen"
#define CIDA_Fun_No		"%s.Fun.%d" 
	Lis<IDA_Func>* ArrayFun = new Lis<IDA_Func>;
/*
..	Name : LoadIDAFunc
..	Function : 从IDA内存中查找
*/
	void LoadIDAFunc(ea_t inStart, ea_t inEnd){
		func_t *m_Func = get_func(inStart);
		if (m_Func == NULL)m_Func = get_next_func(inStart);
		while ((m_Func != NULL) && (inEnd >= m_Func->startEA)){
			IDA_Func* mIDAFunc = new IDA_Func(m_Func);
			ArrayFun->add(mIDAFunc);
			m_Func = get_next_func(m_Func->startEA);
		}
	}
/*
..	Name : Save_Ini
..	Function :　
*/
	void Save_Ini(C_INI* in_Ini,char* inSegName) {
		int mFunLength = 0;
		char* mFunName = (char*)malloc(1024);
		in_Ini->addIntValue(inSegName, CIDA_FunLen, ArrayFun->getLength());
		while (mFunLength < ArrayFun->getLength()) {
			memset(mFunName, 0, 1024);
			sprintf(mFunName, 1024, CIDA_Fun_No, inSegName, mFunLength);
			ArrayFun->Get(mFunLength)->Save_Ini(in_Ini, mFunName);
			mFunLength++;
		}
	}
/*
..	Name : Form_Ini
..	Function:从Ini中读取Segment
..		并添加到ArraySeg中
..		若Seg数据冲突则更新数据
*/
	void Form_Ini(C_INI* in_Ini, char* inName) {
		int m_i = 0;
		int m_FunLen = in_Ini->GetIntValue(inName, CIDA_FunLen);
		char* mFunNo = (char*)malloc(1024);
		while (m_i < m_FunLen) {
			memset(mFunNo, 0, 1024);
			sprintf(mFunNo,1024, CIDA_Fun_No, inName,m_i);
			IDA_Func* mINIFunc = new IDA_Func(in_Ini, mFunNo);
			ArrayFun->add(mINIFunc);
			m_i++;
		}
	}
/*
..	Name : UpToIDA
..	Function :　
*/
	void UpToIDA(ea_t inOld, ea_t inNew) {
		int m_i = 0;
		int m_FunLen = ArrayFun->getLength();
		while (m_i < m_FunLen) {
			ArrayFun->Get(m_i)->UpToIDA(inOld, inNew);
			m_i++;
		}
	}
/*
..	Name : IDALis_Fun
..	Function :　初始化IDALis_Fun
*/
	Lis_Fun(ea_t inStart, ea_t inEnd){
		ArrayFun = new Lis<IDA_Func>();
		LoadIDAFunc(inStart, inEnd);	
	}
/*
..	Name : IDALis_Fun
..	Function :　从INI文件中读取全部数据
*/
	Lis_Fun(C_INI* in_Ini, char* inSecName){
		ArrayFun = new Lis<IDA_Func>();
		Form_Ini(in_Ini, inSecName);
	}
};
