// MainWnd.cpp

#include "StdAfx.h"
#include "MainWnd.h"
#include "SkinFrame.h"
#include "Resource.h"
#include "MsgWnd.h"
#include "DuiPhotoView.h"

Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("CMainWnd.cpp"));
#define log(x) LOG4CPLUS_DEBUG(subTest, x);

BOOL CMainWnd::Receive(SkinChangedParam param)
{
	CControlUI* pRoot = m_pm.FindControl(_T("root"));
	if (pRoot != NULL) {
		if (param.bColor) {
			pRoot->SetBkColor(param.bkcolor);
			pRoot->SetBkImage(_T(""));
		}
		else {
			pRoot->SetBkColor(0);
			pRoot->SetBkImage(param.bgimage);
			//m_pm.SetLayeredImage(param.bgimage);
		}
	}
	return TRUE;
}

CMainWnd::CMainWnd()	
{
	m_pMenu = NULL;
}

CDuiString CMainWnd::GetSkinFile()
{
	try
	{
		return _T("XML_MAIN");
	}
	catch (...)
	{
		throw _T("CMainWnd::GetSkinFile");
	}
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	try
	{
		return _T("MainWnd");
	}
	catch (...)
	{
		throw _T("CMainWnd::GetWindowClassName");
	}
}

void CMainWnd::InitWindow()
{
	log("InitWindow");

	try
	{
		

		WindowImplBase::InitWindow();

		initSystemControls();
		initFileManagerControls();
		initDownLoadManagerControls();
		initUpLoadManagerControls();
		initHistroyManagerControls();

		initLeftBarView();	
		initFileData();
		initFileView();
		initListView();
		initTrList();
		refurbishFileOption();
	
		// 注册托盘图标
		m_trayIcon.CreateTrayIcon(m_hWnd, IDI_FILEBROWSER, _T("文件管理器"));

		CSkinManager::GetSkinManager()->AddReceiver(this);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(subTest, "CMainWnd::Init");
		throw _T("CMainWnd::Init");
	}
}

bool CMainWnd::findLeftBtn(CControlUI* p)
{
	for (size_t i = 0; i < 11; i++){
		if (p == arrBtn[i]){
			return true;
		}
	}
	return false;
}

LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_trayIcon.DeleteTrayIcon();
	bHandled = FALSE;
	// 退出程序
	if (m_pFileListView){
		m_pFileListView->SetDelayedDestroy(false);
		m_pFileListView->RemoveAll();
	}
	PostQuitMessage(0);
	return 0;
}

void CMainWnd::showLeftBtn(CControlUI* p)
{
	for (size_t i = 0; i < 11; i++){
		CButtonUI *pb = arrBtn[i];
		if (pb == p){
			pb->SetBkColor(pb->GetHotBkColor() - 16);
			pb->SetTextColor(0x3A93FF);
			CDuiString sText;
			sText.Format(_T("%s >"), pb->GetText());
			m_pBtnRoot->SetText(sText);
			if (pb == m_pTrDownload ){
				pb->SetNormalImage(_T("file='tr/download_btn.png' source='165,0,330,38' corner='0,0,0,0'"));
				selectRightView(m_pTrDownloadVL);
			}else			
			if (pb == m_pTrUpload){
				pb->SetNormalImage(_T("file='tr/upload_btn.png' source='165,0,330,38' corner='0,0,0,0'"));
				selectRightView(m_pTrUploadVL);
			}else				
			if (pb == m_pTrOver){
				pb->SetNormalImage(_T("file='tr/history_btn.png' source='165,0,330,38' corner='0,0,0,0'"));
				selectRightView(m_pTrHistoryVL);
			}
			else if (pb == m_pTrList){
				selectRightView(m_pTrDownloadVL);
			}
			else{
				selectRightView(m_pAllFileVL);
			}
				
		}
		else{
			pb->SetBkColor(0xffffff);
			pb->SetTextColor(0x000000);
			if (pb == m_pTrDownload)
				pb->SetNormalImage(_T("file='tr/download_btn.png' source='0,0,165,38' corner='0,0,0,0'"));
			if (pb == m_pTrUpload)
				pb->SetNormalImage(_T("file='tr/upload_btn.png' source='0,0,165,38' corner='0,0,0,0'"));
			if (pb == m_pTrOver)
				pb->SetNormalImage(_T("file='tr/history_btn.png' source='0,0,165,38' corner='0,0,0,0'"));
		}
	}
}

