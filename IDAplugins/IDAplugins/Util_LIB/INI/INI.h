#include<Util_LIB.H>
/*
..	Class : C_INI_Key
..	Comment : 此类为INI保存参数
*/
typedef struct C_INI_Key{
public:
	char* Name = NULL;
	char* Value = NULL;
/*
..	Name : C_INI_Key构造函数
..	Function :　初始化字符串值
*/
	C_INI_Key(char* inName, char* inValue){
		Name = strdup(inName);
		Value = strdup(inValue);
	}
}*LP_INIKEY;
/*
..	Class : C_INI_Section
..	Comment : 此类为INI保存节
*/
class C_INI_Section{
public:
	char* SecName = NULL;
	long Leng = NULL;
	List<C_INI_Key>* Key = NULL;
/*
..	Name : AddInt
..	Function :　增加INT参数
*/
	void AddInt(char *in_Key, int in_Value){
		char* m_Value = (char*)Util_Base::Alloc(12);
		sprintf(m_Value, 11, "0x%08x", in_Value);
		Key->Inster(new C_INI_Key(in_Key, m_Value));
		free(m_Value);
	}
/*
..	Name : AddString
..	Function :　增加字符串函数
*/
	void AddString(char *in_Key, char* in_Value){
		Key->Inster(new C_INI_Key(in_Key, in_Value));
	}
/*
..	Name : GetInt
..	Function :　获取INT参数
*/
	int GetInt(char *in_Key){
		if (in_Key == NULL)return NULL;
		if (Key->First == NULL)return NULL;
		char* m_Str = GetString(in_Key);
		if (m_Str == NULL)return NULL;
		return strtol(m_Str, NULL, 16);
	}
/*
..	Name : GetString
..	Function :　获取INT参数
*/
	char* GetString(char *in_Name){
		if (in_Name == NULL)return NULL;
		if (Key->First == NULL)return NULL;
		Key->Reset();
		do{
			if (Key->Get() != NULL){
				if (!strcmp(Key->Get()->Name, in_Name)){
					return  Key->Get()->Value;
				}
			}
			Key->Next();
		} while (Key->Get() != NULL);
		return NULL;
	}
/*
..	Name : C_INI_Section构造函数
..	Function :　初始化C_INI_Section类的基本全局变量、初始化长度
*/
	C_INI_Section(char* in_name){
		Key = new List<C_INI_Key>;
		SecName = strdup(in_name);
		Leng = 0;
	}
};
/*
..	Class : INI
..	Comment : INI类
*/
class INI{
#define	MAX_SECTION			65535		//Section最大长度
#define	MAX_KEY				65535		//KeyValues最大长度
#define	MAX_ALLSECTIONS     65535		//所有Section的最大长度
#define	MAX_ALLKEYS			65535		//所有KeyValue的最大长度
#define MAX_FILE_SIZE		10*1024*1024//文件最大长度
#define LEFT_BRACE			'['			//
#define RIGHT_BRACE			']'			//
public:
	char* iniPath = NULL;
	char* FileBuf = NULL;
	List<C_INI_Section>* Section = NULL;
/*-----------------------------------------------------------------------------------*/
/*
..	Name : GetSection
..	Function :  查找<in_SegName>节
..	Input : in_SegName -> 节名
..	Ouput : NULL -> 不存在 , !NULL -> C_INI_Section*
*/
	C_INI_Section* GetSection(char* in_SegName){
		if (Section == NULL)return NULL;
		Section->Reset();
		while (Section->Get() != NULL){
			if (!strcmp(Section->Get()->SecName, in_SegName)){
				return Section->Get();
			}
			Section->Next();
		}
		return NULL;
	}
/*
..	Name : GetStrValue
..	Function :  查找<in_SegName>节中in_Key对应的Value(String)
..	Input : in_SegName -> 节名
			in_Key ->　Key名字
..	Ouput : NULL -> 不存在 , !NULL -> char*
*/
	char* GetStrValue(char* in_SegName, char* in_Key){
		if (GetSection(in_SegName) == NULL)return NULL;
		return GetSection(in_SegName)->GetString(in_Key);
	}
/*
..	Name : GetIntValue
..	Function :  查找<in_SegName>节中in_Key对应的Value(int)
..	Input : in_SegName -> 节名
			in_Key ->　Key名字
*/
	int GetIntValue(char* in_SegName, char* in_Key){
		char* out_Value = NULL;
		if (GetSection(in_SegName) == NULL)return NULL;
		return GetSection(in_SegName)->GetInt(in_Key);
	}
/*
..	Name : addSection
..	Function :  在链表中添加<in_SegName>节
*/
	void addSection(char* in_SegName){
		if (Section == NULL)Section = new List<C_INI_Section>;
		C_INI_Section* mSec = new C_INI_Section(in_SegName);
		Section->Inster(mSec);
	}
/*
..	Name : addStrValue
..	Function :  在链表的<in_SegName>节中添加KEY和in_Value(String)
*/
	void addStrValue(char* in_SegName, char* in_Key, char* in_Value){
		if (in_Key == NULL)return;
		if (GetSection(in_SegName) == NULL)addSection(in_SegName);
		if (in_Value == NULL)GetSection(in_SegName)->AddString(in_Key, "NULL");
		else GetSection(in_SegName)->AddString(in_Key, in_Value);
	}
/*
..	Name : addIntValue
..	Function :  在链表的<in_SegName>节中添加KEY和in_Value(Int)
*/
	void addIntValue(char* in_SegName, char* in_Key, int in_Value){
		if (in_Key == NULL)return;
		if (GetSection(in_SegName) == NULL)addSection(in_SegName);
		if (in_Value == NULL)GetSection(in_SegName)->AddString(in_Key, "NULL");
		else GetSection(in_SegName)->AddInt(in_Key, in_Value);
	}
/*-----------------------------------------------------------------------------------*/
/*判断是文件是否结束*/
	int isEndFile(char* In_Buf,long m_point){
		if (In_Buf[m_point] == -1)return 1;
		return 0;
	}
/*判断左Section*/
	int isLeftSectionMark(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf, in_Po))return 1;
		return LEFT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*判断右Section*/
	int isRightSectionMark(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf, in_Po))return 1;
		return RIGHT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*判断是否为新行*/
	int isNewLine(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\r' || In_Buf[in_Po] == '\n')return 1;
		return 0;
	}
