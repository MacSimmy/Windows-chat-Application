#pragma once
#include "afxwin.h"
#include"resource.h"
#include"ClientConnection.h"
#include<cstring>
using namespace std;
#define WM_MESSAGE_RECIEVED (WM_USER+100)

// CChattingDialog dialog

class CChattingDialog : public CDialog
{
	DECLARE_DYNAMIC(CChattingDialog)

public:
	CChattingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChattingDialog();
	CChattingDialog(CWnd* pParent = NULL, CString="ChatWindow");

// Dialog Data

	enum { IDD = IDD_CHATTING_DIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnMessageRecieved(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:	
	virtual BOOL OnInitDialog();
	CChattingDialog& operator=(const CChattingDialog &s)  //assignment operator
    { 
			id = s.id;
			return *this; 
	}

	CChattingDialog(const CChattingDialog &s)  // copy constructor
	{ 
		id = s.id; 
	}

	CString RecievedFromUser;
	CString UserRealName(CString);
	CEdit m_EditChatHistory;
	CString m_ChatHistory;
	CString m_recieved_message;
	CEdit m_EditMessageToSend;
	CString m_MessageToSend;
	afx_msg void OnBnClickedSend();
	
private:
	virtual void OnClose();
	CString UserOnChat;
	CWnd* m_pParent;
	//CString ChatWindowCaption;
	virtual void PostNcDestroy();
	int id;;

};
