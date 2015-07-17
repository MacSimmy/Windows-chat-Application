
// MyMessanger.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyMessanger.h"
#include "MainFrm.h"
#include"LoginDialog.h"
#include "MyMessangerDoc.h"
#include "MyMessangerView.h"
#include"ClientConnection.h"
#include"NetworkDataProcessor.h"
#include<vector>
#include"ChattingDialog.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CLoginDialog * loginDlg;
char SignOutMessage[30];
CConnectionManager *client = CConnectionManager::GetClientInstance();
// CMyMessangerApp

BEGIN_MESSAGE_MAP(CMyMessangerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMyMessangerApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SIGNIN, &CMyMessangerApp::OnFileSignin)
	ON_COMMAND(ID_FILE_SIGNOUT, &CMyMessangerApp::OnUserSignout)
	ON_COMMAND(ID_FILE_EXIT, &CMyMessangerApp::OnUserExit)

END_MESSAGE_MAP()


// CMyMessangerApp construction

CMyMessangerApp::CMyMessangerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyMessanger.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMyMessangerApp object

CMyMessangerApp theApp;


// CMyMessangerApp initialization

BOOL CMyMessangerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}


	


	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyMessangerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMyMessangerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	return TRUE;
}

int CMyMessangerApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	sprintf_s(SignOutMessage, "%d~s", SIGN_OUT, (LPCTSTR)loginDlg->m_Username);
	client->Send(SignOutMessage, strlen(SignOutMessage));
	client->ShutDown(2);
	client->Close();
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

// CMyMessangerApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMyMessangerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyMessangerApp message handlers


void CMyMessangerApp::OnFileSignin()
{
	client->ConnectToServer();
		loginDlg = new CLoginDialog();
		INT_PTR nResponse = loginDlg->DoModal();
		if (nResponse == IDOK)
		{
			//NOTIFYICONDATA nid;
			//nid.cbSize = sizeof(NOTIFYICONDATA);
			//CFrameWnd *pFrame = (CFrameWnd*)AfxGetApp()->GetMainWnd();
			//// Get the active view attached to the active MDI child window.
			//CView *pView = (CView*)pFrame->GetActiveView();
			//nid.hWnd = pView->m_hWnd; // or whatever HWND you provided when you created it 
			//nid.uID =100;// or whatever ID you provided when you created it
			//nid.uFlags = NIF_INFO;
			//nid.uTimeout = 1000; // In milliseconds. Min value is 10 seconds, max is 30 seconds. If outside of range, it automatically takes closest limit
			//nid.dwInfoFlags = NIIF_INFO; strcpy_s(nid.szInfo, "Sign In successfull");
			//strcpy_s(nid.szInfoTitle, "Sign In successfull Notifiaction.");
			//Shell_NotifyIcon(NIM_MODIFY, &nid);
		}
		else if (nResponse == IDCANCEL)
		{
			AfxMessageBox("Please Sign In first .");
		}
	}



void CMyMessangerApp::OnUserSignout()
{
	if (client->IsConnected())
	{
		// Getting handle to active view.
		CFrameWnd *pFrame = (CFrameWnd*)AfxGetApp()->GetMainWnd();
		CView *pView = (CView*)pFrame->GetActiveView();
		CListView *pListView = (CListView*)pView;

		if (pListView->GetListCtrl().GetItemCount() != 0)
		{
			pListView->GetListCtrl().DeleteAllItems();
			ASSERT(pListView->GetListCtrl().GetItemCount() == 0);
		}
		sprintf_s(SignOutMessage, "%d~s", SIGN_OUT, (LPCTSTR)loginDlg->m_Username);
		client->Send(SignOutMessage, strlen(SignOutMessage));
		client->ShutDown(2);
		client->Close();
	}
	else
		AfxMessageBox("You are not Signed In. Please Sign In first.");
}

void CMyMessangerApp::OnUserExit()
{
	if (client->IsConnected())
	{
	//TODO: Add your command handler code here
	sprintf_s(SignOutMessage, "%d~s", SIGN_OUT, (LPCTSTR)loginDlg->m_Username);
	int sendbytes = client->Send(SignOutMessage, strlen(SignOutMessage));
	client->ShutDown(2);
	client->Close();
	}

	ASSERT(AfxGetMainWnd() != NULL);
	AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	
}



