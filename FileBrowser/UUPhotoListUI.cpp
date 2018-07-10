#include "stdafx.h"
#include "UUPhotoListUI.h"
#include "DuiPhotoView.h"

CUUPhotoListItemUI::CUUPhotoListItemUI(CUUPhotoListUI* parent) : m_parent(parent), m_bSelected(false)
{
}

CUUPhotoListItemUI::~CUUPhotoListItemUI()
{

}

void CUUPhotoListItemUI::SetImage(CDuiString strImage)
{
	m_strImage = strImage;
	
	if (m_parent != NULL)
	{
		this->SetFixedWidth(m_parent->GetUserHeight());
		this->SetFixedHeight(m_parent->GetUserHeight());
	}
	SetBkImage(strImage);
}



void CUUPhotoListItemUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
		if (m_pParent != NULL) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
	{
		SetSelected(true);
		return;
	}
	CContainerUI::DoEvent(event);
}

void CUUPhotoListItemUI::SetSelected(bool bSelected)
{
	m_bSelected = bSelected;
	if (m_bSelected)
	{
		SetBorderSize(m_sknBorderSize);
		SetBorderColor(m_skdwBorderColor);
		if (m_parent != NULL)
		{
			m_parent->SelectedImageChanged(this);
		}
	}
	else
	{
		SetBorderSize(0);
	}

}

DuiLib::CDuiString CUUPhotoListItemUI::GetImage()
{
	return m_strImage;
}

CUUPhotoListUI::CUUPhotoListUI() :  m_curItem(NULL), m_height(0)
{

}

CUUPhotoListUI::~CUUPhotoListUI()
{

}

void CUUPhotoListUI::Init(int height, CDuiPhotoView* pObserver)
{
	m_height = height;
	m_pObserver = pObserver;
	SetChildPadding(m_sknChildrenPadding);
}

void CUUPhotoListUI::SelectedImageChanged(CUUPhotoListItemUI* curItem)
{
	if ( curItem != m_curItem)
	{
		if (m_curItem != NULL)
		{
			m_curItem->SetSelected(false);
		}
		m_curItem = curItem;

		if (m_pObserver != NULL)
		{
			m_pObserver->OnSelectedImageChanged(GetSelItemIndex(), GetSelItemImage());
		}
	}	
}

void CUUPhotoListUI::AppendImage(CDuiString image)
{
	if (!image.IsEmpty())
	{
		CUUPhotoListItemUI* pItem = new CUUPhotoListItemUI(this);
		pItem->SetImage(image);
		m_imageUIList.push_back(pItem);
		this->Add(pItem);
	}
}

void CUUPhotoListUI::ResetImages(vector<CDuiString>& images)
{
	
	m_imageUIList.clear();
	for (size_t i = 0; i < images.size(); i++){
		AppendImage(images[i]);
	}
	
}

int CUUPhotoListUI::GetUserHeight()
{
	return m_height;
}

int CUUPhotoListUI::GetSelItemIndex()
{
	for (size_t index = 0; index < m_imageUIList.size(); index++){
		if (m_imageUIList[index] == m_curItem){
			return index;
		}
	}
	return -1;
}

CDuiString CUUPhotoListUI::GetSelItemImage()
{
	if (m_curItem != NULL)
	{
		return m_curItem->GetImage();
	}
	return CDuiString();
}

void CUUPhotoListUI::SetSelectItem(int index)
{
	if (m_imageUIList.size() < index)
	{
		return;
	}
	if (m_curItem)
	{
		m_curItem->SetSelected(false);
	}
	m_curItem = m_imageUIList.at(index);
	if (m_curItem)
	{
		m_curItem->SetSelected(true);
	}
}



