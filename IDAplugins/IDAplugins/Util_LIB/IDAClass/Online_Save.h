#include "Util_LIB.H"
class Online_Save{
public:
	List<_Base_Segment> Seg;
/**
* @See	更新全部列表
*	在当前IDA中，更新已添加的段
*/
	void UpAllSegment(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			//查找并更新段
			Seg.Get()->UpSegment();
			Seg.Next();
		}
	}
/**
* @See	将当前段信息全部更新到IDA中
*/
	void To_IDAMem(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			Seg.Get()->To_IDAMem();
			Seg.Next();
		}
	}
/**
* @See		将inSeg加载到Seg列表
* @Param　	inSeg -> IDA段类
*/
	void AddSegment(_Base_Segment* inBSeg){
		if (inBSeg == NULL)return;
		Debug::MSG("CheckCode:0x%08x\n", inBSeg->CheckCode);
		//之前链表为空，直接加载
		if (Seg.GetLength() == 0){
			Seg.Inster(inBSeg);
			return;
		}
		//查找是否有匹配段
		Seg.Reset();
		while (Seg.Get() != NULL){
			if (Seg.Get()->CheckCode == inBSeg->CheckCode){
				//存在共同体,想将数据加载到IDAMem，
				//若IDAMem不存在这段，就不在对Seg更改
				inBSeg->To_IDAMem();
				if (inBSeg->FindSegment() != NULL)
					Seg.Get()->AddSegment(inBSeg->FindSegment());
				return;
			}
			Seg.Next();
		}
		//无匹配段，则添加
		Seg.Inster(inBSeg);
		Debug::MSG("Seg Length:%d\n", Seg.GetLength());
	}
/**
* @See		将inSeg加载到Seg列表
* @Param　	inSeg -> IDA段类
*/
	void AddSegment(segment_t* inSeg){
		if (inSeg == NULL)return;
		_Base_Segment* _seg = new _Base_Segment(inSeg);
		AddSegment(_seg);
	}
/**
* @See		初始化Online_Save
*/
	void Save(char* in_path){
		INI* _ini = new INI();
		_ini->iniPath = in_path;
		_ini->addIntValue("CONFIG", "SegLength", Seg.Length);
		Seg.Reset();
		while (Seg.Get()!= NULL){
			Seg.Get()->To_Ini(_ini, Seg.GetPlace());
			Seg.Next();
		}
		_ini->SaveIni(in_path);
	}
/**
* @See		初始化Online_Save
*/
	void Online_Load(char* inPath){
		INI* _ini = new INI(inPath);
		char* SecName = (char*)Util_Base::Alloc(1024);
		int SegLength = _ini->GetIntValue("CONFIG", "SegLength");
		int m_i = 0;
		while (m_i < SegLength){
			_Base_Segment* _BSeg = new _Base_Segment(_ini, m_i);
			AddSegment(_BSeg);
			m_i++;
		}
		To_IDAMem();
	}
};