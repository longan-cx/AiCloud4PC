#pragma once
#include "DUIWindow.h"
#include "UUCenterUI.h"
#include "UUPhotoListUI.h"

enum EPHOTOTRANSFER
{
	ORIGIN = 1,
	AMPLIFY,
	REDUCE,
};

class CDuiPhotoView : public CDUIWindow
{

public:
	static void ShowDuiPhotoView(HWND hwnd, const CDuiString& strDir){
		vector<CDuiString> paths;	
			
		char szFind[MAX_PATH] = { 0 };
		char szFile[MAX_PATH] = { 0 };		
		WIN32_FIND_DATAA FindFileData;
		strcpy(szFind, w2a((wchar_t*)strDir.GetData()));
		strcat(szFind, "*.*");
		HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE != hFind){
			while (TRUE)
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					if (FindFileData.cFileName[0] != '.')
					{
						strcpy(szFile, w2a((wchar_t*)strDir.GetData()));
						strcat(szFile, "\\");
						strcat(szFile, FindFileData.cFileName);
						CDuiString sFileName;
						sFileName.Append(a2w(szFile));
						paths.push_back(sFileName);
					}
				}
			
				if (!FindNextFileA(hFind, &FindFileData)) break;
			}
			FindClose(hFind);
		}
		if (paths.size() > 0){
			CDuiPhotoView* m_duiPhotoView = new CDuiPhotoView;
			m_duiPhotoView->Create(hwnd, _T("PhotoViewer"), UI_WNDSTYLE_FRAME, 0);
			m_duiPhotoView->SetPhotoList(paths, 0);
			m_duiPhotoView->InitShow();
		}
	}	

public:
	CDuiPhotoView();
	~CDuiPhotoView(){};

#pragma region 窗口的大小限制
private:
	int GetThumbnailSize(){ return 100; }
	int GetMinWidth(){ return 615; }
	int GetMinHeight(){ return 415; }
	CDuiSize screenSize(void) const
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		return CDuiSize(oMonitor.rcWork.right - oMonitor.rcWork.left, oMonitor.rcWork.bottom - oMonitor.rcWork.top);
	}
	int GetMaxWidth(){
		return m_screenSize.cx - 10;
	}
	int GetMaxHeight()
	{
		return m_screenSize.cy - 10;
	}
private:
	CDuiSize m_screenSize;
#pragma endregion 窗口的大小限制
public:
	void InitShow(void);

	void ResetPictureList()
	{
		RECT wndRc = GetCurrentClientRect();
		int wndWidth = wndRc.right - wndRc.left;
		m_perPageNumber = wndWidth / GetThumbnailSize();

		int count = m_fileList.size();
		int mod = count % m_perPageNumber;
		m_pageCount = count / m_perPageNumber;

		int selectIndex = m_currentIndex % m_perPageNumber;
		m_currentPage = m_currentIndex / m_perPageNumber;

		vector<CDuiString> imageList;
		bool isAdd = false;
		int start = m_currentPage * m_perPageNumber;
		int end = start + m_perPageNumber;
		for (size_t i = 0; i < m_fileList.size(); i++)
		{
			if (isAdd)
			{
				if (i == end)
				{
					break;
				}
				imageList.push_back(m_fileList[i]);
			}
			else
			{
				if (i == start)
				{
					isAdd = true;
				}
				if (isAdd)
				{
					imageList.push_back(m_fileList[i]);
				}
			}
		}

		m_pPhotoListUI->ResetImages(imageList);
		m_pPhotoListUI->SetSelectItem(selectIndex);
	}

	virtual void InitWindow() override;

	virtual CDuiString GetSkinFile() override;

	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;

	void SetPhotoList(const vector<CDuiString>& list, int curIndex);

	void OnSelectedImageChanged(int index, CDuiString imagePath);