void CMainWnd::Notify( TNotifyUI& msg )
{
	CDuiString name = msg.pSender->GetName();
	if( msg.sType == _T("click") )
	{
		if (msg.pSender == m_pSkinBtn) {
			new CSkinFrame(m_hWnd, m_pSkinBtn);
		}
		else if (msg.pSender == m_pShowThumb){
			//m_pExplorerHL->SetBkImage(_T("main/vritual.png"));
			m_pShowList->SetVisible(true);
			m_pShowThumb->SetVisible(false);
			m_pExplorerHL->SetVisible(false);
			m_pFileListView->SetVisible(true);
			//initFileView();
			//initListView();
		}
		else if (msg.pSender == m_pShowList){
			//m_pExplorerHL->SetBkImage(_T("main/vritual2.png"));
			m_pShowList->SetVisible(false);
			m_pShowThumb->SetVisible(true);
			m_pExplorerHL->SetVisible(true);
			m_pFileListView->SetVisible(false);
			//initFileView();
			//initListView();
		}
		else if (findLeftBtn(msg.pSender)){
			showLeftBtn(msg.pSender);
		}
		else if (msg.pSender == m_pUploadBtn){
			uploadFile();
		}
		else if (msg.pSender == m_pDeleteBtn){
			deleteFile();
		}
		else if (msg.pSender == m_pDownloadBtn){
			downloadFile();
		}
		else if (msg.pSender == m_pCraetDirBtn){
			createDir();
		}
	}
	else if (msg.sType == _T("headerclick")){
		if (name == _T("headercheck")){
			allSelectListItem(msg.pSender);
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		if (0 == msg.pSender->GetName().CompareNoCase(_T("listitem_option"))){
			showListItemView(msg.pSender->GetParent()->GetParent(), 1);
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		
	}
	else if (msg.sType == _T("itemclick"))
	{
		showListItemView(msg.pSender);
	}

	WindowImplBase::Notify(msg);
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	try
	{
		WindowImplBase::OnFinalMessage(hWnd);
		PostQuitMessage(0);
		delete this;
	}
	catch (...)
	{
		throw _T("CMainWnd::OnFinalMessage");
	}
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	
	if (uMsg == UIMSG_TRAYICON)
	{
		UINT uIconMsg = (UINT)lParam;
		if (uIconMsg == WM_LBUTTONUP) {
			BOOL bVisible = IsWindowVisible(m_hWnd);
			::ShowWindow(m_hWnd, !bVisible ? SW_SHOW : SW_HIDE);
		}
		else if (uIconMsg == WM_RBUTTONUP) {
			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			point.y -= 100;
			m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);
			// 动态添加后重新设置菜单的大小
			m_pMenu->ResizeMenu();
		}
	}
	else if (uMsg == WM_MENUCLICK)
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		if (pMenuCmd != NULL)
		{
			BOOL bChecked = pMenuCmd->bChecked;
			CDuiString sMenuName = pMenuCmd->szName;
			CDuiString sUserData = pMenuCmd->szUserData;
			CDuiString sText = pMenuCmd->szText;
			m_pm.DeletePtr(pMenuCmd);

			if (sMenuName.CompareNoCase(_T("lan")) == 0)
			{
				static bool bEn = false;
				if (!bEn) {
					CResourceManager::GetInstance()->SetLanguage(_T("en"));
					CResourceManager::GetInstance()->LoadLanguage(_T("lan_en.xml"));
				}
				else {
					CResourceManager::GetInstance()->SetLanguage(_T("cn_zh"));
					CResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
				}
				bEn = !bEn;
				CResourceManager::GetInstance()->ReloadText();
				InvalidateRect(m_hWnd, NULL, TRUE);
				m_pm.NeedUpdate();
			}
			else if (sMenuName == _T("qianting"))
			{
				if (bChecked)
				{
					CMsgWnd::MessageBox(m_hWnd, NULL, _T("你预定修潜艇服务"));
				}
				else
				{
					CMsgWnd::MessageBox(m_hWnd, NULL, _T("你取消修潜艇服务"));
				}
			}
			else if (sMenuName == _T("exit")) {
				Close(0);
			}
			else
			{
				CMsgWnd::MessageBox(m_hWnd, NULL, sText);
			}
		}
		bHandled = TRUE;
		return 0;
	}

	return lRes;
}

