
// MyMessangerView.cpp : implementation of the CMyMessangerView class
//
#include "stdafx.h"

#include"NetworkDataProcessor.h"
#include"ClientConnection.h"


// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include"ChattingDialog.h"
#include "MyMessanger.h"
#endif

#include "MyMessangerDoc.h"
#include "MyMessangerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyMessangerView

IMPLEMENT_DYNCREATE(CMyMessangerView, CListView)

BEGIN_MESSAGE_MAP(CMyMessangerView, CListView)
	ON_MESSAGE(WM_SIGN_IN_FAILURE,OnSignInFailure)
	ON_MESSAGE(WM_CLEAR_VIEW,OnClearView)
	ON_MESSAGE(WM_LIST_UPDATED, OnListRecieved)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClientListClick)
END_MESSAGE_MAP()

// CMyMessangerView construction/destruction

CMyMessangerView::CMyMessangerView()
{
	// TODO: add construction code here

}

CMyMessangerView::~CMyMessangerView()
{
}

BOOL CMyMessangerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	

	return CListView::PreCreateWindow(cs);
}

void CMyMessangerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	
	//OnListRecieved(10,10);

}
void CMyMessangerView::OnDraw(CDC* /*pDC*/)
{
	CMyMessangerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

LRESULT CMyMessangerView:: OnSignInFailure(WPARAM wParam, LPARAM lParam)
{
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	client->ShutDown(2);
	client->Close();
	AfxMessageBox("Either Username or Password is wrong.\n Please try with correct credentials.");
	return 0;
}



LRESULT CMyMessangerView::OnClearView(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	if (GetListCtrl().GetItemCount() != 0)
	{
		GetListCtrl().DeleteAllItems();
		ASSERT(GetListCtrl().GetItemCount() == 0);
	}
	AfxMessageBox("Either server is down or it closed the connection.\n"
		"kindly contact Mahendra Chhimwal.");
	return 0;
}

LRESULT CMyMessangerView::OnListRecieved(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	GetListCtrl().ModifyStyle(0, LVS_ALIGNTOP | LVS_AUTOARRANGE | LVS_ICON | LVS_LIST | LVS_REPORT | LVS_SHAREIMAGELISTS| WS_BORDER | WS_CAPTION | WS_CHILD | WS_DLGFRAME | WS_OVERLAPPED | WS_VISIBLE);
	GetListCtrl().SetExtendedStyle(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHT | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE);
	GetListCtrl().DeleteAllItems();
	ASSERT(GetListCtrl().GetItemCount() == 0);
	std::vector<CString>::iterator iter;
	int i = 0;
	for (iter = NetworkDataProcessor::List.begin(); iter != NetworkDataProcessor::List.end(); ++iter)
	{
		GetListCtrl().InsertItem(i, (LPCTSTR)*iter, LVCFMT_LEFT);
		i++;
	}

	return 0;
}


// CMyMessangerView diagnostics

#ifdef _DEBUG
void CMyMessangerView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyMessangerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMyMessangerDoc* CMyMessangerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMessangerDoc)));
	return (CMyMessangerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyMessangerView message handlers
void CMyMessangerView::OnClientListClick(NMHDR* pnmh, LRESULT* pResult)
{
	DWORD dwPos = ::GetMessagePos();
	CPoint point((int)LOWORD(dwPos), (int)HIWORD(dwPos)); 
	GetListCtrl().ScreenToClient(&point);  
	int nIndex; 
	if ((nIndex = GetListCtrl().HitTest(point)) != -1)
	{
	CString string = GetListCtrl().GetItemText(nIndex, 0);
	CChattingDialog *chattingDlg = new CChattingDialog( NULL,string);
	CString user = chattingDlg->UserRealName(string);
	CConnectionManager *client = CConnectionManager::GetClientInstance();
	// Checking if the suer chat window is already opened or not? If not then open it now .
	if ((client->ChatWindows.count(user) == 0))
	 {
		client->ChatWindows[user] = chattingDlg;
		CWnd *pMainWnd = (CWnd*)AfxGetApp()->GetMainWnd();
		chattingDlg->Create(IDD_CHATTING_DIALOG, pMainWnd);
		chattingDlg->ShowWindow(TRUE);
	 }
	UpdateData(FALSE);
	} 
	*pResult = 0;
}