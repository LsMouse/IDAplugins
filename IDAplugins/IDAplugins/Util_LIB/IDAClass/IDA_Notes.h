#include "Util_LIB.H"
class IDA_Notes{
/*注释数据保存*/
/*RP标志为0x80000000*/
#define		Nor_Class		0x0000
#define		NorRP_Class		0x8000
#define		Func_Class		0x0001
#define		FuncRP_Class	0x8001
public:
/*保存注释*/
	typedef struct note_info_t{
		struct note_info_t* Prev;
		struct note_info_t* Next;
		ulong Off_ST;
		ulong Off_ED;
		ulong Flags;
		ulong Class;
		char* value;
		char* name;
	} NOTE_INFO, *LPNOTE_INFO;
/*保存段信息和注释*/
	typedef struct note_seg_t{
		struct note_seg_t* Prev;
		struct note_seg_t* Next;
		struct note_info_t* FT_info;
		struct note_info_t* LT_info;
		ulong iTemCount;
		ulong Mark;
		char* Name;
	}NOTE_SEG, *LPNOTE_SEG;
/*保存整个文件信息和注释*/
	typedef struct note_file_t {
		struct note_seg_t* FT_seg;
		struct note_seg_t* LT_seg;
		ulong segCount;
	} NOTE_FILE, *LPNOTE_FILE;
/*
***********************************************************
*对结构体链表统一操作
*NOTE结构体相似才可以使用函数操作
*对结构体链表操作
***********************************************************
*/
/*将item数据加载到mseg*/
	static int add_node(LPNOTE_SEG mSeg, LPNOTE_INFO mInfo){
		if (mInfo == NULL)return -1;
		if (mSeg->iTemCount == 0){
			mSeg->FT_info = mSeg->LT_info = mInfo;
			mSeg->FT_info->Next = NULL;
			mSeg->iTemCount = 1;
			return 0;
		}
		LPNOTE_INFO info = mSeg->FT_info;
		while (info){//处理相同注释进行统一
			if ((info->Class == mInfo->Class) &&
				(strcmp(info->value, mInfo->value) == 0) &&
				(((info->Off_ST + 1) == mInfo->Off_ST) || ((info->Off_ED + 1) == mInfo->Off_ST))){
				info->Off_ED = mInfo->Off_ST;
				return -1;
			}
			info = info->Next;
		}
		mInfo->Prev = mSeg->LT_info;
		mSeg->LT_info->Next = mInfo;
		mSeg->LT_info = mInfo;
		if (mSeg->iTemCount == 1){
			mSeg->FT_info->Next = mInfo;
		}
		mSeg->iTemCount += 1;
		return 0;
	}
	/*将mseg数据加载到mseg*/
	static int add_node(LPNOTE_SEG D_Seg, LPNOTE_SEG S_Seg){
		if (S_Seg == NULL)return -1;
		if (D_Seg->iTemCount == 0)return -1;
		ulong same = 0, diff = 0;
		LPNOTE_INFO S_Info = S_Seg->FT_info;
		while (S_Info){
			LPNOTE_INFO D_Info = D_Seg->FT_info;
			ulong m_flag = 0;
			while (D_Info){
				if ((S_Info->Off_ST == D_Info->Off_ST)
					&& (S_Info->Class == D_Info->Class)){
					D_Info->value = S_Info->value;
					same++;
					m_flag = 1;
					break;
				}
				D_Info = D_Info->Next;
			}
			if (m_flag == 0){
				diff++;
				add_node(D_Seg, S_Info);
			}
			S_Info = S_Info->Next;
		}
		msg("same：%d,diff:%d\n", same, diff);
		return 0;
	}
	/*将mseg数据加载到File*/
	static int add_node(LPNOTE_FILE LpFile, LPNOTE_SEG LpSeg){
		if (LpSeg == NULL)return -1;
		if (LpSeg->iTemCount == 0)return -1;
		if (LpFile->segCount == 0){
			LpFile->segCount = 1;
			LpFile->FT_seg = LpFile->LT_seg = LpSeg;
			LpFile->FT_seg->Next = NULL;
			return 0;
		}
		LPNOTE_SEG mSeg = LpFile->FT_seg;
		while (mSeg){
			if (mSeg->Mark == LpSeg->Mark){//Mark Match
				msg("add_node(LPNOTE_FILE,LPNOTE_SEG),Mark1:0x%x,Mark2:0x%x\n", mSeg->Mark, LpSeg->Mark);
				mSeg->FT_info = LpSeg->FT_info;
				mSeg->iTemCount = LpSeg->iTemCount;
				mSeg->LT_info = LpSeg->LT_info;
				return 0;
			}
			mSeg = mSeg->Next;
		}
		LpSeg->Prev = LpFile->LT_seg;
		LpFile->LT_seg->Next = LpSeg;
		LpFile->LT_seg = LpSeg;
		if (LpFile->segCount == 1){
			LpFile->FT_seg->Next = LpFile->LT_seg;
		}
		LpFile->segCount++;
		return 0;
	}
	static int del_node(LPNOTE_FILE LpFile, segment_t* mSeg){
		if (mSeg == NULL)return -1;
		LPNOTE_SEG seg = Note_ReadIDA(mSeg);
		if (seg->iTemCount == 0)return -1;
		if (LpFile->segCount == 0)return -1;
		LPNOTE_SEG mseg_first = LpFile->FT_seg;
		while (mseg_first){
			if (mseg_first->Mark == seg->Mark){//已存在之前的一样的注释
				LpFile->segCount--;
				if (mseg_first->Prev == NULL){
					if (mseg_first->Next == NULL)LpFile->FT_seg = NULL;
					LpFile->FT_seg = mseg_first->Next;
					return 0;
				}
				mseg_first->Prev->Next = mseg_first->Next;
				return 0;
			}
			mseg_first = mseg_first->Next;
		}
		return 0;
	}
/*
***********************************************************
*				新建结构体
***********************************************************
*//*创建一个注释内容*/
	static LPNOTE_INFO new_info_null(){
		LPNOTE_INFO mInfo = (LPNOTE_INFO)malloc(sizeof(NOTE_INFO));
		memset(mInfo, 0, sizeof(NOTE_INFO));
		return mInfo;
	}
	static LPNOTE_INFO new_info(ulong st, ulong en, ulong Class, const char* value){
		LPNOTE_INFO mInfo = (LPNOTE_INFO)malloc(sizeof(NOTE_INFO));
		memset(mInfo, 0, sizeof(NOTE_INFO));
		mInfo->Off_ST = st; mInfo->Off_ED = en;
		mInfo->Class = Class; mInfo->value = strdup(value);
		return mInfo;
	}
	/*创建一个注释段*/
	static LPNOTE_SEG new_Seg(char* name, ulong Mark){
		LPNOTE_SEG mSeg = (LPNOTE_SEG)malloc(sizeof(NOTE_SEG));
		memset(mSeg, 0, sizeof(NOTE_SEG));
		mSeg->Name = strdup(name);
		mSeg->Mark = Mark;
		return mSeg;
	}
	/*创建一个注释文件*/
	static LPNOTE_FILE new_File(){
		LPNOTE_FILE LpFile = (LPNOTE_FILE)malloc(sizeof(NOTE_FILE));
		memset(LpFile, 0, sizeof(NOTE_FILE));
		return LpFile;
	}
/*
***********************************************************
*					保存文件
***********************************************************
*/
	static ulong ReadIDA_Mark(ulong ea){
		ulong m_data = 0;
		ulong m_i = 0;
		while (m_i < 0x100){
			m_data += get_full_byte(ea + m_i);
			m_i++;
		}
		return m_data;
	}
	/*写一个注释段*/
	static void write_seg(FILE* fd, LPNOTE_SEG mSeg){
		LPNOTE_INFO mInfo = mSeg->FT_info;
		fprintf(fd, "name:%s\r\n", mSeg->Name);//写入一个注释段名字
		fprintf(fd, "itemSize:%d\r\n", mSeg->iTemCount);//写入一个注释段中含有注释个数
		fprintf(fd, "Mark:0x%08x\r\n", mSeg->Mark);//写入一个注释段中含有注释个数
		while (mInfo){
			ulong ValueSize = strlen(mInfo->value);
			fprintf(fd, "0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\r\n",
				mInfo->Off_ST, mInfo->Off_ED, mInfo->Flags, mInfo->Class, ValueSize + 2);
			Util_Char::ReplaceChar(mInfo->value, '\r', '#');
			Util_Char::ReplaceChar(mInfo->value, '\n', '~');
			fprintf(fd, "%s\r\n", mInfo->value);
			mInfo = mInfo->Next;
		}
	}
	/*写整个注释文件*/
	static void write_file(FILE* fd, LPNOTE_FILE LpFile){
		LPNOTE_SEG mSeg = LpFile->FT_seg;
		fprintf(fd, "segCount:%d\r\n", LpFile->segCount);//写入文件链表个数
		while (mSeg){
			write_seg(fd, mSeg);
			mSeg = mSeg->Next;
		}
		fprintf(fd, "File End\r\n");
		fprintf(fd, "LsMouse !\r\n");
	}
	static void Note_Save(char* path, LPNOTE_FILE LpFile){
		FILE* fd = fopen(path, "wb+");
		if (fd == NULL){
			msg("Note_Save Open File is ERR!\n\n");
			return;
		}
		write_file(fd, LpFile);
		fclose(fd);
	}
/*
***********************************************************
*					读取文件
***********************************************************
*//*读单个注释内容*/
	static LPNOTE_INFO read_info(FILE* fd){
		ulong mST = 0, mED = 0, Flags = 0, Class = 0, ValueSize = 0;
		int m_i = 0;
		char* value;
		fscanf(fd, "0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\r\n",
			&mST, &mED, &Flags, &Class, &ValueSize);
		value = Util_Char::readline(fd);
		Util_Char::ReplaceChar(value, '#', '\r');
		Util_Char::ReplaceChar(value, '~', '\n');
		memset(value + strlen(value) - 2, 0, 2);
		LPNOTE_INFO mInfo = new_info(mST, mED, Class, value);
		free(value);
		return mInfo;
	}
/*读单个段注释内容*/
	static LPNOTE_SEG read_seg(FILE* fd){
		ulong m_i = 0, itemSize = 0;
		ulong Mark = 0;
		char* name = (char*)malloc(1024); memset(name, 0, 1024);
		fscanf(fd, "name:%s\r\n", name);//读取文件名字
		fscanf(fd, "itemSize:%d\r\n", &itemSize);//读取Item个数
		fscanf(fd, "Mark:0x%08x\r\n", &Mark);//读取一个注释段中含有注释个数
		msg("name:%s,Size:0x%x,Mark:0x%x\r\n", name, itemSize, Mark);
		LPNOTE_SEG mSeg = new_Seg(name, Mark);
		while (m_i < itemSize){
			LPNOTE_INFO mInfo = read_info(fd);
			add_node(mSeg, mInfo);
			m_i++;
		}
		free(name);
		return mSeg;
	}
	/*读整个文件内容*/
	static LPNOTE_FILE read_file(FILE* fd){
		ulong segCount = 0, m_i = 0;
		LPNOTE_FILE LpFile = new_File();
		fscanf(fd, "segCount:%d\r\n", &segCount);//文件链表个数
		while (m_i < segCount){
			LPNOTE_SEG mSeg = read_seg(fd);
			add_node(LpFile, mSeg);
			m_i++;
		}
		return LpFile;
	}
	/*创建一个Note 文件结构，并返回*/
	static LPNOTE_FILE Note_openFile(char* path){
		FILE* fd = fopen(path, "rb");
		LPNOTE_FILE LpFile = new_File();
		if (fd == NULL){
			msg("文件打开失败\n");
			return LpFile;
		}
		msg("文件打开成功\n");
		fseek(fd, 0, SEEK_SET);
		LpFile = read_file(fd);
		fclose(fd);
		return LpFile;
	}
/*
***********************************************************
*					与IDA相关函数
***********************************************************
*//*读取IDA的注释*/
	static LPNOTE_SEG Note_Read_IDA(char* name, ulong ST, ulong END){
		ulong m_i = 0;
		ulong Mark = ReadIDA_Mark(ST);
		LPNOTE_SEG mSeg = new_Seg(name, Mark);
		while (m_i < (END - ST)){
			char* str_note = (char*)malloc(1024);
			memset(str_note, 0, 1024);
			if (get_cmt(ST + m_i, 0, str_note, 1024) > 2){
				LPNOTE_INFO mInfo = new_info(m_i, 0, Nor_Class, str_note);
				add_node(mSeg, mInfo);
			}
			memset(str_note, 0, 1024);
			if (get_cmt(ST + m_i, 1, str_note, 1024) > 2){
				LPNOTE_INFO mInfo = new_info(m_i, 0, NorRP_Class, str_note);
				add_node(mSeg, mInfo);
			}
			func_t* fun = get_func(ST + m_i);
			if (fun != NULL){
				if ((ST + m_i) != fun->startEA){
					char* func_note = get_func_cmt(fun, 0);
					char *no_note = "No Note";
					if (func_note == NULL){
						LPNOTE_INFO mInfo = new_info(m_i, fun->endEA - ST, Func_Class, no_note);
						add_node(mSeg, mInfo);
					}
					else if (strlen(func_note) < 2){
						LPNOTE_INFO mInfo = new_info(m_i, fun->endEA - ST, Func_Class, no_note);
						add_node(mSeg, mInfo);
					}
					else{
						LPNOTE_INFO mInfo = new_info(m_i, fun->endEA - ST, Func_Class, func_note);
						add_node(mSeg, mInfo);
					}
					func_note = get_func_cmt(fun, 1);
					if (func_note != NULL){
						if ((strlen(func_note) >1)){
							LPNOTE_INFO mInfo = new_info(m_i, fun->endEA - ST, FuncRP_Class, func_note);
							add_node(mSeg, mInfo);
						}
					}
				}
			}
			m_i++;
			free(str_note);
		}
		return mSeg;
	}
/*读取IDA SEG段的注释*/
	static LPNOTE_SEG Note_ReadIDA(segment_t* seg){
		char* segName = (char*)malloc(1024);
		memset(segName, 0, 1024);
		get_true_segm_name(seg, segName, 1024);
		LPNOTE_SEG  mSeg = Note_Read_IDA(segName, seg->startEA, seg->endEA);
		free(segName);
		return mSeg;
	}
	static void NoteLoadIDA(LPNOTE_SEG NoteSeg, segment_t* IDASeg){
		ulong seg_size = IDASeg->endEA - IDASeg->startEA;
		ulong m_i = 0;
		ulong Mark = ReadIDA_Mark(IDASeg->startEA);
		if (Mark != NoteSeg->Mark)return;
		LPNOTE_INFO info = NoteSeg->FT_info;
		while (info){
			if (info->Class == Nor_Class){
				set_cmt(IDASeg->startEA + info->Off_ST, info->value, 0);
			}
			else if (info->Class == NorRP_Class){
				set_cmt(IDASeg->startEA + info->Off_ST, info->value, 1);
			}
			else if (info->Class == Func_Class){
				ua_code((IDASeg->startEA + info->Off_ST) & 0xFFFFFFFFC);
				add_func(IDASeg->startEA + info->Off_ST - 1, IDASeg->startEA + info->Off_ED - 1);
				func_t* fun = get_func(IDASeg->startEA + info->Off_ST);
				if (strcmp(info->value, "No Note") != 0)
					set_func_cmt(fun, info->value, 0);
			}
			else if (info->Class == FuncRP_Class){
				ua_code((IDASeg->startEA + info->Off_ST) & 0xFFFFFFFFC);
				add_func(IDASeg->startEA + info->Off_ST - 1, IDASeg->startEA + info->Off_ED - 1);
				func_t* fun = get_func(IDASeg->startEA + info->Off_ST);
				set_func_cmt(fun, info->value, 1);
			}
			info = info->Next;
		}
	}
/*
***********************************************************
*					Debug
***********************************************************
*//*打印数据*/
	static ssize_t Note_getFileSize(LPNOTE_FILE LpFile){
		ulong m_size = 0;
		LPNOTE_SEG mSeg = LpFile->FT_seg;
		while (mSeg){
			m_size += mSeg->iTemCount;
			mSeg = mSeg->Next;
		}
		return m_size;
	}
	static void Note_printf(LPNOTE_SEG seg, ulong start, ulong end){
		if (seg == NULL)return;
		ulong m_i = 0;
		LPNOTE_INFO mInfo = seg->FT_info;
		while (info != NULL){
			if (m_i > end){}
			else if (m_i < start){}
			else{
				msg("O_S:0x%x,O_E:0x%x,class:0x%x\n"
					"value_size:0x%x,value:%s \n",
					mInfo->Off_ST, mInfo->Off_ED, mInfo->Class,
					strlen(mInfo->value), mInfo->value);
			}
			mInfo = mInfo->Next;
			m_i++;
		}
	}
	static void Note_printfInfo(LPNOTE_FILE mFile){
		LPNOTE_SEG mseg = mFile->FT_seg;
		ulong m_i = 0;
		msg("segCount:%d\n", mFile->segCount);
		while (mseg){
			msg("No.%d,Name=%s,ItemCount:%d\n", m_i, mseg->Name, mseg->iTemCount);
			m_i++;
			mseg = mseg->Next;
		}

	}

};