void CMainWnd::initTrList()
{
	CDialogBuilder builder1;

	m_pDonwloadListView->SetDelayedDestroy(false);
	m_pDonwloadListView->RemoveAll();
	m_pUploadListView->SetDelayedDestroy(false);
	m_pUploadListView->RemoveAll();
	m_pHistoryListView->SetDelayedDestroy(false);
	m_pHistoryListView->RemoveAll();

	for (size_t i = 0; i < tr_datas.size(); i++)
	{
		CDuiString sName;
		sName.Format(_T("%s_%d"), a2w((char*)tr_datas[i].md5.data()), tr_datas[i].tr_type);
		CListContainerElementUI* pListItem = NULL;
		CControlUI* pControl = NULL;
		switch (tr_datas[i].tr_type){
		case TR_TYPE_DOWNLOAD:
		case TR_YTPE_UPLOAD:
			pListItem = (CListContainerElementUI*)builder1.Create(_T("tr_listitem.xml"), NULL, this, &m_pm, NULL);
			pListItem->SetName(sName);
			if (tr_datas[i].tr_type == TR_YTPE_UPLOAD)m_pUploadListView->Add(pListItem);
			else m_pDonwloadListView->Add(pListItem);

			pControl = pListItem->FindSubControl(_T("tr_list_image"));
			if (pControl != NULL)
			{
				pControl->SetBkImage(a2w((char*)tr_datas[i].middle_image.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_list_filename"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].name.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_list_trtime"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].tr_time.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_list_trprogress"));
			if (pControl != NULL)
			{
				CProgressUI *pp = (CProgressUI *)pControl;
				pp->SetValue(tr_datas[i].tr_progress);
			}
			pControl = pListItem->FindSubControl(_T("tr_list_progress_text"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].tr_progress_text.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_list_progress_speed"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].tr_speed.data()));
			}
			break;
		case TR_YTPE_NOR:
			pListItem = (CListContainerElementUI*)builder1.Create(_T("tr_history_listitem.xml"), NULL, this, &m_pm, NULL);
			pListItem->SetName(sName);
			m_pHistoryListView->Add(pListItem);
			pControl = pListItem->FindSubControl(_T("tr_history_listitem_name"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].name.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_history_listitem_size"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].size_txt.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_history_listitem_overtime"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].tr_overtime.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_history_listitem_status_text"));
			if (pControl != NULL)
			{
				pControl->SetText(a2w((char*)tr_datas[i].tr_status_text.data()));
			}
			pControl = pListItem->FindSubControl(_T("tr_history_listitem_image"));
			if (pControl != NULL)
			{
				pControl->SetBkImage(a2w((char*)tr_datas[i].middle_image.data()));
			}
			break;
		}
	}
}


void CMainWnd::initListView()
{
	CDialogBuilder builder1;

	m_pFileListView->SetDelayedDestroy(false);
	m_pFileListView->RemoveAll();
	refurbishFileOption();
	initStatusBar();
	
	for (size_t i = 0; i < datas.size(); i++)
	{
		CListContainerElementUI* pListItem = (CListContainerElementUI*)builder1.Create(_T("listitem.xml"), NULL, this, &m_pm, NULL);
		pListItem->SetName(a2w((char*)datas[i].md5.data()));
		m_pFileListView->Add(pListItem);
		CControlUI* pLabel = pListItem->FindSubControl(_T("listitem_filename"));
		if (pLabel != NULL)
		{
			pLabel->SetText(a2w((char*)datas[i].name.data()));
		}
		CControlUI* pTime = pListItem->FindSubControl(_T("listitem_time"));
		if (pTime != NULL){
			pTime->SetText(a2w((char*)datas[i].create_time.data()));
		}
		CControlUI* pSize = pListItem->FindSubControl(_T("listitem_size"));
		if (pSize != NULL){
			CDuiString sSize;
			sSize.Format(_T("%s"), a2w((char*)datas[i].size_txt.data()));
			pSize->SetText(sSize);
		}

		CControlUI* pOption = pListItem->FindSubControl(_T("listitem_option"));
		if (pOption != NULL) {
			COptionUI *pp = (COptionUI*)pOption;
			pp->Selected(datas[i].isselect);
		}

		CControlUI* pImage = pListItem->FindSubControl(_T("listitem_image"));
		if (pImage != NULL) {
			pImage->SetBkImage(a2w((char*)datas[i].small_image.data()));
		}
	}	
}

void CMainWnd::initFileView()
{
	CDialogBuilderCallbackEx callback;
	for (size_t i = 0; i < datas.size(); i++)
	{
		CDialogBuilder builder;
		CControlUI* pFileItem = static_cast<CControlUI*>(builder.Create(_T("fileitem.xml"), (UINT)0, &callback, &m_pm));
		//pFileItem->SetName();
		pFileItem->SetForeImage(a2w((char*)datas[i].image.data()));
		pFileItem->SetText(a2w((char*)datas[i].name.data()));
		m_pFileListHL->Add(pFileItem);
	}
	initStatusBar();
}


void CMainWnd::refurbishFileOption()
{
	bool finded = false;
	for (size_t i = 0; i < datas.size(); i++){
		if (datas[i].isselect){
			finded = true;
			break;
		}
	}

	m_pDownloadBtn->SetEnabled(finded);
	m_pDeleteBtn->SetEnabled(finded);

	if (!m_pUploadBtn->IsEnabled()){
		m_pUploadBtn->SetForeImage(_T("res='sysbtn/upload.png' source='60,0,80,20' dest='5,7,25,27'"));
	}
	else{
		m_pUploadBtn->SetForeImage(_T("res='sysbtn/upload.png' source='0,0,20,20' dest='5,7,25,27'"));
	}
	if (!m_pDownloadBtn->IsEnabled()){
		m_pDownloadBtn->SetForeImage(_T("res='sysbtn/download.png' source='60,0,80,20' dest='5,7,25,27'"));
	}
	else{
		m_pDownloadBtn->SetForeImage(_T("res='sysbtn/download.png' source='0,0,20,20' dest='5,7,25,27'"));
	}
	if (!m_pDeleteBtn->IsEnabled()){
		m_pDeleteBtn->SetForeImage(_T("res='sysbtn/delete.png' source='60,0,80,20' dest='5,7,25,27'"));
	}
	else{
		m_pDeleteBtn->SetForeImage(_T("res='sysbtn/delete.png' source='0,0,20,20' dest='5,7,25,27'"));
	}
	if (!m_pCraetDirBtn->IsEnabled()){
		m_pCraetDirBtn->SetForeImage(_T("res='sysbtn/createdir.png' source='60,0,80,20' dest='5,7,25,27'"));
	}
	else{
		m_pCraetDirBtn->SetForeImage(_T("res='sysbtn/createdir.png' source='0,0,20,20' dest='5,7,25,27'"));
	}
}


void CMainWnd::uploadFile()
{
	std::string path = CMyUtils::getFilePath();
	if (!path.empty()){
		CDuiString sTxt = _T("");
		sTxt.Format(_T("你要上传的文件"));
		CMsgWnd::MessageBox(m_hWnd, sTxt, a2w((char*)path.data()));
	}
}


void CMainWnd::deleteFile()
{
	CDuiString sText = _T("你确定要删除文件:\n");
	for (size_t i = 0; i < datas.size(); i++){
		if (datas[i].isselect){
			sText.Format(_T("%s%s;"), sText.GetData(), a2w((char*)datas[i].name.data()));
			
		}
	}
	
	bool isrefersh = false;
	if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, NULL, sText)){
		for (std::vector<CFileData>::iterator it = datas.begin(); it != datas.end(); ){
			if (it->isselect){
				it = datas.erase(it);
				isrefersh = true;
				
			}
			else{
				++it;
			}			
		}
	}

	if (isrefersh)initListView();
}

