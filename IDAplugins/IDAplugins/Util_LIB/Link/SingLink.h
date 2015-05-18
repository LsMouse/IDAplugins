#include "Util_LIB.H"
/*
..单向链表子模板,用于保存模板内容和指向下一个链表
*/
template <class Type>class LinkNode {
public:
	char* IDName;
	Type Data;				//数据保存
	LinkNode* next;
/*设置名字*/
	void SetIDName(char *in_name) {
		IDName = strdup(in_name);
	}
/*设置节点内容*/
	void SetType(Type in_Type) {
		memcpy(&Data, &in_Value, sizeof(Type));
	}
	void SetType(Type* in_Type) {
		memcpy(&Data,in_Value, sizeof(Type));
	}
/*设置为空*/
	void Empty() {
		next = NULL;
		TypeName = NULL;
		Data = NULL;
	};
/*获取下一个链表链接*/
	Type* GetNext(){
		return next;
	}
/*删除下一个节点*/
	void RemoveAfter(){
		if (nextLink == NULL)return;
		next = next->next;
	}
/*初始化模块内容*/
	LinkNode(Type in_Value) {
		memcpy(&Data, &in_Value, sizeof(Type));
		IDName = NULL;
		next = NULL;
	}
	/*初始化模块内容*/
	LinkNode(char* in_name,Type in_Value){
		memcpy(&Data, &in_Value, sizeof(Type));
		IDName = strdup(in_name);
		next = NULL;
	}
};
/*
//			单向链表子模板
//	1.用于读取操作、插入、移出等等
*/
template <class Type>class SingLink{
public:
	int Length = 0;
	LinkNode<Type> *FirstList,*LastList;
/*在末尾加入一个节点*/
	void add(Type value) {
		if (value == NULL)return;
		if (FirstList == NULL) {
			LastList = FirstList = new LinkNode<Type>(value);
			Length = 1;
			return;
		}
		Length++;
		LastList->next = new LinkNode<Type>(value);
		LastList = LastList->next;
	}
/*添加一个带有名字字节,*/
	void add(char * in_Name,Type in_Value) {
		/*先查找是否有名字相同的NODE，有的话更新NODE*/
		if (Get(in_Name) != NULL){
			Get(in_Name)->Data = new LinkNode<Type>(in_Value);
			return;
		}
		Length++;
		if (in_Value == NULL)return;
		if (FirstList == NULL) {
			LastList = FirstList = new LinkNode<Type>(in_Value);
			return;
		}
		LastList->next = new LinkNode<Type>(in_Value);
		LastList = LastList->next;
	}
/*获取第N个链表结构中的内容*/
	LinkNode<Type>* Get(int in_no){
		LinkNode<Type>* retLink = FirstList;
		while (in_no--){
			/*想要获取的长度并没有在获取数据的长度上*/
			if (retLink == NULL)return NULL;
			retLink = retLink->next;
		}
		return retLink;
	}
/*获取第N个链表结构内容*/
	Type* GetData(char* in_Name){
		if (Get(in_Name) != NULL)return &Get(in_Name)->Data;
		return NULL;
	}
	Type* GetData(int in_No){
		if (Get(in_Name) != NULL)return &Get(in_Name)->Data;
		return NULL;
	}
/*移出第(in_no)个节点*/
	void Remove(int in_no){
		LinkNode<Type>* mLink  = Get(in_no);
		if (mLink == NULL) return;
		Get(in_no - 1)->next = Get(in_no + 1);
		delete  mLink;
	}
/*在（in_no）节点插入in_Link内容*/
	int Insert(int in_no,Type in_Link){
		SingLinkNode<Type>* mAgo = Get(in_no-1);
		SingLinkNode<Type>* mLast = Get(in_no);
		if (mAgo == NULL)return -1;
		if (in_no == 0){
			LinkNode<Type>* mBack = FirstList;
			FirstList = new LinkNode<Type>(in_Link);
			FirstList->next = mBack;
			return 0;
		}
		mAgo->next = new LinkNode<Type>(in_Link);
		mAgo->next->next = mLast;
		return 0;
	}
/*清列表数据*/
	void MarkEmpty() {
		FirstList = LastList = NULL;
		Length = 0;
	}
/*初始化链表*/
	SingLink(){
		MarkEmpty();
	}
};



