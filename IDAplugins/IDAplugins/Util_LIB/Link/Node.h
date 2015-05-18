template <class Type>class Node{
public:
	Type* Data = NULL;
	Node<Type>* next = NULL;
	Node<Type>* prev = NULL;
/*
..	Name : getInfo
..	Function :　获取链表内容
*/
	Type* getInfo(){
		return Data;
	}
/*
..	Name : Node
..	Function :　链表初始化
*/
	Node(Type* in_Type){
		Data = in_Type;
		next = prev = NULL; 
	}
};
template <class Type>class Lis{
public:
	Node<Type>*First = NULL, *Last = NULL;
/*
..	Name : Node
..	Function :　链表初始化
*/	
	void add(Type* in_Type){
		if (in_Type == NULL)return;
		if ((First == NULL) || (Last == NULL)){
			Node<Type>* mData = new Node<Type>(in_Type);
			First = Last = mData;
		}
		else{
			Node<Type>* mData = new Node<Type>(in_Type);
			mData->prev = NULL;
			Last->next = mData;
			Last = Last->next;
			Last->next = NULL;
		}
	}
/*
..	Name : Get
..	Function :　获取第in_no号Type数据
*/
	Type* Get(int in_no){
		int m_i = 0;
		Node<Type>* mNode = First;
		while (m_i < in_no){
			if (mNode == NULL)return NULL;
			mNode = mNode->next;
			m_i++;
		}
		if (mNode == NULL)return NULL;
		return mNode->getInfo();
	}
/*
..	Name : Set
..	Function :　重新设置第in_no号Type数据
*/
	void Set(int in_no, Type* in_Type) {
		int m_i = 0;
		Node<Type>* mNode = First;
		while (m_i < in_no) {
			if (mNode == NULL)return;
			mNode = mNode->next;
			m_i++;
		}
		if (mNode == NULL)return;
		mNode->Data = in_Type;
	}
/*
..	Name : getLength
..	Function :　获取链表宽度
*/
	int getLength(){
		int m_i = 0;
		Node<Type>* po_type = First;
		while (po_type != NULL){
			po_type = po_type->next;
			m_i++;
		}
		return m_i;
	}
/*
..	Name : Node
..	Function :　链表初始化
*/
	Lis(){
		First = Last = NULL;
	}
};
