#include<Util_LIB.H>
/*
..	Class : C_INI_Key 
..	Comment : 此类为INI保存参数
*/
class C_INI_Key{
public:
	char* Name = NULL;
	char* Value = NULL;
/*
..	Name : C_INI_Key构造函数
..	Function :　初始化字符串值
*/
	C_INI_Key(char *in_Key, char* in_Value){
		Name = (char*)malloc(strlen(in_Key) + 1);
		Value = (char*)malloc(strlen(in_Value) + 1);
		Name = strdup(in_Key);
		Value = strdup(in_Value);
	}
};
/*
..	Class : C_INI_Section
..	Comment : 此类为INI保存节
*/
class C_INI_Section{
public:
	char* SecName = NULL;
	long Leng = NULL;
	Lis<C_INI_Key>* Key = new Lis<C_INI_Key>;
/*
..	Name : AddInt
..	Function :　增加INT参数
*/
	void AddInt(char *in_Key, int in_Value){
		char* m_Value = (char*)malloc(12);
		memset(m_Value, 0, 12);
		sprintf(m_Value, 11, "0x%08x", in_Value);
		Key->add(new C_INI_Key(in_Key, m_Value));
	}
/*
..	Name : AddString
..	Function :　增加字符串函数
*/
	void AddString(char *in_Key, char* in_Value){
		Key->add(new C_INI_Key(in_Key, in_Value));
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
		return strtol(m_Str,NULL, 16);
	}
/*
..	Name : GetString
..	Function :　获取INT参数
*/
	char* GetString(char *in_Key){
		if (in_Key == NULL)return NULL;
		if (Key->First == NULL)return NULL;
		Node<C_INI_Key>* m_key = Key->First;
		do{
			if (m_key->Data != NULL){
				if (!strcmp(m_key->Data->Name, in_Key)){
					return  m_key->Data->Value;
				}
			}
			m_key = m_key->next;
		} while (m_key != NULL);
		return NULL;
	}
/*
..	Name : C_INI_Section构造函数
..	Function :　初始化C_INI_Section类的基本全局变量、初始化长度
*/
	C_INI_Section(char* in_name){
		Key = new Lis<C_INI_Key>;
		SecName = in_name;
		Leng = 0;
	}
};
/*
*/
class C_INI{
#define	MAX_SECTION			65535		//Section最大长度
#define	MAX_KEY				65535		//KeyValues最大长度
#define	MAX_ALLSECTIONS     65535		//所有Section的最大长度
#define	MAX_ALLKEYS			65535		//所有KeyValue的最大长度
#define MAX_FILE_SIZE		1024*1024 //文件最大长度
#define LEFT_BRACE			'['			//
#define RIGHT_BRACE			']'			//
public:
	char* iniPath;
	char* IniInfoBuf = NULL;
	long IniInfoBuf_Size = NULL;
	//节点列表
	Lis<C_INI_Section>* ArraySection = new Lis<C_INI_Section>;
/*-----------------------------------------------------------------------------------*/
/*
..	Name : GetSection
..	Function :  查找<in_SegName>节
..	Input : in_SegName -> 节名
..	Ouput : NULL -> 不存在 , !NULL -> C_INI_Section*
*/
	C_INI_Section* GetSection(char* in_SegName){
		Node<C_INI_Section>* mFind = ArraySection->First;
		while (mFind != NULL){
			if (!strcmp(mFind->Data->SecName, in_SegName)){
				return mFind->Data;
			}
			mFind = mFind->next;
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
		if (ArraySection == NULL)ArraySection = new Lis<C_INI_Section>;
		C_INI_Section* mSec = new C_INI_Section(in_SegName);
		ArraySection->add(mSec);
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
	int isEndFile(long m_point){
		if (IniInfoBuf[m_point] == -1)return 1;
		if (m_point >= IniInfoBuf_Size)return 1;
		return 0;
	}
/*判断左Section*/
	int isLeftSectionMark(char* In_Buf, long in_Po){
		return LEFT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*判断右Section*/
	int isRightSectionMark(char* In_Buf, long in_Po){
		return RIGHT_BRACE == In_Buf[in_Po] ? 1 : 0;
	}
/*判断是否为新行*/
	int isNewLine(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\r' || In_Buf[in_Po] == '\n')return 1;
		return 0;
	}
/*判断是否结束*/
	int isEndKey(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\0' || In_Buf[in_Po] == '=')return 1;
		return 0;
	}
/*判断是否结束*/
	int isEndValue(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 1;
		if (In_Buf[in_Po] == '\0')return 1;
		if ((In_Buf[in_Po] == 0x0D))return 1;
//		if (In_Buf[in_Po] == 0x0A)return 1;
		return 0;
	}
/*判断是空格字符*/
	int isSpace(char* In_Buf, long in_Po){
		if (isEndFile(in_Po))return 0;
		return isspace(In_Buf[in_Po]);
	}

/*
..Name : load_ini_file
..Comment:将INI文件数据加载到链表结构
..
*/
	int load_ini_file(){
		FILE *in_fd = NULL;
		assert(iniPath != NULL);
		assert(IniInfoBuf != NULL);
		int i = 0;
		IniInfoBuf = (char*)malloc(MAX_FILE_SIZE);
		memset(IniInfoBuf, -1, MAX_FILE_SIZE);
		in_fd = fopen(iniPath, "rb+");
		if (NULL == in_fd){
			return 0;
		}
		IniInfoBuf[i] = fgetc(in_fd);
		while (IniInfoBuf[i] != (char)EOF) {
			i++;
			assert(i < MAX_FILE_SIZE);
			IniInfoBuf[i] = fgetc(in_fd);
		}
		IniInfoBuf[i] = 0x0D;
		IniInfoBuf[i + 1] = 0x0A;
		IniInfoBuf[i + 2] = '\0';
		IniInfoBuf[i + 3] = EOF;
		IniInfoBuf_Size = i-1;
		fclose(in_fd);
		return 1;
	}
/*
..Name : load
..Comment:将INI文件数据加载到链表结构
..
*/
	int load(){
		load_ini_file();
		//开始查找整个INI文件内容
		long po_Find = 0;
		//判断文件是否结束
		msg("Find INI @ %s\n", iniPath);
		while (!isEndFile(po_Find)){
			//循环查找段(Section)、判断新行和查找'[',则进行查找
			if (isLeftSectionMark(IniInfoBuf, po_Find++)){
				char* p_SecName;
				long mStart_Section = po_Find;
				long mLen_Section;
				//字符串结束或则查找到']'
				while (!isRightSectionMark(IniInfoBuf, po_Find)){
					po_Find++;
				}
				mLen_Section = po_Find - mStart_Section;
				//获取到Section字符串、去空格
				p_SecName = (char*)malloc(mLen_Section + 1);
				memset(p_SecName, 0, mLen_Section + 1);
				memcpy(p_SecName, &IniInfoBuf[mStart_Section], mLen_Section);
				p_SecName = trim(p_SecName);
				addSection(p_SecName);
			//	_MSG("FindSec:%s\n", p_SecName);
				//跳过空格字符
				while (isSpace(IniInfoBuf, ++po_Find));
				//判断是否为新的段开始， 不是开始查找Key和Value
				while (!isLeftSectionMark(IniInfoBuf, po_Find)){
					//开始查找Key和Value
					char* p_Key;
					long mStart_Key = po_Find;
					long mLen_Key;
					//判断结束新行或则字符串结束。		
					while (isSpace(IniInfoBuf, po_Find)) {
						po_Find++;
					}		
					mStart_Key = po_Find;
					//判断是否为注释,是的话重新查找开始判断
					if (';' != IniInfoBuf[po_Find++]){
						char* p_Value;
						long mStart_Value;
						long mLen_Value;
						//查找KEY字符串结束、去空格
						while (!isEndKey(IniInfoBuf, po_Find)){
							po_Find++;
						}
						if (isEndFile(po_Find))break;
						mLen_Key = po_Find - mStart_Key;
						p_Key = (char*)malloc(mLen_Key + 1);
						memset(p_Key, 0, mLen_Key + 1);
						memcpy(p_Key, &IniInfoBuf[mStart_Key], mLen_Key);
						p_Key = trim(p_Key);
						//获取等号
						while (IniInfoBuf[po_Find++] != '='){
							if (isEndFile(po_Find))break;
						}
						//获取Value
						while (isSpace(IniInfoBuf,po_Find)){
							po_Find++;
						}
						mStart_Value = po_Find;
						while (!isEndValue(IniInfoBuf, po_Find)){
							po_Find++;
						}
						//查找Value字符串结束、去空格
						if (isEndFile(po_Find))break;
						mLen_Value = po_Find - mStart_Value ;
						p_Value = (char*)malloc(mLen_Value + 1);
						memset(p_Value, 0, mLen_Value + 1);
						memcpy(p_Value, &IniInfoBuf[mStart_Value], mLen_Value);
						p_Value = trim(p_Value);
						Util_Char::ReplaceChar(p_Value,0x0D,0x01);
						Util_Char::ReplaceChar(p_Value, 0x0A, 0x02);
						//添加到列表，
					//	_MSG("FindKey:%s,%s\n", p_Key, p_Value);
						addStrValue(p_SecName, p_Key, p_Value);			
						while (isSpace(IniInfoBuf, po_Find)){
							po_Find++;
						}
					}
				}
			}
		}
		msg("Find INI End\n", iniPath);
		return 0;
	}
/*
..
..
*/
	void SaveIni(char* in_Path){
		assert(iniPath != NULL);
		int m_EndF = 0;
		FILE* m_fd = fopen(in_Path, "wb+");
		char* mBuf = (char*)malloc(10 * 1024);
		int m_i = 0;
		while (ArraySection->Get(m_i)!= NULL){
			memset(mBuf, 0, 10 * 1024);
			sprintf(mBuf, 1024, "[%s]\r\n", ArraySection->Get(m_i)->SecName);
			Util_File::fwrite(m_fd, mBuf);
			int m_j = 0;
			while (ArraySection->Get(m_i)->Key->Get(m_j) != NULL){
				memset(mBuf, 0, 10 * 1024);
				sprintf(mBuf, 1024, "%s = %s\r\n", ArraySection->Get(m_i)->Key->Get(m_j)->Name,
				ArraySection->Get(m_i)->Key->Get(m_j)->Value);
				Util_File::fwrite(m_fd, mBuf);
				m_j++;
			}
			m_i++;
		}
		fclose(m_fd);
	}
/*
..设置文件配置,初始化变量.
..
*/
	C_INI(char* in_path){
		iniPath = strdup(in_path);
		if (IniInfoBuf == NULL){
			IniInfoBuf = (char*)malloc(MAX_FILE_SIZE);
		}
		memset(IniInfoBuf, -1, MAX_FILE_SIZE);
	}
/*-----------------------------------------------------------------------------------*/
};