void CMainWnd::downloadFile()
{
	
	CMsgWnd::MessageBox(m_hWnd, NULL, _T("下载文件"));
}



void CMainWnd::createDir()
{
	CDuiString strDir = CPaintManagerUI::GetInstancePath();
	strDir += _T("tmp\\images\\");
	CDuiPhotoView::ShowDuiPhotoView(this->m_hWnd, strDir);
}


void CMainWnd::showListItemView(CControlUI* pSender, int flag)
{
	if (pSender != NULL){
		for (size_t i = 0; i < datas.size(); i++)
		{
			if ((pSender->GetName() == a2w((char*)datas[i].md5.data()))){
				CListContainerElementUI* pListItem = (CListContainerElementUI*)pSender;
				COptionUI* pOption = (COptionUI*)pListItem->FindSubControl(_T("listitem_option"));
				if (flag == 0) {	
					datas[i].setSelect(!datas[i].isselect);					
					if (pOption)pOption->Selected(datas[i].isselect);
				}
				else{
					if (pOption){
						datas[i].setSelect(pOption->IsSelected());
						pListItem->Select();
					}
				}
				refurbishFileOption();
			}
		}
	}
}

void CMainWnd::allSelectListItem(CControlUI* pSender)
{
	static bool isselect = false;
	//if (datas.size() == 0) return;
	if (pSender != NULL){
		isselect = !isselect;
		if (!isselect) pSender->SetForeImage(_T("res='sysbtn/cb_normal.png' dest='10,4,26,20'"));
		else pSender->SetForeImage(_T("res='sysbtn/cb_selected.png' dest='10,4,26,20'"));
		for (size_t i = 0; i < datas.size(); i++)
		{
			datas[i].isselect = isselect;
		}
		initListView();
		refurbishFileOption();
	}
}


