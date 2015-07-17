
// MyMessangerView.h : interface of the CMyMessangerView class
//

#pragma once
#include"MyMessangerDoc.h"
#include<map>
using namespace std;
#define WM_LIST_UPDATED (WM_USER+10)
#define WM_CLEAR_VIEW (WM_USER+20)
#define WM_SIGN_IN_FAILURE WM_USER+30

class CMyMessangerView : public CListView
{
protected: // create from serialization only
	CMyMessangerView();
	DECLARE_DYNCREATE(CMyMessangerView)

// Attributes
public:
	
	CMyMessangerDoc* GetDocument() const;
// Operations
public:
	void OnClientListClick(NMHDR* pnmh, LRESULT* pResult);

virtual void OnDraw(CDC* pDC);  // overridden to draw this view
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMyMessangerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:

// Generated message map functions
	CComboBox m_Combo;
	CListCtrl m_List;
protected:
	afx_msg LRESULT OnSignInFailure(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListRecieved(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyMessangerView.cpp
inline CMyMessangerDoc* CMyMessangerView::GetDocument() const
   { return reinterpret_cast<CMyMessangerDoc*>(m_pDocument); }
#endif

