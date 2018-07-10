#pragma once

//顺时针角度旋转
enum ROTATEANGLE
{
	ROTATEANGLE0 = 0,
	ROTATEANGLE90,
	ROTATEANGLE180,
	ROTATEANGLE270,
};

class  CUUCenterUI : public CControlUI
{
	DECLARE_DUICONTROL(CUUCenterUI)
public:	
	CUUCenterUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	//设置的绝对路径。
	void SetCenterImage(CDuiString img)
	{
		m_imageStr = img;
		
		if (m_image)
		{
			delete m_image;
			m_image = NULL;
		}
		m_image = new Gdiplus::Image(m_imageStr);
		InitCenterUI(m_image->GetWidth(), m_image->GetHeight());
	}

	int GetCenterImageWidth()
	{
		return m_image->GetWidth();
	}
	int GetCenterImageHeight()
	{
		return m_image->GetHeight();
	}

	CDuiString GetCenterImage()
	{
		return m_imageStr;
	}
	void SetRotate(ROTATEANGLE angle)
	{
		m_angle = angle;
		switch (m_angle)
		{
		case ROTATEANGLE0:
			m_fAngle = 0.0f;
			InitCenterUI(GetCenterImageWidth(),GetCenterImageHeight());
			break;
		case ROTATEANGLE90:
			m_fAngle = 90.0f;
			InitCenterUI(GetCenterImageHeight(), GetCenterImageWidth());
			break;
		case ROTATEANGLE180:
			m_fAngle = 180.0f;
			InitCenterUI(GetCenterImageWidth(), GetCenterImageHeight());
			break;
		case ROTATEANGLE270:
			m_fAngle = 270.0f;
			InitCenterUI(GetCenterImageHeight(), GetCenterImageWidth());
			break;
		default:
			m_fAngle = 0.0f;
			break;
		}
	}

	int GetCurrentK()
	{
		return m_k;
	}

	void SetCurrentTransferRect(int val)
	{
		int width = GetCenterImageWidth();
		int height = GetCenterImageHeight();
		RECT rc = GetPos();
		int rcWidth = rc.right - rc.left;
		int rcHeight = rc.bottom - rc.top;

		int cx = rc.left + rcWidth / 2;
		int cy = rc.top + rcHeight / 2;

		int oldWidth = m_currentRect.right - m_currentRect.left;
		int oldHeight = m_currentRect.bottom - m_currentRect.top;

		double kx = 1.0 * (cx - m_currentRect.left) / oldWidth;
		double ky = 1.0 * (cy - m_currentRect.top) / oldHeight;

		RECT ret;
		if (val*width / 100 < rcWidth)
		{
			ret.left = cx - (val*width / 100) / 2;
			ret.right = ret.left + (width * val / 100);
		}
		else
		{
			int lx = cx - (kx * val * width) / (100);
			if (lx + (width * val / 100) < rcWidth)
			{
				ret.right = rc.right;
				ret.left = ret.right - width*val / 100;
			}
			else
			{
				ret.left = lx;
				ret.right = ret.left + (width * val / 100);
			}

		}

		if (val*height / 100 < rcHeight)
		{
			ret.top = cy - (val*height / 100) / 2;
			ret.bottom = ret.top + (height * val / 100);
		}
		else
		{
			int ty = cy - (ky * height * val) / (100);
			if (ty + (val*height / 100) < rcHeight)
			{
				ret.bottom = rc.bottom;
				ret.top = ret.bottom - (height * val / 100);
			}
			else
			{
				ret.top = ty;
				ret.bottom = ret.top + (height * val / 100);
			}
		}
		m_currentTransferRect = ret;
		this->Invalidate();
	}

