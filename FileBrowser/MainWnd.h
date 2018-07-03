// MainWnd.h

#pragma once
#include "SkinManager.h"
#include "FileItemUI.h"
#include "FileData.h"

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("FileItem")) == 0) return new CFileItemUI;		
		return NULL;
	}
};

class CMainWnd : public WindowImplBase ,public SkinChangedReceiver
{
public:
	CMainWnd();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const ;
	
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);



private:
	//ϵͳ��ť
	CButtonUI* m_pSkinBtn;
	//�ļ�����ť
	CVerticalLayoutUI* m_pAllFileVL;
	CListUI* m_pFileListView;
	CTileLayoutUI* m_pExplorerHL;
	CTileLayoutUI* m_pFileListHL;
	CButtonUI* m_pUploadBtn;
	CButtonUI* m_pDownloadBtn;
	CButtonUI* m_pDeleteBtn;
	CButtonUI* m_pCraetDirBtn;
	CButtonUI* m_pShowThumb;
	CButtonUI* m_pShowList;
	CButtonUI* m_pBtnRoot;
	CButtonUI* m_pBack;
	CButtonUI* m_pGo;

	//�������ť
	CButtonUI* m_pAllFile;
	CButtonUI* m_pPicture;
	CButtonUI* m_pMusic;
	CButtonUI* m_pDoc;
	CButtonUI* m_pOther;
	CButtonUI* m_pVideo;
	CButtonUI* m_pTrList;
	CButtonUI* m_pSeed;
	CButtonUI* m_pTrDownload;
	CButtonUI* m_pTrUpload;
	CButtonUI* m_pTrOver;
	CButtonUI* arrBtn[11]; //������������߰�ť

	//�ױ�״̬��
	CLabelUI* m_pFileitemCount;
	CHorizontalLayoutUI *m_pSelectPageUI;
	//�Ǳ�
	CTrayIcon m_trayIcon;
	//�˵�
	CMenuWnd* m_pMenu;
	//�ұ߽��沼��
	//�ұ߽��沼��Histroy
	CVerticalLayoutUI* m_pTrHistoryVL;
	CListUI* m_pHistoryListView;
	
	//�ұ߽��沼��DonwLoad
	CVerticalLayoutUI* m_pTrDownloadVL;
	CListUI* m_pDonwloadListView;
	CProgressUI* m_pTrDonwloadSumProgress;
	CLabelUI* m_pTrDonwloadSumSeep;
	CButtonUI* m_pTrDonwloadAllPause;
	CButtonUI* m_pTrDonwloadAllCanel;
	//�ұ߽��沼��Upload
	CVerticalLayoutUI* m_pTrUploadVL;
	CListUI* m_pUploadListView;
	CProgressUI* m_pTrUploadSumProgress;
	CLabelUI* m_pTrUploadSumSeep;
	CButtonUI* m_pTrUploadAllPause;
	CButtonUI* m_pTrUploadAllCanel;

	//�����б�
	std::vector<CFileData> datas; //�ļ��б�
	std::vector<CFileData> tr_datas;//�ļ������б�
private:
	bool findLeftBtn(CControlUI* p);
	void showLeftBtn(CControlUI* p);
	void initListView();
	void initFileView();
	void initTrList();
	void refurbishFileOption();
	void uploadFile();
	void downloadFile();
	void deleteFile();
	
	void createDir();
	void showListItemView(CControlUI* pSender, int flag=0);
	void allSelectListItem(CControlUI* pSender);
	void initStatusBar();
	void selectRightView(CControlUI*);
	void initFileData();
	void initLeftBarView();
	void initSystemControls();
	void initFileManagerControls();
	void initDownLoadManagerControls();
	void initUpLoadManagerControls();
	void initHistroyManagerControls();
};