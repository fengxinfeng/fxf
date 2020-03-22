#pragma once
 
/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector> 

namespace  nui {

	struct NMedia {
		int  mediatype;  //    0, picture; 1, video
		std::wstring imageurl;  // image location
		std::wstring videourl;  // video location
	}  ;

	class CPhotoViewer : public CSubVBox
	{
	public:
		CPhotoViewer(ui::VBox *p);
		~CPhotoViewer();
		/*
		*  二次构造
		* @param  data, 初始化数据
		*/
		void Construct(std::vector<NMedia> data);
		void SetVisible(bool value);
		void Start();
		void Stop();
		void DisplayNext();
	protected:
		ui::Control*  CreateControl(const std::wstring& pstrClass);
		//void HandleMessage(ui::EventArgs &msg);
	private:
		void ClickToLeft();
		void ClickToRight();
		void ToLeft();
		void ToRight();
		void AutoToLeft(); 
		void SetBorder(int num);
		void  CallJS();
		void PlayVideo();
		void PauseVideo();
		void OnLoadEnd(int httpStatusCode);
		void  OnAfterCreated(CefRefPtr<CefBrowser> browser);
		void LoadVideo(int serno);
		 
	private:
		 
		bool  m_hasinited;
		std::vector<ui::Label *> m_vector;
		std::vector<NMedia> m_data;
		int m_StepLength;
		int m_Serno;
		int m_LeftMargin;
		int m_RightMargin;
		ui::Label *m_PlayControl;
		ui::Label *m_bigimage;
		int  m_MiddleNO;
		HWND m_hwnd;
		bool m_ismoving;
		nim_comp::CefControlBase* m_cefvideo;
		nim_comp::CefControlBase* cef_control_dev_;
		bool m_isloadend;
		int m_taskwaittoload;
		 
	};
}
 

