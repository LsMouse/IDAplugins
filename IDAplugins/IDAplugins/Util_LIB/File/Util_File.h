#include "Util_LIB.H"
class Util_File{
public:
/**/
	static ssize_t fwrite(FILE* in_fd,char* in_Str){
		ssize_t m_writeLen = 0;
		int m_int = 0;
		qfwrite(in_fd, in_Str, strlen(in_Str));
		return m_writeLen;
	}

};

