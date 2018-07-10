#ifndef CUUPHOTOLISTUI_H
#define CUUPHOTOLISTUI_H

#pragma once
#include "UUPhotoListUI.h"
#include "DUIWindow.h"

class CUUPhotoListUI;
class CDuiPhotoView;

class CUUPhotoListItemUI : public CHorizontalLayoutUI
{
public:
	CUUPhotoListItemUI(CUUPhotoListUI* parent = NULL);
	~CUUPhotoListItemUI();

	void SetImage(CDuiString strImage);
	void DoEvent(TEventUI& event);
	void SetSelected(bool bSelected);
	CDuiString GetImage();

protected:
	static const DWORD m_skdwBorderColor = 0xFF666666;
	static const int m_sknBorderSize = 3;
	CUUPhotoListUI* m_parent;
	CDuiString  m_strImage;
	bool m_bSelected;
};


class CUUPhotoListUI : public CHorizontalLayoutUI
{
public:
	CUUPhotoListUI();
	~CUUPhotoListUI();

	void Init(int height, CDuiPhotoView* pObserver = NULL);
	void SetSelectItem( int index);
	void ResetImages(vector<CDuiString>& images);
	void SelectedImageChanged(CUUPhotoListItemUI* curItem);
	int GetUserHeight();
	int GetSelItemIndex();
	CDuiString GetSelItemImage();

protected:
	void AppendImage(CDuiString image);
	vector<CUUPhotoListItemUI*> m_imageUIList;
	CUUPhotoListItemUI* m_curItem = NULL;

	static const int m_sknChildrenPadding = 3;
	int m_height;
	CDuiPhotoView* m_pObserver;
};


#endif


