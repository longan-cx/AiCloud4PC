// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4244 4018)
#include "targetver.h"
//  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#define WIN32_LEAN_AND_MEAN             
// Windows ͷ�ļ�: 
#include <windows.h>
#include <commdlg.h>
// C ����ʱͷ�ļ�
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


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
