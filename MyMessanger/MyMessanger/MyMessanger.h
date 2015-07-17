
// MyMessanger.h : main header file for the MyMessanger application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"    
#include"ChattingDialog.h"
// main symbols


// CMyMessangerApp:
// See MyMessanger.cpp for the implementation of this class
//

class CMyMessangerApp : public CWinApp
{
public:
	CMyMessangerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CChattingDialog* m_pmodeless;
// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnFileSignin();
	afx_msg void OnUserSignout();
	afx_msg void OnUserExit();
	DECLARE_MESSAGE_MAP()
	
};

extern CMyMessangerApp theApp;
