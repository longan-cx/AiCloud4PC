#pragma once
#include "stdafx.h"
#include <map>
#include <string>

class CDUIWindow
	: public WindowImplBase
{
protected:
	enum { SKINID_UNKNOWN = -1 };
	HICON m_hIcon = NULL;
	std::map < CDuiString, int > m_SkinIdMap;
	int getIdByName(LPCTSTR sz)
	{
		auto it = m_SkinIdMap.find(sz);
		if (it != m_SkinIdMap.end())
			return it->second;
		return SKINID_UNKNOWN;
	}

	CDuiString getNameById(int id)
	{
		std::map<CDuiString, int>::iterator it = m_SkinIdMap.begin();
		for (; it != m_SkinIdMap.end(); it++)
		{
			if (it->second == id)
				return it->first;
			
		}
		return _T("");
	}

public:
	~CDUIWindow()
	{
		if (m_hIcon)
			::DestroyIcon(m_hIcon);
	}

	virtual LPCTSTR GetWindowClassName(void) const { return _T("BrotsoftGUI"); }
	void SetIcon(STRINGorID bitmap, LPCTSTR type, DWORD mask = 0, bool bGrayScale = false)
	{
		TImageInfo* ptii = CRenderEngine::LoadImage(bitmap, type, mask);
		if (ptii)
		{
			HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), ptii->nX, ptii->nY);
			ICONINFO ii = { 0 };
			ii.fIcon = TRUE;
			ii.hbmColor = ptii->hBitmap;
			ii.hbmMask = hbmMask;
			HICON h = CreateIconIndirect(&ii);
			if (h)
			{
				::SendMessage(m_hWnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)h);
				::SendMessage(m_hWnd, WM_SETICON, (WPARAM)FALSE, (LPARAM)h);
				if (m_hIcon) ::DestroyIcon(m_hIcon);
				m_hIcon = h;
			}
			::DeleteObject(hbmMask);
			CRenderEngine::FreeImage(ptii);
		}
	}
};

#define DECLARE_SKIN_NAME_ID(name, id)	m_SkinIdMap[name] = id;
