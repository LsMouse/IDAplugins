#include "Util_LIB.H"
/**
* @See		定义链表基本单位结构体
* @Struct	LinkNode
*/
template <class Type>

struct LinkNode{
	Type*	info;
	struct LinkNode*	_N_next;
	struct LinkNode*	_N_prev;
};
/**
* @See		LinkNode基本操作类
* @Class	_LinkNode_Base
*/
template <class Type>
class _LinkNode_Base {
public:
typedef	LinkNode<Type>*		LP_Node;
/**
* @See		创建一个空内容的基本单位结构体
* @Return	返回基本单位结构体指针
*/
	static LP_Node CreatVoidNode(){
		return(LP_Node)Util_Base::Alloc(sizeof(struct LinkNode<Type>));
	}
/**
* @See		创建基本单位结构体
* @Return	返回基本单位结构体指针
*/
	static LP_Node CreatNode(Type* inTy){
		LP_Node _out = CreatVoidNode();
		_out->info = inTy;
		return _out;
	}
};
/**
* @See		列表基本操作类
* @Class	_List_Base
*/
template <class Type>
class _List_Base :public _LinkNode_Base<Type>{
public:
	LP_Node		First,Last;
	size_t		Length;
/**
* @See		清楚整个链表内容
*/
	void Clear(){
// 存在问题
/*		if ((First != NULL) && (Last != NULL)){
			LP_Node _m_point = Last;
			while (_m_point != NULL){
				_m_point->info = NULL;
				_m_point = _m_point->_N_prev;
				free(_m_point->_N_next);
				_m_point->_N_next = NULL;
			}
		}**/
		_List_Base();
	}
/**
* @See	初始化列表
*/
	_List_Base(){
		First = Last = NULL;
		Length = 0;
	}
};
/**
* @See		列表类
* @Class	List
*/
template <class Type>
class List :public _List_Base<Type>{
public:
	LP_Node _LinkThis = NULL;
	ulong _NoThis = 0;
/**
* @See		添加数据，添加到列表尾部
* @Param	类型数据
*/
	void Inster(Type* _inTy){
		//判断列表是否为空
		if (First == NULL){
			Last = First = CreatNode(_inTy);
			Length = 1;
			return;
		}
		Last->_N_next = CreatNode(_inTy);
		Last->_N_next->_N_prev = Last;
		Last = Last->_N_next;
		Length++;
	}
/**
* @See		获取列表数据
* @Param	类型数据
*/
	Type* Get(int inNo){
		LP_Node _OutTyre = First;
		int m_i = 0;
		while ((m_i < inNo) && (_OutTyre != NULL)){
			_OutTyre = _OutTyre->_N_next;
			m_i++;
		}
		return _OutTyre->info;
	}
/**
* @See		获取链表宽度
* @Return	返回链表宽度
*/
	size_t GetLength(){
		return Length;
	}
/*-----------------------------------------------------------------------------------*/
/*						以下函数为了操作方便设置的									*/
/**
* @See		复位This指针
*/
	void Reset(){
		_LinkThis = First;
		_NoThis = 0;
	}
/**
* @See		获取This指向的数据
* @Return	This数据域
*/
	Type* Get(){
		if (_LinkThis == NULL)return NULL;
		return _LinkThis->info;
	}
/**
* @See		获取This的位置
* @Return	This数据域
*/
	ulong GetPlace(){
		return _NoThis;
	}
/**
* @See		获取This指针指向下一个数据
*/
	void Next(){
		if (_LinkThis == NULL)return ;
		_LinkThis = _LinkThis->_N_next;
		_NoThis++;
	}
/**
* @See		初始化列表
*/
	List(){
		_List_Base();
	}
};

