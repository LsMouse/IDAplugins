#include "Util_LIB.H"
class AskUI{
public:
	static char* GetFileName() {
		char* mNote = (char*)malloc(_MAX_PATH);
		memset(mNote, 0, _MAX_PATH);
		get_root_filename(mNote, _MAX_PATH);
		return mNote;
	}
};

