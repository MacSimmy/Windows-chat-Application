// ChattingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyMessanger.h"
#include "ChattingDialog.h"
#include "afxdialogex.h"
#include"NetworkDataProcessor.h"



// CChattingDialog dialog

IMPLEMENT_DYNAMIC(CChattingDialog, CDialog)

CChattingDialog::CChattingDialog(CWnd* pParent, CString caption) : CDialog(CChattingDialog::IDD, pParent)
, m_ChatHistory(_T(""))
, m_MessageToSend(_T(""))
{
	m_pParent = pParent;
	UserOnChat = caption;
}


BOOL CChattingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowText(UserOnChat);
	return TRUE;// return TRUE  unless you set the focus to a control.
}
CChattingDialog::CChattingDialog(CWnd* pParent /*=NULL*/): CDialog(CChattingDialog::IDD, pParent)
	, m_ChatHistory(_T(""))
	, m_MessageToSend(_T(""))
 {
	m_pParent = pParent;
}

CChattingDialog::~CChattingDialog()
{

}




void CChattingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHATTING_HISTORY, m_EditChatHistory);
	DDX_Text(pDX, IDC_EDIT_CHATTING_HISTORY, m_ChatHistory);
	DDV_MaxChars(pDX, m_ChatHistory, 1024);
	DDX_Control(pDX, IDC_EDIT_MESSAGE_TO_SEND, m_EditMessageToSend);
	DDX_Text(pDX, IDC_EDIT_MESSAGE_TO_SEND, m_MessageToSend);
}


BEGIN_MESSAGE_MAP(CChattingDialog, CDialog)
	ON_MESSAGE(WM_MESSAGE_RECIEVED, OnMessageRecieved)
	ON_BN_CLICKED(IDOK, &CChattingDialog::OnBnClickedSend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChattingDialog message handlers


void CChattingDialog::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	/*m_ChatHistory = m_MessageToSend;
	UpdateData(FALSE);*/
	if (m_MessageToSend == "")
	{
		m_EditMessageToSend.SetFocus();
		return;
	}
	else
	{
		NetworkDataProcessor *networkprocessor = new NetworkDataProcessor();

		char *packet_data = new char[MAX_BUFFER_SIZE];
		CString SendToUser = UserRealName(UserOnChat);
		packet_data = networkprocessor->EncodeData(SEND_TO, SendToUser, m_MessageToSend);

		int sendbytes = client->Send(packet_data, strlen(packet_data), 0);
		if (sendbytes <= 0)
		{
			int error = GetLastError();

		}
		else

		{
			UpdateData(TRUE);
			m_ChatHistory = m_ChatHistory + _T("\r\n") + client->ClientName + " : " + m_MessageToSend;
			UpdateData(FALSE);
			m_EditMessageToSend.SetSel(0, -1);
			m_EditMessageToSend.Clear();
			
			
		}
			
	}
	
	
}


CString CChattingDialog:: UserRealName(CString UserName)
{
	char username[25];
	sprintf_s(username, "%s", (LPCTSTR)UserName);
	unsigned int i = 0;
	while (i<strlen(username))
	{
		if (isupper(username[i]))
		{
			(username[i] = tolower(username[i]));

		}
		if (username[i] == ' ')
		{
			username[i] = '.';
		}
		i++;
	}
	CString newname(username, strlen(username));
	return newname;

}




LRESULT CChattingDialog::OnMessageRecieved(WPARAM wParam, LPARAM lParam)
{
	CString user =RecievedFromUser;
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	if (client->ChatWindows.count(user) ==0)
	{
		// user Chat dialog box window is not opened yet.
		// Open it first.
		CChattingDialog * pNewChatDialog = new CChattingDialog(NULL,UserOnChat);
		client->ChatWindows[user] = pNewChatDialog;
		CWnd *pMainWnd = (CWnd*)AfxGetApp()->GetMainWnd();
		pNewChatDialog->Create(IDD_CHATTING_DIALOG, pMainWnd);
		pNewChatDialog->ShowWindow(TRUE);
		UpdateData(TRUE);
		pNewChatDialog->m_ChatHistory =_T("\r\n") + RecievedFromUser + ":" + m_recieved_message;
		UpdateData(FALSE);

	}
	UpdateData(TRUE);
	m_ChatHistory += _T("\r\n")+ RecievedFromUser + ":" + m_recieved_message;
	UpdateData(FALSE);

	return 0;
}

void CChattingDialog::OnClose()
{
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	std::map<CString, CChattingDialog*>::iterator iter = client->ChatWindows.begin();
	while (iter != client->ChatWindows.end())
	{
		if (iter->second == this)
		{
			client->ChatWindows.erase(iter++);
		}

		else
		{
			++iter;
		}
	}
	CDialog::OnClose();

}


void CChattingDialog::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if (m_pParent)
	{
		((CMyMessangerApp*)m_pParent)->m_pmodeless = NULL;
	}
	delete this;
}