void CMainWnd::initStatusBar()
{
	size_t fileCount = datas.size();
	if (m_pFileitemCount){
		CDuiString sTxt = _T("");
		sTxt.Format(_T("%d项"), fileCount);
		m_pFileitemCount->SetText(sTxt);
	}

	if (fileCount > 100){
		m_pSelectPageUI->SetVisible(true);
	}
	else{
		m_pSelectPageUI->SetVisible(false);
	}
}


void CMainWnd::selectRightView(CControlUI* pCurrVL)
{
	m_pTrHistoryVL->SetVisible(false);
	m_pTrDownloadVL->SetVisible(false);
	m_pTrUploadVL->SetVisible(false);
	m_pAllFileVL->SetVisible(false);
	pCurrVL->SetVisible();
}


void CMainWnd::initFileData()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < 28; i++){
		int r = (rand() % (19 - 0)) + 0;
		std::string type = CFileData::getFileType((size_t)r);
		CDuiString sText;
		sText.Format(_T("%d.%s"), rand(), a2w((char*)type.data()));

		int gb = 1024 * 1024 * 1024;
		int mb = 1024 * 1024;
		int size = (rand() % (1024 * 2 - 0)) * 1024 * 1024;

		CFileData file(w2a((wchar_t*)sText.GetData()), size, type);
		//if(rand() % 2)file.setSelect(true);
		int status = (rand() % (3 - 0)) + 0;
		file.tr_status = status;
		file.tr_type = status;

		int trsize = (rand() % (size - 0)) * 1024 * 1024;
		file.tr_size = trsize;

		datas.push_back(file);
		tr_datas.push_back(file);
	}
}


