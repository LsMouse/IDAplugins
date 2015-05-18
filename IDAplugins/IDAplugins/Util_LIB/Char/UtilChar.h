#include<Util_LIB.H>
class Util_Char{
public:
	/*
	..	Name : readline
	..读取文件一行知道"\r\n"则停止
	*/
	static char* readline(FILE* in_fd){
		char* line = (char*)malloc(1024);
		memset(line, 0, 1024);
		char c = 0, old_c = 0;
		int len = 0;
		while (((c = fgetc(in_fd)) != EOF)){
			if ((old_c == '\r') || (c == '\n'))
				break;
			line[len++] = c;
			old_c = c;
		}
		if (c == EOF){
			free(line);
			return NULL;
		}
		return line;
	}
	/*
	..	name : strcat
	*/
	static char* strcat(char * dst, const char * src){
		char * cp = dst;
		while (*cp)
			cp++; /* find end of dst */
		while (*cp++ = *src++); /* Copy src to end of dst */
		return(dst); /* return dst */
	}
	/*
	*/
	static void ReplaceChar(char *sSrc, char sMatchStr, char sReplaceStr)
	{
		int  StringLen = strlen(sSrc);
		while (StringLen){
			if (*(sSrc + StringLen) == sMatchStr){
				*(sSrc + StringLen) = sReplaceStr;
			}
			StringLen--;
		}
	}
	/*
	*/
	static void ReplaceStr(char *sSrc, char* sMatchStr, char* sReplaceStr)
	{
		int  StringLen = strlen(sSrc);
		while (StringLen){
			if (*(sSrc + StringLen) == *sMatchStr){
				*(sSrc + StringLen) = *sReplaceStr;
			}
			StringLen--;
		}
	}
	/*
	*/
	static char TryRead(FILE* in_fd){
		char c = fgetc(in_fd);
		fseek(in_fd, -1, SEEK_CUR);
		return c;
	}
};