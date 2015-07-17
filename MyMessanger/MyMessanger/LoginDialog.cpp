// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyMessanger.h"
#include "LoginDialog.h"
#include "afxdialogex.h"
#include"ClientConnection.h"
#include"NetworkDataProcessor.h"


// CLoginDialog dialog

IMPLEMENT_DYNAMIC(CLoginDialog, CDialogEx)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDialog::IDD, pParent)
	, m_Username(_T(""))
	, m_Password(_T(""))
{

}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_Username);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_USERNAME, m_EditUsername);
	DDX_Control(pDX, IDC_PASSWORD, m_EditPassword);
	DDV_MaxChars(pDX, m_Username, 20);
	DDV_MaxChars(pDX, m_Password, 20);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedSignIn)
	ON_BN_CLICKED(IDCANCEL, &CLoginDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDialog message handlers


void CLoginDialog::OnBnClickedSignIn()
{

// TODO: Add your control notification handler code here
	UpdateData();
	BOOL valid_login = FALSE;
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	//client->ConnectToServer();
	if (m_Username == "")
	{
		AfxMessageBox("You must provide a username");
		m_EditUsername.SetFocus();
		return;
	}
	if (m_Password == "")
	{
		AfxMessageBox("Blank passwords are not allowed\n"
			"Please provide a password");
		m_EditPassword.SetFocus();
		return;
	}

	client->ClientSignIn(m_Username, m_Password);
	UpdateData(FALSE);
	OnOK();
	
}
	
void CLoginDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
