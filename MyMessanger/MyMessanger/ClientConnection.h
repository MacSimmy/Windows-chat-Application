#pragma once
#include "afxsock.h"
#include"NetworkDataProcessor.h"
#include"MainFrm.h"
#include"ChattingDialog.h"
#include<map>
using namespace std;

class CConnectionManager :public CAsyncSocket
{
public:
	static CConnectionManager *GetClientInstance();
	BOOL ClientSignIn(CString, CString);
	BOOL ConnectToServer();
public:
	CString  m_sendBuffer; 
    int    m_nBytesSent; 
	int   m_nBytesBufferSize = MAX_BUFFER_SIZE;
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
  	virtual void OnSend(int nErrorCode);
	BOOL IsConnected();
public:
	CString ClientName;
	std::map<CString, class CChattingDialog* >ChatWindows;
private:
	BOOL Connected=FALSE;
	CConnectionManager();
	~CConnectionManager();
	static CConnectionManager * client_instance;
};