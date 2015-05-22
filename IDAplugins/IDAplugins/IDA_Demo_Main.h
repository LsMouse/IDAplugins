#ifdef __IDA_DEMO_MAIN_C_
int _stdcall IDAP_init(void);
void _stdcall IDAP_term(void);
void _stdcall IDAP_run(int arg);
/*
********************************************************************
*				IDA				Config
*
*
********************************************************************
*/// There isn't much use for these yet, but I set them anyway. 
char IDAP_comment[] = "Ls_IDAplugins";
char IDAP_help[] = "Ls_IDAplugins";
// The name of the plug-in displayed in the Edit->Plugins menu. It can  
// be overridden in the user's plugins.cfg file. 
char IDAP_name[] = "Ls_IDAplugins";
// The hot-key the user can use to run your plug-in. 
char IDAP_hotkey[] = "Alt-1";
// The all-important exported PLUGIN object 
plugin_t PLUGIN = {
	IDP_INTERFACE_VERSION,	// IDA version plug-in is written for  
	0,						// Flags (see below)  
	IDAP_init,				// Initialisation function  
	IDAP_term,				// Clean-up function 	 
	IDAP_run,				// Main plug-in body     	
	IDAP_comment, 			// Comment �unused  
	IDAP_help,				// As above �unused	// Edit->Plugins menu  
	IDAP_name,
	IDAP_hotkey				// Hot key to run the plug-in  
};
/**/
extern int Export_Module();
extern int Import_Module();
extern int Arm_Moudle();
extern int Debug_Moude();
extern int Note_Moudle();
/**/
#endif