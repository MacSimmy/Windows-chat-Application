#pragma once
#include "afxwin.h"
#include"NetworkDataProcessor.h"

// CLoginDialog dialog

class CLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDialog();

// Dialog Data
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Username;
	CString m_Password;
	// controll to username edit box
	CEdit m_EditUsername;
	CEdit m_EditPassword;
	afx_msg void OnBnClickedSignIn();
	afx_msg void OnBnClickedCancel();
};
