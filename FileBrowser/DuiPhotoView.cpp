#include "stdafx.h"
#include "DuiPhotoView.h"
#include <CommCtrl.h>
#include "FileDialogEx.h"
#include <wingdi.h>
#include "UUPhotoListUI.h"

#pragma comment(lib, "Comctl32.lib")

CDuiPhotoView::CDuiPhotoView()
{
	m_screenSize = screenSize();
	m_changeList.push_back(3);
	m_changeList.push_back(5);
	m_changeList.push_back(7);
}

void CDuiPhotoView::InitWindow()
{
	m_imgpanel = static_cast<CUUCenterUI*>(m_pm.FindControl(_T("imgpanel")));
	m_bkpanel = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("bkpanel")));
	m_toolbar = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("toolbar")));
	m_picturelistpanel = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("picturelistpanel")));
	m_bottomlistpanel = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("bottomlistpanel")));
	

	m_transfernumber = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("transfernumber")));
	m_numberlabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("numberlabel")));

	m_fullscreenclosebtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("fullscreenclosebtn")));
	m_closebtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));

	m_fullscreenleftbtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("fullscreenleftbtn")));
	m_leftbtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("leftbtn")));

	m_fullscreenrightbtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("fullscreenrightbtn")));
	m_rightbtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("rightbtn")));


	m_uppagebtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("uppagebtn")));
	m_downpagebtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("downpagebtn")));
	
	__super::InitWindow();
}

DuiLib::CDuiString CDuiPhotoView::GetSkinFile()
{
	return _T("XML_IMAGE");
}


CControlUI* CDuiPhotoView::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("UUCenter")) == 0)
	{
		return new CUUCenterUI();
	}
	return __super::CreateControl(pstrClass);
}

void CDuiPhotoView::InitShow(void)
{
	m_imgpanel->SetCenterImage(m_fileList[m_currentIndex]);
	//////////////////////////////////////////////////////////////////////////
	ResizeWnd();

	//设置背景panel 的位置。
	RECT wndRc = GetCurrentClientRect();
	m_bkpanel->SetPos(wndRc);
	m_imgpanel->SetPos(wndRc);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	//初始化控件显示状态
	m_toolbar->SetVisible(false);
	m_leftbtn->SetVisible(false);
	m_rightbtn->SetVisible(false);
	m_transfernumber->SetVisible(false);

	if (m_isZoom)
	{
		m_fullscreenclosebtn->SetVisible(true);
		m_closebtn->SetVisible(false);
	}
	else
	{
		m_fullscreenclosebtn->SetVisible(false);
		m_closebtn->SetVisible(true);
	}
	m_fullscreenleftbtn->SetVisible(false);
	m_fullscreenrightbtn->SetVisible(false);

	//设置图片位置。
	SetCurrentPhoto(m_fileList[m_currentIndex]);
	//居中显示窗口。
	ShowWindow();
	CenterWindow();
	//////////////////////////////////////////////////////////////////////////
}

void CDuiPhotoView::SetPhotoList(const vector<CDuiString>& list, int curIndex)
{
	m_fileList = list;
	m_currentIndex = curIndex;

	if (m_pPhotoListUI)
	{
		delete m_pPhotoListUI;
		m_pPhotoListUI = NULL;
	}
	m_pPhotoListUI = new CUUPhotoListUI();
	m_pPhotoListUI->Init(GetThumbnailSize(), this);
	m_picturelistpanel->Add(m_pPhotoListUI);	
}

LRESULT CDuiPhotoView::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//鼠标滚轴事件

	int delta = GET_WHEEL_DELTA_WPARAM(wParam);

	if (delta == 0)
	{
		return __super::OnMouseWheel(uMsg, wParam, lParam, bHandled);
	}
	if (delta > 0)
	{
		TransferPhoto(AMPLIFY);
	}
	else
	{
		TransferPhoto(REDUCE);
	}

	return 0;
}

bool m_isdwon = false;

