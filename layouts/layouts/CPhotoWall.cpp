/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CPhotoWall.h"    
#include <math.h>
 

namespace nui {
	 
	static const int childmargin = 20;
	static std::wstring schildmargin = L"20";
	CPhotoWall::CPhotoWall(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/banar/photowall.xml", NULL);
		p->Add(this); 
		m_parent = p;
		m_hasinited = false;
	}
	 
	CPhotoWall::~CPhotoWall() {}
 
	void CPhotoWall::SetVisible(bool value) {
		if (m_parent) {
			m_parent->SetVisible(value);
		}
		else {
			__super::SetVisible(value);
		}
	}

	void CPhotoWall::Construct(std::vector<std::wstring> arr) {
		OutputDebugString(L"CPhotoWall::Construct   ------->");
		m_hwnd = this->GetWindow()->GetHWND();
 
		int size = arr.size();

		ui::HBox *container = dynamic_cast<ui::HBox*>(FindSubControl(L"image_container"));
		container->RemoveAll();
		container->SetAttribute(L"childmargin", schildmargin);
	  
		int fixedwidth = container->GetFixedWidth();
		int itemwidth = fixedwidth/5; 
		m_StepLength = itemwidth + childmargin; 
		m_Serno = 0;
		m_LeftMargin = itemwidth * size + childmargin * (size-1) - fixedwidth;


		WCHAR dbuf[256]; 
		wsprintf(dbuf, L"  itemwidth=%d   fixedwidth=%d",   itemwidth, fixedwidth);
		OutputDebugString(dbuf); 

		int i = 1;
		for (auto &s : arr) {
			ui::Label  *photo = new ui::Label;
			WCHAR namebuf[32]; 
			wsprintf(namebuf, L"name:%d", i++);
			photo->SetName(namebuf);
			photo->SetAttribute(L"name", namebuf);
			OutputDebugString(namebuf);
			photo->SetFixedWidth(itemwidth);
 
			photo->SetAttribute(L"height", L"stretch");
 
			//photo->SetAttribute(L"bkimage",s);  
			photo->SetBkImage(s); 
			//photo->SetAttribute(L"visible", L"true");
			photo->SetVisible(true); 
			container->Add(photo); 
			m_vector.push_back(photo); 
		}
		 
		if (!isBindEvent) {
			ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
			ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));

			if (leftbtn) {
				OutputDebugString(L"CPhotoWall::CPhotoWall find left button");
				leftbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CPhotoWall::CPhotoWall   left button");
					ToLeft();
					return true;
				});
			}

			if (rightbtn) {
				OutputDebugString(L"CPhotoWall::CPhotoWall find right button");
				rightbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CTurnImage::CTurnImage   right button");
					ToRight();
					return true;
				});
			}
			isBindEvent = true;
		 }

 
		OutputDebugString(L"CPhotoWall::Construct   -------> ");
	}
 
	void CPhotoWall::ToLeft() { 
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();
		WCHAR dbuf[128];
		wsprintf(dbuf, L"m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(dbuf);
 
		if (abs(r.left) >= m_LeftMargin) {
			OutputDebugString(L" to left value, return");
			return;
		}

		int morevalue = abs(r.left) + m_StepLength - m_LeftMargin;
		if (morevalue > 0) {
			r.left = r.left - m_StepLength + morevalue;
		}
		else {
			r.left = r.left - m_StepLength;
		}

		 
		p->SetMargin(r);
		m_Serno++;
		 
		WCHAR d2buf[128];
		wsprintf(d2buf, L"m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(d2buf);
	}

	void CPhotoWall::ToRight() { 
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();

		WCHAR d2buf[128];
		wsprintf(d2buf, L"m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(d2buf);
		if (r.left >= 0) {
			r.left = 0;
			p->SetMargin(r);
			OutputDebugString(L" to right value, return");
			return;
		}

		int morevalue = r.left + m_StepLength;
		if (morevalue > 0) {
			r.left = 0;
		}
		else {
			r.left = r.left + m_StepLength;
		}
 ;
		p->SetMargin(r);
		m_Serno--;

		WCHAR dbuf[128];
		wsprintf(dbuf, L"m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(dbuf);
	}
		 
 
}
