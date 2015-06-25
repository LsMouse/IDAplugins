#include <Util_LIB.HPP>
//
static int Debug_En = TRUE;
//
namespace Util{
/**
* @See	获取调试输出状态
* @Return Debug_En -> 获取调试输出状态
*/
	static int GetEnable(){
		return Debug_En;
	}
/**
* @See	设置调试输出状态
* @Param inEn -> 设置调试输出状态
*/
	static void SetEnable(int inEn){
		Debug_En = inEn;
	}
/**
* @See	打印数据，与printf语句相似
*/
	static void MSG(const char *format, ...){
		if (GetEnable() == 0)return;
		va_list va;
		va_start(va, format);
		vmsg(format, va);
		va_end(va);
	}
/**
* @See	设置调试输出状态
* @Param inEn -> 设置调试输出状态
*/
	static void MSGHex(void* inData,size_t inSize){
		size_t m_i = 0;
		if (inSize == 0)return;
		if (GetEnable() == 0)return;
		uchar *mdata = (uchar *)inData;
		do{
			_MSG("0x%02X ", *mdata);
			mdata++;
			m_i++;
			if ((m_i % 16) == 0){
				_MSG("\n");
			}
			else if ((m_i % 8) == 0){
				_MSG("\t");
			}
		} while (m_i < inSize);
	}
};

