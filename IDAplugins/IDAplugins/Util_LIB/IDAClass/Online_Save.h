#include "Util_LIB.H"
class Online_Save{
public:
	List<_Base_Segment> Seg;
/**
* @See		更新全部列表
*/
	void Update(){
		Seg.Reset();
		while (Seg.Get() != NULL){
			Seg.Get()->Update();
			Seg.Next();
		}
	}
/**
* @See		将inSeg加载到Seg列表
* @Param　	inSeg -> IDA段类
*/
	void AddSegment(segment_t* inSeg){
		if (inSeg == NULL)return;
		_Base_Segment* _seg = new _Base_Segment(inSeg);
		Debug::MSG("CheckCode:0x%08x\n", _seg->CheckCode);
		if (Seg.GetLength() == 0){
			Seg.Inster(_seg);
		}
		else{
			//查找是否有匹配段
			Seg.Reset();
			while (Seg.Get() != NULL){
				if (Seg.Get()->CheckCode == _seg->CheckCode){
					Seg.Get()->Update(inSeg);
					delete _seg;
					return;
				}
				Seg.Next();
			}
			//无匹配段，则添加
			Seg.Inster(_seg);
		}
		Debug::MSG("Seg Length:%d\n", Seg.GetLength());
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
			Seg.Get()->ToIni(_ini, Seg.GetPlace());
			Seg.Next();
		}
		_ini->SaveIni(in_path);
	}
/**
* @See		初始化Online_Save
*/
	Online_Save(){

	}
};