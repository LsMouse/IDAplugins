#include "Util_LIB.H"
class Util_Base{
public:
	static void* Alloc(ulong inSize){
		void* out = malloc(inSize+1);
		memset(out, 0, inSize + 1);
		return out;
	}
};

