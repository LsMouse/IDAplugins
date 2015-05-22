#ifndef __UTIL_LIB_H_
#define __UTIL_LIB_H_
/*标准C_C++头*/
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <ctype.h> 
#include <string.h> 
#include <string> 
#include <Windows.h>
/*IDA插件基本头*/
#include <ida.hpp> 
#include <idp.hpp> 
#include <loader.hpp>
#include <name.hpp>
#include <ua.hpp>
#include <Dbg.hpp>
#include <IDAClass/IDA_SDK.H>
/*包含子类头文件*/
#include <Debug/Util_LIB_Debug.H> 
#include <Char/Util_LIB_Char.H>
#include <File/Util_LIB_File.H>
#include <List/Util_LIB_List.H>
#include <INI/Util_LIB_INI.H>

#include <Linux/Util_LIB_Linux.H>
/**/
#include <IDAClass/Util_LIB_IDAClass.H>
/**/
#endif