void CMainWnd::initLeftBarView()
{
	log("initLeftBarView")
	m_pAllFile = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_allfile")));
	m_pPicture = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_picture")));
	m_pMusic = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_music")));
	m_pDoc = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_doc")));
	m_pOther = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_other")));
	m_pVideo = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_video")));
	m_pTrList = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_trlist")));
	m_pSeed = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_seed")));
	m_pTrDownload = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_tr_download")));
	m_pTrUpload = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_tr_upload")));
	m_pTrOver = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_tr_over")));

	m_pPicture->SetVisible(false);
	m_pMusic->SetVisible(false);
	m_pDoc->SetVisible(false);
	m_pOther->SetVisible(false);
	m_pVideo->SetVisible(false);
	m_pSeed->SetVisible(false);

	arrBtn[0] = m_pAllFile;
	arrBtn[1] = m_pPicture;
	arrBtn[2] = m_pMusic;
	arrBtn[3] = m_pDoc;
	arrBtn[4] = m_pOther;
	arrBtn[5] = m_pVideo;
	arrBtn[6] = m_pTrList;
	arrBtn[7] = m_pSeed;
	arrBtn[8] = m_pTrDownload;
	arrBtn[9] = m_pTrUpload;
	arrBtn[10] = m_pTrOver;
}


void CMainWnd::initSystemControls()
{
	log("initSystemControls")
	m_pSkinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("skinbtn")));
}


void CMainWnd::initFileManagerControls()
{
	log("initFileManagerControls")
	m_pAllFileVL = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("allfile_right")));
	m_pUploadBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_upload")));
	m_pDownloadBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_download")));
	m_pDeleteBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_delete")));
	m_pCraetDirBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_createdir")));
	m_pShowThumb = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_showthumb")));
	m_pShowList = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_showlist")));
	m_pExplorerHL = static_cast<CTileLayoutUI*>(m_pm.FindControl(_T("explorer")));
	m_pFileListHL = static_cast<CTileLayoutUI*>(m_pm.FindControl(_T("filelist")));
	m_pFileListView = static_cast<CListExUI*>(m_pm.FindControl(_T("listview")));
	m_pBtnRoot = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_root")));
	m_pBack = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_back")));
	m_pGo = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_go")));
	m_pFileitemCount = static_cast<CLabelUI*>(m_pm.FindControl(_T("fileitem_count")));
	m_pSelectPageUI = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("select_page_ui")));
}


void CMainWnd::initDownLoadManagerControls()
{
	log("initDownLoadManagerControls")
	m_pDonwloadListView = static_cast<CListExUI*>(m_pm.FindControl(_T("tr_donwload_listview")));
	m_pTrDownloadVL = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("tr_download_right")));

	m_pTrDonwloadSumProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("tr_download_sum_progress")));
	m_pTrDonwloadSumSeep = static_cast<CLabelUI*>(m_pm.FindControl(_T("tr_download_sum_seep_label")));
	m_pTrDonwloadAllPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("tr_download_sum_pause_btn")));
	m_pTrDonwloadAllCanel = static_cast<CButtonUI*>(m_pm.FindControl(_T("tr_download_sum_canel_btn")));
	
}


void CMainWnd::initUpLoadManagerControls()
{
	log("initUpLoadManagerControls")
	m_pHistoryListView = static_cast<CListExUI*>(m_pm.FindControl(_T("tr_history_listview")));
	m_pTrHistoryVL = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("tr_history_right")));

	m_pTrUploadSumProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("tr_upload_sum_progress")));
	m_pTrUploadSumSeep = static_cast<CLabelUI*>(m_pm.FindControl(_T("tr_upload_sum_seep_label")));
	m_pTrUploadAllPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("tr_upload_sum_pause_btn")));
	m_pTrUploadAllCanel = static_cast<CButtonUI*>(m_pm.FindControl(_T("tr_upload_sum_canel_btn")));

}


void CMainWnd::initHistroyManagerControls()
{
	log("initHistroyManagerControls")
	m_pUploadListView = static_cast<CListExUI*>(m_pm.FindControl(_T("tr_upload_listview")));
	m_pTrUploadVL = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("tr_upload_right")));
}