protected:

	virtual LRESULT OnMouseWheel(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

	virtual LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

	void MoveImage(POINT &pt)
	{
		int xpos = m_currentMovePoint.x - pt.x;
		int ypos = m_currentMovePoint.y - pt.y;
		m_imgpanel->MoveImage(xpos, ypos);	
	}

	virtual LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

	virtual LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

	virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	virtual void OnClick(TNotifyUI& msg) override;

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

private:
	int GetCurrentTransferNumber();
	void DoRotationAction();

	void SetCurrentPhoto(CDuiString &val);

	void SaveAsPicture();

	RECT GetCurrentClientRect()
	{
		RECT rc;
		::GetClientRect(this->GetHWND(), &rc);
		return rc;
	}

	void ResizeWnd()
	{
		//已知图片宽高， 初始化窗口大小， 背景图片大小， 
		if (m_imgpanel->GetCenterImageHeight() < GetMinHeight() && m_imgpanel->GetCenterImageWidth() < GetMinWidth())
		{
			//宽高都小于最小宽高
			//窗口大小不变
		}
		else if (
			m_imgpanel->GetCenterImageWidth() < GetMaxWidth() &&
			m_imgpanel->GetCenterImageWidth() > GetMinWidth() &&
			m_imgpanel->GetCenterImageHeight() < GetMaxHeight() &&
			m_imgpanel->GetCenterImageHeight() > GetMinHeight()
			)
		{
			//宽高都大于最小宽高，但是都小于最大宽高。
			this->ResizeClient(m_imgpanel->GetCenterImageWidth(), m_imgpanel->GetCenterImageHeight());
		}
		else if (
			m_imgpanel->GetCenterImageWidth() < GetMaxWidth() &&
			m_imgpanel->GetCenterImageWidth() > GetMinWidth() &&
			m_imgpanel->GetCenterImageHeight() < GetMinHeight()
			)
		{
			//宽在最大和最小宽之间，高小于最小高
			this->ResizeClient(m_imgpanel->GetCenterImageWidth(), GetMinHeight());
		}
		else if (
			m_imgpanel->GetCenterImageWidth() < GetMinWidth() &&
			m_imgpanel->GetCenterImageHeight() < GetMaxHeight() &&
			m_imgpanel->GetCenterImageHeight() > GetMinHeight()
			)
		{
			//高在最大和最小高之间，宽小于最小宽
			this->ResizeClient(GetMinWidth(), m_imgpanel->GetCenterImageHeight());
		}
		else if (
			m_imgpanel->GetCenterImageWidth() > GetMaxWidth() &&
			m_imgpanel->GetCenterImageHeight() < GetMaxHeight()
			)
		{
			//宽大于最大宽，高小于最大高。
			double k = 1.0 * GetMaxWidth() / m_imgpanel->GetCenterImageWidth();
			int retH = k* m_imgpanel->GetCenterImageHeight();
			if (retH < GetMinHeight())
			{
				this->ResizeClient(GetMaxWidth(), GetMinHeight());
			}
			else
			{
				this->ResizeClient(GetMaxWidth(), retH);
			}
		}
		else if (
			m_imgpanel->GetCenterImageWidth() < GetMaxWidth() &&
			m_imgpanel->GetCenterImageHeight() > GetMaxHeight()
			)
		{
			//宽小于最大宽，高大于最大高。
			double k = 1.0 * GetMaxHeight() / m_imgpanel->GetCenterImageHeight();
			int retW = k* m_imgpanel->GetCenterImageWidth();
			if (retW < GetMinHeight())
			{
				this->ResizeClient(GetMinWidth(), GetMaxHeight());
			}
			else
			{
				this->ResizeClient(retW, GetMaxHeight());
			}
		}
		else if (
			m_imgpanel->GetCenterImageWidth() > GetMaxWidth() &&
			m_imgpanel->GetCenterImageHeight() > GetMaxHeight()
			)
		{
			//宽大于最大宽，高大于最大高。
			double kx = 1.0 * GetMaxWidth() / m_imgpanel->GetCenterImageWidth();
			double ky = 1.0 * GetMaxHeight() / m_imgpanel->GetCenterImageHeight();
			if (kx > ky)
			{
				//按照ky来缩放。
				int retW = ky* m_imgpanel->GetCenterImageWidth();
				if (retW < GetMinHeight())
				{
					this->ResizeClient(GetMinWidth(), GetMaxHeight());
				}
				else
				{
					this->ResizeClient(retW, GetMaxHeight());
				}
			}
			else
			{
				//按照kx来缩放。
				int retH = kx* m_imgpanel->GetCenterImageHeight();
				if (retH < GetMinHeight())
				{
					this->ResizeClient(GetMaxWidth(), GetMinHeight());
				}
				else
				{
					this->ResizeClient(GetMaxWidth(), retH);
				}
			}
		}
		else
		{
			//other 
			//Q_ASSERT(false);
		}
	}


	void SetTransferNumberPos()
	{
		RECT rc;
		::GetWindowRect(this->GetHWND(), &rc);

		int rcWidth = rc.right - rc.left;
		int rcHeight = rc.bottom - rc.top;

		int width = m_transfernumber->GetFixedWidth();
		int height = m_transfernumber->GetFixedHeight();

		RECT ret;
		ret.left = (rcWidth - width) / 2;
		ret.right = ret.left + width;
		ret.top = (rcHeight - height) / 2;
		ret.bottom = ret.top + height;
		m_transfernumber->SetPos(ret);
	}
	void TransferPhoto(EPHOTOTRANSFER val)
	{
		switch (val)
		{
		case ORIGIN:
			m_wheelNumber = 0;
			break;
		case AMPLIFY:
		{
			int val = GetCurrentTransferNumber();
			if (val < 1600)
			{
				m_wheelNumber++;
			}
		}		
			break;
		case REDUCE:
		{	
			int val = GetCurrentTransferNumber();
			if (val > 5)
			{
				m_wheelNumber--;
			}
		}
			break;
		default:
			
			break;
		}		
		TransferAction();

	}

	void TransferAction()
	{
		int ret = GetCurrentTransferNumber();

		if (ret > 1600)
		{
			ret = 1600;
		}
		if (ret < 5)
		{
			ret = 5;
		}

		CDuiString sText;
		sText.Format(_T("%d%"), ret);

		m_numberlabel->SetText(sText);
		m_transfernumber->SetVisible(true);

		KillTimer(this->GetHWND(), 1);
		SetTimer(this->GetHWND(), 1, 500, NULL);

		m_imgpanel->SetCurrentTransferRect(ret);
	}


private:
	//图片panel
	CUUCenterUI* m_imgpanel = NULL;

	//背景panel
	CVerticalLayoutUI* m_bkpanel = NULL;
	//工具栏
	CHorizontalLayoutUI* m_toolbar = NULL;
	CHorizontalLayoutUI* m_picturelistpanel = NULL;
	CHorizontalLayoutUI* m_bottomlistpanel = NULL;
	
	
	
	//放大缩小倍数显示
	CVerticalLayoutUI* m_transfernumber = NULL;

	CLabelUI* m_numberlabel = NULL;
	

	CButtonUI* m_fullscreenclosebtn = NULL;
	CButtonUI* m_closebtn = NULL;
	CButtonUI* m_leftbtn = NULL;
	CButtonUI* m_fullscreenleftbtn = NULL;
	CButtonUI* m_rightbtn = NULL;
	CButtonUI* m_fullscreenrightbtn = NULL;

	CButtonUI* m_uppagebtn = NULL;
	CButtonUI* m_downpagebtn = NULL;

	CUUPhotoListUI* m_pPhotoListUI = NULL;

	//滚轮值
	int m_wheelNumber = 0;
	//循环变化比例
	vector<int> m_changeList;

	//是否长按鼠标左键
	bool m_isHold = false;
	//是否能拖动图片。
	bool m_isInImagePanel = false;

	//长按鼠标左键拖动时的点。
	POINT m_HoldPoint;

	POINT m_currentMovePoint;

	//鼠标追踪
	BOOL m_bMouseTrack = TRUE;

	vector<CDuiString> m_fileList;
	int m_currentIndex;	
	int m_rotationCount = 0;

	bool m_isChanged = false;

	//是否全屏
	bool m_isZoom = false;


	//当前页
	int m_currentPage = 1;
	//总页数
	int m_pageCount = 1;
	//每页显示多少图片
	int m_perPageNumber = 0;
};
