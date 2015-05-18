#include "Util_LIB.H"
/*
..			IDA Segment
..
*/
class Segment{
public:
//
	SingLink<Comment>* Cmt = new SingLink<Comment>;
	List<Func_t*>* Func = new List<Func_t*>;
	Lis_Fun* Fun = NULL;
	int Fun_Length = 0; 
	int Cmt_Length = 0;
/*
..Name : IniFileToSegment
..Comment:将INI文件里面的数据初始化成Segment文件数据
*/
	void SaveToINIFile(char* in_path){	
		_MSG("SaveToINIFile @ %s", in_path);
	//	C_INI* mIni = Fun->toINI(in_path);
	//	mIni->SaveIni(in_path);
	}
/*
..Name : addSegment
..Comment: 将segment_t里
*/
	void addSegment(segment_t* in_Seg){
			
	}
/*
..Name : IDATToSegment
..Comment: 将segment_t里
*/
	void IDATToSegment(segment_t* in_Seg){


	}
/*
..Name : IniFileToSegment
..Comment:将INI文件里面的数据初始化成Segment文件数据
..	
*/
	void IniFileToSegment(char* in_IniFile){

	}
/*
..Name : Segment(char*)
..Input : in_IniFile -> INI文件路径
..Comment: 传入INI文件路径，从INI文件中读取Segment数据
*/
	Segment(char* in_IniFile){
		C_INI* Ini = new C_INI(in_IniFile);
		Ini->load();
		//Fun = new Lis_Fun(Ini);
	}
/*
..Name : Segment(segment_t*)
..Comment:传入需要初始化段
*/
	Segment(segment_t* in_Seg){
		Segment();
		Fun = new Lis_Fun(in_Seg->startEA, in_Seg->endEA);
		if (Fun == NULL)return;
		_MSG("Segment @Length:%d\n", Fun->ArrayFun->getLength());	
	}
/*
..Name : Segment()
..Comment:初始化Segment，将Segment内部数据全部清除
*/
	Segment(){
		Cmt = new SingLink<Comment>;
		Func = new List<Func_t*>;
		Fun_Length = 0;
		Cmt_Length = 0;
	}
};