/*判断是否结束*/
	int isEndKey(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\0' || In_Buf[in_Po] == '=')return 1;
		return 0;
	}
/*判断是否结束*/
	int isEndValue(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 1;
		if (In_Buf[in_Po] == '\0')return 1;
		if ((In_Buf[in_Po] == 0x0D))return 1;
		if (In_Buf[in_Po] == 0x0A)return 1;
		return 0;
	}
/*判断是空格字符*/
	int isSpace(char* In_Buf, long in_Po){
		if (isEndFile(In_Buf,in_Po))return 0;
		return isspace(In_Buf[in_Po]);
	}
/*
..Name : load
..Comment:将INI文件数据加载到链表结构
*/
	int load(){
		//开始查找整个INI文件内容
		long po_Find = 0;
		//判断文件是否结束
		Debug::MSG("Find INI @ %s\n", iniPath);
		while (!isEndFile(FileBuf,po_Find)){
			//循环查找段(Section)、判断新行和查找'[',则进行查找
			if (isLeftSectionMark(FileBuf, po_Find++)){
				char* p_SecName;
				long mStart_Section = po_Find;
				long mLen_Section;
				//字符串结束或则查找到']'
				while (!isRightSectionMark(FileBuf, po_Find)){
					po_Find++;
				}
				mLen_Section = po_Find - mStart_Section;
				//获取到Section字符串、去空格
				p_SecName = (char*)malloc(mLen_Section + 1);
				memset(p_SecName, 0, mLen_Section + 1);
				memcpy(p_SecName, &FileBuf[mStart_Section], mLen_Section);
				p_SecName = trim(p_SecName);
				addSection(p_SecName);
				//	_MSG("FindSec:%s\n", p_SecName);
				//跳过空格字符
				while (isSpace(FileBuf, ++po_Find));
				//判断是否为新的段开始， 不是开始查找Key和Value
				while (!isLeftSectionMark(FileBuf, po_Find)){
					//开始查找Key和Value
					char* p_Key;
					long mStart_Key = po_Find;
					long mLen_Key;
					//判断结束新行或则字符串结束。		
					while (isSpace(FileBuf, po_Find)) {
						po_Find++;
					}
					mStart_Key = po_Find;
					//判断是否为注释,是的话重新查找开始判断
					if (';' != FileBuf[po_Find++]){
						char* p_Value;
						long mStart_Value;
						long mLen_Value;
						//查找KEY字符串结束、去空格
						while (!isEndKey(FileBuf, po_Find)){
							po_Find++;
						}
						if (isEndFile(FileBuf,po_Find))break;
						mLen_Key = po_Find - mStart_Key;
						p_Key = (char*)malloc(mLen_Key + 1);
						memset(p_Key, 0, mLen_Key + 1);
						memcpy(p_Key, &FileBuf[mStart_Key], mLen_Key);
						p_Key = trim(p_Key);
						//获取等号
						while (FileBuf[po_Find++] != '='){
							if (isEndFile(FileBuf,po_Find))break;
						}
						//获取Value
						while (isSpace(FileBuf, po_Find)){
							po_Find++;
						}
						mStart_Value = po_Find;
						while (!isEndValue(FileBuf, po_Find)){
							po_Find++;
						}
						//查找Value字符串结束、去空格
						if (isEndFile(FileBuf,po_Find))break;
						mLen_Value = po_Find - mStart_Value;
						p_Value = (char*)malloc(mLen_Value + 1);
						memset(p_Value, 0, mLen_Value + 1);
						memcpy(p_Value, &FileBuf[mStart_Value], mLen_Value);
						p_Value = trim(p_Value);
						Util_Char::ReplaceChar(p_Value, 0x0D, 0x01);
						Util_Char::ReplaceChar(p_Value, 0x0A, 0x02);
						//添加到列表，
						Debug::MSG("FindKey:%s,%s\n", p_Key, p_Value);
						addStrValue(p_SecName, p_Key, p_Value);
						while (isSpace(FileBuf, po_Find)){
							po_Find++;
						}
					}
				}
			}
		}
		Debug::MSG("Find INI End\n", iniPath);
		return 0;
	}
/*
..
..
*/
	void SaveIni(char* in_Path){
		if (in_Path == NULL)return;
		FILE* m_fd = fopen(in_Path, "wb+");
		char* mBuf = (char*)malloc(10 * 1024);
		Section->Reset();
		while (Section->Get() != NULL){
			memset(mBuf, 0, 10 * 1024);
			sprintf(mBuf, 1024, "[%s]\r\n", Section->Get()->SecName);
			Util_File::fwrite(m_fd, mBuf);
			Section->Get()->Key->Reset();
			while (Section->Get()->Key->Get() != NULL){
				memset(mBuf, 0, 10 * 1024);
				sprintf(mBuf, 1024, "%s = %s\r\n",
					Section->Get()->Key->Get()->Name,
					Section->Get()->Key->Get()->Value);
				Util_File::fwrite(m_fd, mBuf);
				Section->Get()->Key->Next();
			}
			Section->Next();
		}
		fclose(m_fd);
	}
/*
..		设置文件路径，读取文件能容
*/
	INI(char* in_path){
		iniPath = strdup(in_path);
		Section = new List<C_INI_Section>;
		FileBuf = Util_File::Read(in_path, "rb+");
		load();
	}
	INI(){
		Section = new List<C_INI_Section>;
	}
};

