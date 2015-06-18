#include <Util_LIB.HPP>
namespace Util{
/**
* @See		读取文件一行知道"\r\n"则停止
* @Param	in_fd -> 文件句柄
* @Return	out_line -> 返回读取字符串
*/
	static char* readline(FILE* in_fd){
		char* out_line = (char*)malloc(1024);
		memset(out_line, 0, 1024);
		char c = 0, old_c = 0;
		int len = 0;
		while (((c = fgetc(in_fd)) != EOF)){
			if ((old_c == '\r') || (c == '\n'))
				break;
			out_line[len++] = c;
			old_c = c;
		}
		if (c == EOF){
			free(out_line);
			return NULL;
		}
		return out_line;
	}
/**
* @See		字符串合并
* @Param	dst -> 原字符串
* @Param	src -> 添加字符串
* @Return	out_line -> 返回合并字符串
*/
	static char* strcat(char * dst, const char * src){
		char * cp = dst;
		while (*cp)
			cp++; /* find end of dst */
		while (*cp++ = *src++); /* Copy src to end of dst */
		return(dst); /* return dst */
	}
/**
* @See		字符串字符替换函数
* @Param	sSrc -> 原字符串
* @Param	sMatchStr -> 需要替换字符
* @Param	sReplaceStr -> 替换字符
*/
	static void ReplaceChar(char *sSrc, char sMatchStr, char sReplaceStr){
		int  StringLen = strlen(sSrc);
		while (StringLen){
			if (*(sSrc + StringLen) == sMatchStr){
				*(sSrc + StringLen) = sReplaceStr;
			}
			StringLen--;
		}
	}
/**
* @See		尝试读取一个文件
* @Param	in_fd -> 文件句柄
* @Return	out_line -> 返回读取数据
*/
	static char TryRead(FILE* in_fd){
		char c = fgetc(in_fd);
		fseek(in_fd, -1, SEEK_CUR);
		return c;
	}
/**
* @See		读取每个Seg特殊计算标志(前0x100的和)
* @Param	inStart -> 其实地址
* @Return	OutMark -> 返回检验数据
*/
	static ulong ReadCheck(ea_t inStart) {
		ulong OutMark = 0;
		ulong m_i = 0;
		while (m_i < 0x100){
			OutMark += get_full_byte(inStart + m_i);
			m_i++;
		}
		return OutMark;
	}

};