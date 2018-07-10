#include "stdafx.h"
#include "UUCenterUI.h"
#include <Gdiplus.h>


IMPLEMENT_DUICONTROL(CUUCenterUI)
CUUCenterUI::CUUCenterUI()
{
	m_imageStr = _T("");
	m_angle = ROTATEANGLE0;

}

LPCTSTR CUUCenterUI::GetClass() const
{
	return _T("UUCenterUI");
}

LPVOID CUUCenterUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("UUCenter")) == 0) return static_cast<CUUCenterUI*>(this);
	return CControlUI::GetInterface(pstrName);
}


void CUUCenterUI::PaintStatusImage(HDC hDC)
{		
	if (m_imageStr.IsEmpty())
	{
		return;
	}
	//gdi+ ����
	Gdiplus::Graphics* myGraphics = new Gdiplus::Graphics(hDC);
	
	int rcWidth = m_currentTransferRect.right - m_currentTransferRect.left;
	int rcHeight = m_currentTransferRect.bottom - m_currentTransferRect.top;

	if (m_angle != ROTATEANGLE0)
	{

		POINT imgRotateCenterPos = { rcWidth / 2, rcHeight / 2 }; //��ת������ͼƬ����(�����ͼƬ����)
		//CRect rcShow(imgShowRect);��//ͼƬҪ���Ƶ�λ������.
		//�� �����ͼƬ����ת��������  ת��Ϊ  �������������
		POINT centerPos = { imgRotateCenterPos.x + m_currentTransferRect.left, imgRotateCenterPos.y + m_currentTransferRect.top };
		
		myGraphics->TranslateTransform(centerPos.x, centerPos.y); //Դ���ƶ�����ת����
		myGraphics->RotateTransform(m_fAngle); //��ת
		myGraphics->TranslateTransform(-centerPos.x, -centerPos.y);//��ԭԴ��
	}
	//��ĳ�������ʾͼ��
	myGraphics->DrawImage(m_image, m_currentTransferRect.left, m_currentTransferRect.top, rcWidth, rcHeight);
	//this->Invalidate();
}

int CUUCenterUI::GetK(int w, int h)
{
	int ret;
	RECT rc = GetPos();
	int rcWidth = rc.right - rc.left;
	int rcHeight = rc.bottom - rc.top;
		
	if (w < rcWidth && h < rcHeight)
	{
		ret = 100;
	}
	else if (w < rcWidth && h > rcHeight)
	{		
		ret = 100.0 * rcHeight / h;
	}
	else if (w > rcWidth && h < rcHeight)
	{
		ret = 100.0 * rcWidth / w;
	}
	else if (w > rcWidth && h > rcHeight)
	{
		double kx = 100.0 * rcWidth / w;
		double ky = 100.0 * rcHeight / h;
		if (kx > ky)
		{
			//����ky�����š�
			ret = ky;
		}
		else
		{
			//����kx�����š�			
			ret = kx;
		}
	}
	else
	{
		ret = 100;
	}
	return ret;
}

RECT CUUCenterUI::GetDestDrawRect(int k)
{
	RECT rc = this->GetPos();
	int cx = rc.left + (rc.right - rc.left) / 2;
	int cy = rc.top + (rc.bottom - rc.top) / 2;
	
	RECT ret;
	ret.left = cx - (k * GetCenterImageWidth()) / (2 * 100);
	ret.right = ret.left + k *GetCenterImageWidth() / 100;
	ret.top = cy - (k * GetCenterImageHeight()) / (2 * 100);
	ret.bottom = ret.top + k * GetCenterImageHeight() / 100;
	return ret;
}