LRESULT CDuiPhotoView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_isChanged = false;
	if (m_bMouseTrack)     // 若允许 追踪，则。 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE | TME_HOVER;
		csTME.hwndTrack = this->GetHWND();// 指定要 追踪 的窗口 
		csTME.dwHoverTime = 1;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
		::_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持 
		m_bMouseTrack = FALSE;   // 若已经 追踪 ，则停止 追踪 
	}

	// 	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));

	bHandled = FALSE;
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };


	if (pt.x > 0 && pt.x < GetCurrentClientRect().right&&
		pt.y>0 && pt.y < GetCurrentClientRect().bottom
		)
	{
		m_toolbar->SetVisible(true);
	}
	else
	{
		m_toolbar->SetVisible(false);
	}


	if (m_isZoom)
	{
		if (0 < GetCurrentClientRect().bottom - pt.y && GetCurrentClientRect().bottom - pt.y < 200)
		{
			//m_bottomlistpanel->SetVisible(true);
			m_bottomlistpanel->SetVisible(false);
		}
		else
		{
			m_bottomlistpanel->SetVisible(false);
		}
	}


	if (m_isZoom)
	{
		if (0 < GetCurrentClientRect().bottom - pt.y && GetCurrentClientRect().bottom - pt.y < 100&&
			0 < pt.x && pt.x < 100 
			)
		{
			RECT valRc = m_uppagebtn->GetPos();
			m_uppagebtn->SetVisible(true);
		}
		else
		{
			m_uppagebtn->SetVisible(false);
		}

		if (0 < GetCurrentClientRect().bottom - pt.y && GetCurrentClientRect().bottom - pt.y < 100 &&
			0 < GetCurrentClientRect().right - pt.x && GetCurrentClientRect().right - pt.x < 100
			)
		{
			m_downpagebtn->SetVisible(true);
		}
		else
		{
			m_downpagebtn->SetVisible(false);
		}
	}


	if (0 < pt.x && pt.x < 100)
	{
		if (m_isZoom)
		{
			m_leftbtn->SetVisible(false);
			m_fullscreenleftbtn->SetVisible(true);
		}
		else
		{
			m_leftbtn->SetVisible(true);
			m_fullscreenleftbtn->SetVisible(false);
		}
	}
	else
	{
		m_leftbtn->SetVisible(false);
		m_fullscreenleftbtn->SetVisible(false);
	}

	if (0 < GetCurrentClientRect().right - pt.x && GetCurrentClientRect().right - pt.x < 100)
	{
		if (m_isZoom)
		{
			m_rightbtn->SetVisible(false);
			m_fullscreenrightbtn->SetVisible(true);
		}
		else
		{
			m_rightbtn->SetVisible(true);
			m_fullscreenrightbtn->SetVisible(false);
		}
	}
	else
	{
		m_rightbtn->SetVisible(false);
		m_fullscreenrightbtn->SetVisible(false);
	}
	if (m_isHold)
	{
		if (m_isInImagePanel)
		{
			MoveImage(pt);
			m_currentMovePoint = pt;
		}
		else
		{
			//移动窗口
			RECT rc;
			::GetWindowRect(this->GetHWND(), &rc);
			int ax = rc.left - (m_HoldPoint.x - pt.x);
			int ay = rc.top - (m_HoldPoint.y - pt.y);

			::SetWindowPos(this->GetHWND(), NULL, ax, ay, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		bHandled = TRUE;
	}
	else
	{
		return __super::OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

LRESULT CDuiPhotoView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI* pControl = this->m_pm.FindControl(pt);

	if (pControl == NULL)
	{
		return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}
	if (
		pControl->GetName() == _T("closebtn") ||
		pControl->GetName() == _T("leftbtn") ||
		pControl->GetName() == _T("rightbtn") ||
		pControl->GetName() == _T("uppagebtn") ||
		pControl->GetName() == _T("downpagebtn")
		)
	{
		return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}

	RECT rect = m_toolbar->GetPos();

	if (PtInRect(&rect, pt))
	{
		return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}
	rect = m_bottomlistpanel->GetPos();

	if (PtInRect(&rect, pt))
	{
		return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}
	m_isHold = true;
	m_HoldPoint = pt;
	m_currentMovePoint = m_HoldPoint;
	RECT retRc = m_imgpanel->GetTransferRect();
	if (PtInRect(&retRc, pt))
	{
		//点击到图片上。
		int retWidth = retRc.right - retRc.left;
		int retHeight = retRc.bottom - retRc.top;

			RECT rc = GetCurrentClientRect();

			int height = rc.bottom - rc.top;
			int width = rc.right - rc.left;

			if (retWidth > width || retHeight > height)
			{
				m_isInImagePanel = true;
			}
			else
			{
				m_isInImagePanel = false;
			}
		}
		else
		{
			m_isInImagePanel = false;
	}

	return __super::OnLButtonDown(uMsg, wParam, lParam, bHandled);
}

LRESULT CDuiPhotoView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_imgpanel->SetCurrentRect(m_imgpanel->GetTransferRect());
	m_isHold = false;
	m_isInImagePanel = false;
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

int CDuiPhotoView::GetCurrentTransferNumber()
{
	int ret = 0;
	int i = m_wheelNumber % 3;
	int j = m_wheelNumber / 3;
	if (m_wheelNumber == 0)
	{
		ret = m_imgpanel->GetCurrentK();
	}
	else if (m_wheelNumber > 0)
	{
		//放大的图。
		ret = m_imgpanel->GetCurrentK() + j * 15 + m_changeList[i];
	}
	else
	{
		//缩小的图。
		ret = m_imgpanel->GetCurrentK() + j * 15 - m_changeList[-i];
	}
	return ret;
}

LRESULT CDuiPhotoView::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bMouseTrack = TRUE;
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

LRESULT CDuiPhotoView::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	switch (uMsg)
	{
	case WM_MOUSELEAVE:
	{
		m_toolbar->SetVisible(false);
		m_leftbtn->SetVisible(false);
		m_rightbtn->SetVisible(false);
		//鼠标离开。
		m_bMouseTrack = TRUE;

		if (m_isChanged)
		{
			TransferAction();
			m_isChanged = false;
		}
		break;
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		//左键双击。
	}
	break;
	case  WM_TIMER:
	{
		UINT   nIDEvent = (UINT)wParam;
		if (nIDEvent == 1)
		{
			m_transfernumber->SetVisible(false);
			KillTimer(this->GetHWND(), 1);
		}
	}
	break;
	default:
		return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

void CDuiPhotoView::OnClick(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("closebtn") ||
		name == _T("fullscreenclosebtn")
		)
	{
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		//::PostQuitMessage(0L);
	}
	else if (name == _T("leftbtn") ||
		name == _T("fullscreenleftbtn"))
	{
		m_wheelNumber = 0;
		if (m_currentIndex > 0)
		{
			m_currentIndex--;
			CDuiString val = m_fileList[m_currentIndex];
			SetCurrentPhoto(val);
		}
	}
	else if (name == _T("rightbtn") ||
		name == _T("fullscreenrightbtn")
		)
	{
		m_wheelNumber = 0;
		if (m_currentIndex < m_fileList.size() - 1)
		{
			m_currentIndex++;
			CDuiString val = m_fileList[m_currentIndex];
			SetCurrentPhoto(val);
		}
	}
	else if (name == _T("amplifybtn"))
	{
		//放大
		TransferPhoto(AMPLIFY);
	}
	else if (name == _T("reducebtn"))
	{
		//缩小
		TransferPhoto(REDUCE);
	}
	else if (name == _T("originbtn"))
	{
		TransferPhoto(ORIGIN);
	}
	else if (name == _T("maxbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		//窗口变大。

	}
	else if (name == _T("restorebtn"))
	{
		//窗口还原
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	else if (name == _T("rotationbtn"))
	{
		DoRotationAction();
	}
	else if (name == _T("saveasbtn"))
	{
		SaveAsPicture();
	}
	else
	{
		__super::OnClick(msg);
	}
}

void CDuiPhotoView::SetCurrentPhoto(CDuiString &val)
{
	if (m_currentIndex == 0)
	{
		m_leftbtn->SetEnabled(false);
	}
	else
	{
		m_leftbtn->SetEnabled(true);
	}
	if (m_currentIndex == m_fileList.size() - 1)
	{
		m_rightbtn->SetEnabled(false);
		//m_rightbtn->SetToolTip(_T("sss"));
	}
	else
	{
		m_rightbtn->SetEnabled(true);
	}
	m_imgpanel->SetCenterImage(val);
}

LRESULT CDuiPhotoView::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		CControlUI* pControl = m_pm.FindControl(_T("maxbtn"));
		if (pControl) pControl->SetVisible(!!bZoomed);
		pControl = m_pm.FindControl(_T("restorebtn"));
		if (pControl) pControl->SetVisible(!bZoomed);

		RECT wndRc = GetCurrentClientRect();
		RECT oldRc = m_imgpanel->GetPos();
		m_bkpanel->SetPos(wndRc);
		m_imgpanel->SetPos(wndRc);
		m_imgpanel->ChangedCurrentRect(oldRc);
		TransferAction();

		if (::IsZoomed(*this))
		{
			m_isZoom = true;
			//窗口最大化
			m_closebtn->SetVisible(false);
			m_fullscreenclosebtn->SetVisible(true);

			
			RECT pageRc;
			GetWindowRect(this->GetHWND(), &pageRc);

			//RECT upRc = { 0, 0, 0, 0 };
			/*upRc.left = 5;
			upRc.top = wndRc.bottom - 50;
			upRc.right = 35;
			upRc.bottom = wndRc.bottom - 20;*/
			//m_uppagebtn->SetPos(upRc);

			//RECT downRc;
			//downRc.left = pageRc.right - 35;
			//downRc.top = pageRc.bottom - 50;
			//downRc.right = pageRc.right - 5;
			//downRc.bottom = pageRc.bottom - 20;
			//m_downpagebtn->SetPos(downRc);
			//m_downpagebtn->SetFixedWidth(30);
			//m_downpagebtn->SetFixedHeight(30);
			//ResetPictureList();
		}
		else
		{
			m_isZoom = false;
			//窗口正常。
			m_closebtn->SetVisible(true);
			m_fullscreenclosebtn->SetVisible(false);
		}

		m_leftbtn->SetVisible(false);
		m_rightbtn->SetVisible(false);
		m_fullscreenleftbtn->SetVisible(false);
		m_fullscreenrightbtn->SetVisible(false);
	}
	return lRes;

}

void CDuiPhotoView::SaveAsPicture()
{
	CDuiString filename = _T("sss.png");//m_fileList[m_currentIndex];
	CDuiString initDir = _T("C:/");

	LPCTSTR lpszFilter = _T("Images (*.jpg *jpeg *.gif *.png *.bmp)");

	
	CFileDialogEx fileDlg;
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(this->GetHWND());
	fileDlg.SetFileName(filename);
	fileDlg.SetDefExt(_T("png"));
	if (fileDlg.ShowSaveFileDlg(initDir))
	{
		CDuiString filePath = fileDlg.GetPathName();
		//save
		//todo shizhenyu
	}
}

void CDuiPhotoView::DoRotationAction()
{
	//顺时针旋转90°。
	m_wheelNumber = 0;
	m_rotationCount++;
	int i = m_rotationCount % 4;
	m_imgpanel->SetRotate((ROTATEANGLE)i);
}

void CDuiPhotoView::OnSelectedImageChanged(int index, CDuiString imagePath)
{
	//todo
}
