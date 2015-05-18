#define __IDA_Notes_C_
#include"IDA_Header.H"
const char ASK_NOTES_UI[] = "Ls Notes \n\n"
				"<文件更新IDA:R>\n"
				"<删除全部SEG段:R>\n"
				"<当前IDA段更新并保存:R>\n"
				"<调试输出:R>>";
enum{
	MODE_FILE_TO_IDA,
	MODE_DEL_ALLSEG,
	MODE_SAVE_FILE,
	MODE_DEBUG_PRINT
}; 


char NotePath[_MAX_PATH];
char workPath[_MAX_PATH];
IDA_Notes::LPNOTE_FILE note = IDA_Notes::new_File();
/*
*/
void IDA_Note_Run(){
	getcwd(workPath, _MAX_PATH);
	memset(NotePath, 0, _MAX_PATH);
	char *buf = (char *)malloc(1024);
	get_root_filename(buf, 1024);
	sprintf(NotePath, _MAX_PATH, "%s\\%s.ini", workPath , buf);
	free(buf);
	msg("File :%s\n", NotePath);
	int mode = 0;
	IDA_Notes::LPNOTE_SEG noteseg = IDA_Notes::Note_ReadIDA(getseg(get_screen_ea()));
	if (AskUsingForm_c(ASK_NOTES_UI, &mode) == 0)return;
	switch (mode){
	case MODE_FILE_TO_IDA:
		Debug_Run(msg("IDA_Debug_ALL IDA_Note_Run  MODE_FILE_TO_IDA\n"));
		note = IDA_Notes::Note_openFile(NotePath);
		noteseg = note->FT_seg;
		while (noteseg){
			segment_t* seg = get_first_seg();
			do{
				IDA_Notes::NoteLoadIDA(noteseg, seg);
				seg = get_next_seg(seg->startEA);
			} while (seg);
			noteseg = noteseg->Next;
		}
	break;
	case MODE_SAVE_FILE:
		Debug_Run(msg("IDA_Debug_ALL IDA_Note_Run  MODE_SAVE_FILE\n"));
		Debug_Run(msg("noteseg->iTemCount:%d\n", noteseg->iTemCount));
		IDA_Notes::add_node(note, noteseg);
		if (note->segCount == 0)return;
		IDA_Notes::Note_Save(NotePath, note);
	break;
	case MODE_DEL_ALLSEG:
		Debug_Run(msg("IDA_Debug_ALL IDA_Note_Run  MODE_DEL_ALLSEG\n"));
		note = IDA_Notes::new_File();
	break;
	case MODE_DEBUG_PRINT:
	break;
	default:return;
	}
	Debug_Run(msg("note->segCount:%d\n", note->segCount));
}