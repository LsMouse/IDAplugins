#include <Util_LIB.HPP>
class AskUI{
public:
	static char* GetFileName() {
		char* mNote = (char*)malloc(_MAX_PATH);
		memset(mNote, 0, _MAX_PATH);
		get_root_filename(mNote, _MAX_PATH);
		return mNote;
	}
	static char* GetSegmentName(segment_t* inSeg){
		char* _SegName = (char*)Util_Base::Alloc(1024);
		if (get_segm_name(inSeg, _SegName, 1024) == NULL){
			free(_SegName);
			return NULL;
		}
		return _SegName;
	}
};

