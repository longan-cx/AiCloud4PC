// FileBrowser.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "FileBrowser.h"
#include "MainWnd.h"
#include "MD5.h"
//----------------------------------------------------------------------------------------------------------------------------------------

//程序入口处调用该函数进行内存泄露检测
//程序退出前调用_CrtDumpMemoryLeaks();函数可查看内存泄露的ID号，该ID号可作为Breakpoint的参数，运行时将会在相应位置自动断点
inline  void  EnableMemLeakCheck(int Breakpoint = 0){
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	if (Breakpoint)
		_CrtSetBreakAlloc(1912);
}

#ifdef _DEBUG
#define  new   new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#endif 
#define _CRT_SECURE_NO_DEPRECATE

//----------------------------------------------------------------------------------------------------------------------------------------

void InitResource()
{
	// 资源类型
	CPaintManagerUI::SetResourceType(UILIB_FILE);
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch (CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_RESOURCE:
	{
		strResourcePath += _T("skin\\6he\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
		break;
	}
	case UILIB_ZIP:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		CPaintManagerUI::SetResourceZip(_T("6he.zip"), true);
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_ZIPRESOURCE:
	{
		strResourcePath += _T("skin\\6he\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
		if (hResource != NULL) {
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
			if (hGlobal != NULL) {
				dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
				if (dwSize > 0) {
					CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
					// 加载资源管理器
					CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
				}
			}
			::FreeResource(hResource);
		}
	}
		break;
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	EnableMemLeakCheck();

	//GdiplusStartupInput   gdiplusStartupInput;
	//ULONG_PTR             gdiplusToken;
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	log4cplus::initialize();
	helpers::LogLog::getLogLog()->setInternalDebugging(true);
	SharedFileAppenderPtr append_1(
		new RollingFileAppender(LOG4CPLUS_TEXT("logs/filebrowser.log"), 5 * 1024, 5,
		true, true));
	append_1->setName(LOG4CPLUS_TEXT("First"));
	append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(_T("%d{%m/%d/%y %H:%M:%S}[%p(%t)] - %m[%c:%L]%n"))));
	append_1->getloc();
	Logger::getRoot().addAppender(SharedAppenderPtr(append_1.get()));
	Logger root = Logger::getRoot();
	Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("FileBrowser.cpp"));
	LOG4CPLUS_DEBUG(subTest, "program start");
	
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CPaintManagerUI::SetInstance(hInstance);
	InitResource();

	if (::LoadLibrary(_T("d3d9.dll")) == NULL)
		::MessageBox(NULL, _T("加载 d3d9.dll 失败，一些特效可能无法显示！"), _T("信息提示"), MB_OK | MB_ICONWARNING);

	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL)
		return 0;

	pFrame->Create(NULL, _T("FileBrowser"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 600);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	//GdiplusShutdown(gdiplusToken);
	LOG4CPLUS_DEBUG(subTest, "program exit");
	log4cplus::Logger::shutdown();

	_CrtDumpMemoryLeaks();
	return 0;
}
