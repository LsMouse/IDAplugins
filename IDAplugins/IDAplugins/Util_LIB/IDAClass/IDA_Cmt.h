#include "Util_LIB.H"
/*
..保存数据以INI文件格式保存, 具体格式如下．．
..__________________________________________________________________________________
..[Seg.No.Cmt.No] 指向第No个Segment的第No个注释数据
	Name->函数名字
	Cmt->不可重复数据
	ReCmt->可重复数据
..__________________________________________________________________________________
*/
class Comment {
#define IDACMT_Start_EA	"Start_EA"
#define IDACMT_Cmt		"Cmt"
#define IDACMT_ReCmt	"ReCmt"
public:
	ea_t Start_EA;
	char* Cmt = NULL;
	char* Recmt = NULL;
/*
..	Name : isHaveDate
..	Function :　初始化HaveDate类
*/
	static boolean isHaveDate(ea_t inEA){
		char mBuf[1024];
		memset(mBuf, 0, 1024);
		if ((get_cmt(inEA, 0, mBuf, 1024) < 1) && (get_cmt(inEA, 1, mBuf, 1024) < 1)){
			return false;
		}
		return true;
	}
/*
..	Name : Save_Ini
..	Function :　
*/
	void Save_Ini(C_INI* inIni, char* inName){
		char* mCmtSection = strdup(inName);
		inIni->addIntValue(mCmtSection, IDACMT_Start_EA, Start_EA);
		if(Cmt != NULL)
			inIni->addStrValue(mCmtSection, IDACMT_Cmt, Cmt);
		if(Recmt != NULL)
			inIni->addStrValue(mCmtSection, IDACMT_ReCmt, Recmt);
	}
/*
..	Name : UpToIDA(ea_t inOld, ea_t inNew) 
..	Function : 将数据更新到IDA中
*/
	void UpToIDA(ea_t inOld, ea_t inNew) {
		ea_t mThis_EA = Start_EA - inOld + inNew;
		if (Cmt != NULL)
			set_cmt(mThis_EA, Cmt,0);
		if (Recmt != NULL)
			set_cmt(mThis_EA, Recmt, 1);
	}
/*
..	Name : Comment
..	Function :　初始化Comment类
*/
	Comment(ea_t inEA){
		Start_EA = inEA;
		char mBuf[1024];
		memset(mBuf,0,1024);
		if (get_cmt(Start_EA, 0, mBuf, 1024) < 1)Cmt = NULL;
		else{
			Cmt = (char*)malloc(1024);
			memset(Cmt,0,1024);
			get_cmt(Start_EA, 0, Cmt, 1024);
		}
		if (get_cmt(Start_EA, 1, mBuf, 1024) < 1)Recmt = NULL;
		else{
			Recmt = (char*)malloc(1024);
			memset(Recmt, 0, 1024);
			get_cmt(Start_EA, 1, Recmt, 1024);
		}
	}
/*
..	Name : Comment
..	Function : 将inIni中数据的Comment为起始端获取到IDA中
*/
	Comment(C_INI* inIni, char* inSecName) {
		Start_EA = inIni->GetIntValue(inSecName, IDACMT_Start_EA);
		Cmt = inIni->GetStrValue(inSecName, IDACMT_Cmt);
		Recmt = inIni->GetStrValue(inSecName, IDACMT_ReCmt);
	}
};
class Lis_Cmt{
#define	CIDA_CmtLen		"CmtLen"
#define	CIDA_CmtNo		"%s.Cmt.%d"
public:
	Lis<Comment>* ArrayCmt = new Lis<Comment>;
/*
..	Name : AddCmt
..	Function :　
*/
	void AddCmt(ea_t inStart,ea_t inEnd){
		ea_t m_i = inStart;
		while (m_i <= inEnd){
			if (Comment::isHaveDate(m_i)){
				ArrayCmt->add(new Comment(m_i));
			}
			m_i++;
		}
	}
/*

*/
	void Save_Ini(C_INI* in_Ini, char* inName){
		int m_i = 0;
		char* mBuf = (char*)malloc(1024);
		in_Ini->addIntValue(inName, CIDA_CmtLen, ArrayCmt->getLength());
		while (m_i < ArrayCmt->getLength()) {
			memset(mBuf, 0, 1204);
			sprintf(mBuf,1024, CIDA_CmtNo, inName, m_i);
			ArrayCmt->Get(m_i)->Save_Ini(in_Ini, mBuf);
			m_i++;
		}
		free(mBuf);
	}
/*
..	Name : Form_Ini
..	Function:从Ini中读取CMT_并添加到ArrayCmt中
*/
	void Form_Ini(C_INI* in_Ini, char* inName) {
		int m_i = 0;
		char* mFunNo = (char*)malloc(1024);
		int m_CmtLen = in_Ini->GetIntValue(inName, CIDA_CmtLen);
		while (m_i < m_CmtLen) {
			memset(mFunNo, 0, 1024);
			sprintf(mFunNo, 1024, CIDA_CmtNo, inName, m_i);
			Comment* mCMT = new Comment(in_Ini, mFunNo);
			ArrayCmt->add(mCMT);
			m_i++;
		}
	}
/*
..	Name : UpToIDA(ea_t inOld, ea_t inNew)
..	Function :　
*/
	void UpToIDA(ea_t inOld, ea_t inNew){
		int m_i = 0;
		int m_CmtLen = ArrayCmt->getLength();
		while (m_i < m_CmtLen) {
			ArrayCmt->Get(m_i)->UpToIDA(inOld, inNew);
			m_i++;
		}
	}
/*
..	Name : Lis_Cmt
..	Function :　初始化Lis_Cmt类
*/
	Lis_Cmt(C_INI* in_Ini, char* inName) {
		ArrayCmt = new Lis<Comment>;
		Form_Ini(in_Ini, inName);
	}
	Lis_Cmt(ea_t inStart, ea_t inEnd){
		ArrayCmt = new Lis<Comment>;
		AddCmt(inStart, inEnd);
	}
	Lis_Cmt(segment_t* inSeg){
		ArrayCmt = new Lis<Comment>;
		Lis_Cmt(inSeg->startEA, inSeg->endEA);
	}
};