	void MoveImage(int xpos, int ypos)
	{
		//移动图片
		
		int retWidth = m_currentTransferRect.right - m_currentTransferRect.left;
		int retHeight = m_currentTransferRect.bottom - m_currentTransferRect.top;

		RECT rc = this->GetPos();
		int height = rc.bottom - rc.top;
		int width = rc.right - rc.left;

		if (retWidth > width && retHeight > height)
		{
			//宽高都大于窗口宽高

			int ax = m_currentTransferRect.left - xpos;
			int ay = m_currentTransferRect.top - ypos;
			if (xpos > 0)
			{
				//往左移动。
				if (ax + retWidth < rc.right)
				{
					ax = rc.right - retWidth;
				}
			}
			else if (xpos <0)
			{
				//往右移动
				if (ax > rc.left)
				{
					ax = rc.left;
				}
			}
			else
			{
				//未移动
			}

			if (ypos > 0)
			{
				//往上移
				if (ay + retHeight < rc.bottom)
				{
					ay = rc.bottom - retHeight;
				}
			}
			else if (ypos <0)
			{
				//往下移
				if (ay>rc.top)
				{
					ay = rc.top;
				}
			}
			else
			{
				//未移动
			}

			RECT ret;
			ret.left = ax;
			ret.top = ay;
			ret.right = ax + retWidth;
			ret.bottom = ay + retHeight;

			m_currentTransferRect = ret;
		}
		else if (retWidth > width && retHeight < height)
		{
			//宽大于窗口宽， 高小于窗口高。
			int ax = m_currentTransferRect.left - xpos;
			if (xpos > 0)
			{
				//往左移动。
				if (ax + retWidth < rc.right)
				{
					ax = rc.right - retWidth;
				}
			}
			else if (xpos <0)
			{
				//往右移动
				if (ax > rc.left)
				{
					ax = rc.left;
				}
			}
			else
			{
				//未移动
			}
			RECT ret;
			ret.left = ax;
			ret.top = m_currentTransferRect.top;
			ret.right = ax + retWidth;
			ret.bottom = m_currentTransferRect.bottom;

			m_currentTransferRect = ret;

		}
		else if (retWidth < width && retHeight > height)
		{
			//高都大于窗口高, 宽小于窗口宽。
			int ay = m_currentTransferRect.top - ypos;
			if (ypos > 0)
			{
				//往上移
				if (ay + retHeight < rc.bottom)
				{
					ay = rc.bottom - retHeight;
				}
			}
			else if (ypos <0)
			{
				//往下移
				if (ay>rc.top)
				{
					ay = rc.top;
				}
			}
			else
			{
				//未移动
			}
			RECT ret;
			ret.left = m_currentTransferRect.left;
			ret.top = ay;
			ret.right = m_currentTransferRect.right;
			ret.bottom = ay + retHeight;
			m_currentTransferRect = ret;
		}
		else
		{
			//do noting;
		}
		this->Invalidate();
	}


	void SetCurrentRect(RECT rc)
	{
		m_currentRect = rc;
	}

	RECT GetTransferRect()
	{
		return m_currentTransferRect;
	}

	void ChangedCurrentRect(RECT oldRc)
	{
		//初始的宽高
		int oldW = oldRc.right - oldRc.left;
		int oldH = oldRc.bottom - oldRc.top;
		//现在宽高
		RECT rc = GetPos();
		int rcWidth = rc.right - rc.left;
		int rcHeight = rc.bottom - rc.top;

		double kx = 1.0* rcWidth / oldW;
		double ky = 1.0* rcHeight / oldH;
		m_currentRect.left = kx* m_currentRect.left;
		m_currentRect.right = kx* m_currentRect.right;
		m_currentRect.top = ky* m_currentRect.top;
		m_currentRect.bottom = ky* m_currentRect.bottom;
	}

	virtual void PaintStatusImage(HDC hDC);

private:
	void InitCenterUI(int w, int h)
	{
		m_k = GetK(w,h);
		m_currentRect = GetDestDrawRect(m_k);
		m_currentTransferRect = m_currentRect;
		this->Invalidate();
	}	
private:
	int GetK(int w, int h);
	RECT GetDestDrawRect(int k);

private:
	ROTATEANGLE m_angle = ROTATEANGLE0;
	CDuiString m_imageStr;

	Gdiplus::Image* m_image = NULL;
	float m_fAngle = 0.0f;
	int m_k;
	RECT m_currentRect;
	RECT m_currentTransferRect;
};
