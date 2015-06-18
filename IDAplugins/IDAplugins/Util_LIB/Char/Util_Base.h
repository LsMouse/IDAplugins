#include <Util_LIB.HPP>
class Util_Base{
public:
/**
* @See	获取已清空的内存
* @Param　inSize -> 传入内存获取宽度
* @Return　out_Mem -> 申请的内存起始地址
*/
	static void* Alloc(ulong inSize){
		void* out_Mem = malloc(inSize+1);
		memset(out_Mem, 0, inSize + 1);
		return out_Mem;
	}
/**
* @See 
* @Class 
* @Struct
* @Function 
* @Param 
* @Return 
* @Since
*/
};

