#include "Util_LIB.H"
/*		
..实现在调试模式下，保存当前函数名称、注释等内容，显示重新链接程序时导致注释丢失等。
..保存数据以INI文件格式保存,具体格式如下．．
..__________________________________________________________________________________
..[Config] -＞ 存放整个配置信息
..	SegLength -> Seg个数
..	[Seg.No] -> 指向第No个Segment数据
		Name ->Segment 名字
		Start_EA ->	段开始地址
		End_EA ->	段结束地址
		FunLen ->   函数个数
		CmtLen ->	注释个数
		CheckCode -> Segment 校验码
..		[Seg.No.Fun.No] 指向第No个Segment的第No个函数数据
			Name -> 函数名字
			Start_EA -> 函数开始地址
			End_EA ->	函数结束地址
			Cmt -> 不可重复数据
			ReCmt -> 可重复数据
			Code_S -> 开始码(NoUsed)
			Code_E -> 结束码(NoUsed)
..		[Seg.No.Cmt.No] 指向第No个Segment的第No个注释数据
			Name -> 函数名字
			Cmt -> 不可重复数据
			ReCmt -> 可重复数据
..__________________________________________________________________________________
*/
class IDA_Back{
public:
	Lis_Seg* LisSeg = new Lis_Seg();
/*
..	Name :  Save_BSegm(char* inPath)
..	Function : 保存成Save_BSegm文件(INI格式)
*/
	void Save_BSegm(char* inPath){
		C_INI* mBSegm = new C_INI(inPath);
		LisSeg->Save_Ini(mBSegm);
		mBSegm->SaveIni(inPath);
	}
/*
..	Name : Form_BSegm(char* inPath)
..	Function : 从BSegm文件中(INI格式)，读取数据。
*/
	void Form_BSegm(char* inPath){
		C_INI* mBSegm = new C_INI(inPath);
		mBSegm->load();
		LisSeg->Form_Ini(mBSegm);
		LisSeg->UpToIDA();
	}
/*
..	Name : addSegment(ea_t inStart,ea_t inEnd)
..	Function : 添加当前段
*/
	void addSegment(ea_t inStart, ea_t inEnd) {
		LisSeg->addSeg(inStart,inEnd);
	}
/*
..	Name : addSegment(segment_t* inSeg)
..	Function : 添加当前段
*/
	void addSegment(segment_t* inSeg){
		LisSeg->addSeg(inSeg);
	}
/*
..	Name : addSegment(IDA_Seg* inSeg)
..	Function : 添加当前段
*/
	void addSegment(IDA_Seg* inSeg) {
		LisSeg->addSeg(inSeg);
	}
/*
..	Name : IDA_Back
..	Function : 初始化数据
*/
	IDA_Back(){
		LisSeg = new Lis_Seg();
	}
};