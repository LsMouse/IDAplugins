#include "Util_LIB.H"
/*
..实现在调试模式下，保存当前函数名称、注释等内容，显示重新链接程序时导致注释丢失等。
..保存数据以INI文件格式保存,具体格式如下．．
..__________________________________________________________________________________
..[Seg.No] -> 指向第No个Segment数据
	Name ->Segment 名字
	Start_EA ->	段开始地址
	End_EA ->	段结束地址
	FunLen ->   函数个数
	CmtLen ->	注释个数
	CheckCode -> Segment 校验码
..__________________________________________________________________________________
*/
class IDA_Seg{
#define	CIDA_SEG_Name		"Name"
#define CIDA_SEG_Start		"Start_EA"
#define CIDA_SEG_End		"End_EA"
#define CIDA_SEG_Check		"CheckCode"
public:
	segment_t* Seg = NULL;
	Lis_Fun* Fun = NULL;
	Lis_Cmt* Cmt = NULL;
	char* Name = NULL;
	ea_t Start_EA = 0;
	ea_t End_EA = 0;
	ulong CheckCode = 0;
/*
..	Name : ReadCheckCode
..	Function : 读取每个Seg特殊计算标志(前0x100的和)
*/
	ulong ReadCheckCode(ea_t inStart) {
		ulong OutMark = 0;
		ulong m_i = 0;
		while (m_i < 0x100){
			OutMark += get_full_byte(inStart + m_i);
			m_i++;
		}
		return OutMark;
	}
/*
..	Name : UpToIDA
..	Function :　
*/
	void UpToIDA() {
		/*查找相同的seg*/
		int m_i = 0;
		ea_t mSegLen = End_EA - Start_EA;
		while (getnseg(m_i) != NULL) {
			segment_t* mseg = getnseg(m_i);
			ea_t m_Len = mseg->endEA - mseg->startEA;
			if (CheckCode == ReadCheckCode(mseg->startEA)) {
				Fun->UpToIDA(Start_EA,mseg->startEA);
				Cmt->UpToIDA(Start_EA,mseg->startEA);
				return;
			}
			if (CheckCode == ReadCheckCode(mseg->startEA+ Start_EA)) {
				Fun->UpToIDA(Start_EA, mseg->startEA);
				Cmt->UpToIDA(Start_EA, mseg->startEA);
				return;
			}
			m_i++;
		}
	}
/*
..	Name : Save_Ini
..	Function :　
*/
	void Save_Ini(C_INI* in_Ini,char* inName){
		char* mSegSection = strdup(inName);
		in_Ini->addSection(mSegSection);
		in_Ini->addStrValue(mSegSection, CIDA_SEG_Name, Name);
		in_Ini->addIntValue(mSegSection, CIDA_SEG_Start, Start_EA);
		in_Ini->addIntValue(mSegSection, CIDA_SEG_End, End_EA);
		in_Ini->addIntValue(mSegSection, CIDA_SEG_Check, CheckCode);
		Fun->Save_Ini(in_Ini, mSegSection);
		Cmt->Save_Ini(in_Ini, mSegSection);
	}
/*
..	Name : IDA_Seg(segment_t* inSeg)
..	Function :　
*/
	IDA_Seg(segment_t* inSeg) {
		char* nameSeg = (char*)malloc(1024);
		Name = (char*)malloc(1024);
		memset(nameSeg, 0, 1024);
		memset(Name, 0, 1024);
		get_segm_name(inSeg, nameSeg,1024);
		sprintf(Name,1024,"%s.%s", AskUI::GetFileName(), nameSeg);
		Start_EA = inSeg->startEA;
		End_EA = inSeg->endEA;
		CheckCode = ReadCheckCode(Start_EA);
		Fun = new Lis_Fun(inSeg->startEA, inSeg->endEA);
		Cmt = new Lis_Cmt(inSeg->startEA, inSeg->endEA);
	}
/*
..	Name : IDA_Seg(segment_t* inSeg)
..	Function :　
*/
	IDA_Seg(char* inName,ea_t inStart,ea_t inEnd) {
		Name = strdup(inName);
		Start_EA = inStart;
		End_EA = inEnd;
		CheckCode = ReadCheckCode(inStart);
		Fun = new Lis_Fun(inStart, inEnd);
		Cmt = new Lis_Cmt(inStart, inEnd);
	}
/*
..	Name : IDA_Seg(C_INI* inIni,char* inSegName)
..	Function : 将inIni中数据的inSegName为起始端获取到IDA中
*/
	IDA_Seg(C_INI* inIni,char* inSegName) {
		Name = inIni->GetStrValue(inSegName, CIDA_SEG_Name);
		Start_EA = inIni->GetIntValue(inSegName, CIDA_SEG_Start);
		End_EA = inIni->GetIntValue(inSegName, CIDA_SEG_End);
		CheckCode = inIni->GetIntValue(inSegName, CIDA_SEG_Check);
		Fun = new Lis_Fun(inIni, inSegName);
		Cmt = new Lis_Cmt(inIni, inSegName);
	}
};
/*
..						IDA段集合链表
*/
class Lis_Seg{
#define CIDA_Config		"Config"
#define CIDA_Seg_Len	"SegLen"
#define CIDA_Seg_No		"Seg.%d" 
public:
	Lis<IDA_Seg>* ArraySeg = new Lis<IDA_Seg>;
/*
..	Name : addSeg
..	Function :　添加segment_t段到列表中
*/
	void addSeg(ea_t inStart, ea_t inEnd) {
		ArraySeg->add(new IDA_Seg(AskUI::GetFileName(), inStart, inEnd));
	}
/*
..	Name : addSeg
..	Function :　添加segment_t段到列表中
*/
	void addSeg(segment_t* inSeg) {
		int m_Length = ArraySeg->getLength();
		int m_i = 0;	
		IDA_Seg* inIDASeg = new IDA_Seg(inSeg);
		while (m_i < m_Length) {
			IDA_Seg* mSeg = ArraySeg->Get(m_i);
			if (inIDASeg->CheckCode == mSeg->CheckCode) {
				ArraySeg->Set(m_i, inIDASeg);
				return;
			}
			m_i++;
		}
		ArraySeg->add(new IDA_Seg(inSeg));
	}
/*
..	Name : addSeg
..	Function :　添加segment_t段到列表中
*/
	void addSeg(IDA_Seg* inSeg) {
		int m_Length = ArraySeg->getLength();
		int m_i = 0;
		while (m_i < m_Length) {
			IDA_Seg* mSeg = ArraySeg->Get(m_i);
			if (inSeg->CheckCode == mSeg->CheckCode) {
				ArraySeg->Set(m_i, inSeg);
				return;
			}
			m_i++;
		}
		ArraySeg->add(inSeg);
	}
/*
..	Name : Save_Ini
..	Function :　保存SEG全部列表
*/
	void Save_Ini(C_INI* in_Ini) {
		int m_i = 0;
		char* mSegName = (char*)malloc(1024);
		int mSegLength = ArraySeg->getLength();
		in_Ini->addIntValue(CIDA_Config, CIDA_Seg_Len, mSegLength);
		while (m_i < mSegLength) {
			memset(mSegName, 0, 1024);
			sprintf(mSegName, 1024, CIDA_Seg_No, m_i);
			ArraySeg->Get(m_i)->Save_Ini(in_Ini, mSegName);
			m_i++;
		}
	}
/*
..	Name : Form_Ini
..	Function:从Ini中读取Segment
..		并添加到ArraySeg中
..		若Seg数据冲突则更新数据
*/
	void UpToIDA() {
		int m_i = 0;
		int mSegLength = ArraySeg->getLength();
		while (m_i < mSegLength) {
			ArraySeg->Get(m_i)->UpToIDA();
			m_i++;
		}
	}
/*
..	Name : Form_Ini
..	Function:从Ini中读取Segment
..		并添加到ArraySeg中
..		若Seg数据冲突则更新数据
*/
	void Form_Ini(C_INI* in_Ini) {
		int m_i = 0;
		int mSegLength = in_Ini->GetIntValue(CIDA_Config, CIDA_Seg_Len);
		_MSG("Form_Ini SegLen:%d\n", mSegLength);
		char* mSegSection = (char*)malloc(1024);
		while (m_i < mSegLength) {
			memset(mSegSection,0,1024);
			sprintf(mSegSection,1024, CIDA_Seg_No,m_i);
			IDA_Seg* mIDASeg = new IDA_Seg(in_Ini,mSegSection);
			ArraySeg->add(mIDASeg);
			m_i++;
		}
	}
/*
..	Name : Lis_Seg
..	Function :　
*/
	Lis_Seg() {
		ArraySeg = new Lis<IDA_Seg>;
	}
};

