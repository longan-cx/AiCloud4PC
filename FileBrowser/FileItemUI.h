#pragma once
#include "LabelEx.h"

class CFileItemUI : public DuiLib::COptionUI
{
		DECLARE_DUICONTROL(CFileItemUI)
public:
	CFileItemUI();
	~CFileItemUI(void);

public:
	void DoEvent(DuiLib::TEventUI& event);
	virtual void PaintStatusImage(HDC hDC);

public:
	void SetIcon(HICON hIcon);

private:
	HICON m_hIcon;
};

