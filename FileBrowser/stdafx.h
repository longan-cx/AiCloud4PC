// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4244 4018)
#include "targetver.h"
//  从 Windows 头文件中排除极少使用的信息
#define WIN32_LEAN_AND_MEAN             
// Windows 头文件: 
#include <windows.h>
#include <commdlg.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

#include "UIlib.h"
#pragma comment(lib, "DuiLib_d.lib")
using namespace DuiLib;

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loggingmacros.h>
#pragma comment(lib, "log4cplusD.lib")
using namespace log4cplus;

#include "Utils.h"


// TODO:  在此处引用程序需要的其他头文件
