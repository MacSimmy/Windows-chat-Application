#include "stdafx.h"
#include"ClientConnection.h"
#include"MyMessangerView.h"
#include"ChattingDialog.h"
#include"LoginDialog.h"

#define DEFAULT_PORT 6881
char credential_error[] = "Invalid Credentials. Please login with valid credentials.";
char SignIn_Successfull[] = "Singn In successfull.";

CConnectionManager* CConnectionManager::client_instance = 0;
CConnectionManager::CConnectionManager()
{
	if (AfxSocketInit() == FALSE)
	{
		AfxMessageBox("WinSock initialization is failed.\n"
			"IDP_SOCKETS_INIT_FAILED");
		exit(1);

	}

}

BOOL CConnectionManager::ConnectToServer()
{
	
	
	if (CAsyncSocket::Create() == FALSE)   //nPort, SOCK_STREAM, FD_READ | FD_CLOSE, "127.0.0.1"
	{

		AfxMessageBox("Unable to create the socket for connection");


	}

	// connecting to server,
	if (this->Connect("172.16.73.109", 6881))
		return TRUE;
	else
		return FALSE;
  


}

CConnectionManager::~CConnectionManager()
{
}


CConnectionManager * CConnectionManager::GetClientInstance()
{
	if (!client_instance)
	{
		client_instance = new CConnectionManager();
		return client_instance;
	}

	return client_instance;
}


BOOL CConnectionManager::ClientSignIn(CString username, CString password)
{

	NetworkDataProcessor networkprocessor;
	char *packet_data=new char(MAX_BUFFER_SIZE);
	packet_data = networkprocessor.EncodeData(SIGN_IN, username, password);
	int sendbytes = client_instance->Send(packet_data, strlen(packet_data),0);
	if (sendbytes > 0)
	{
		return TRUE;
	}
	else
	{
		int error = GetLastError();
		return FALSE;
	}
	
	/*
			}
			default:
			{

					   AfxMessageBox("Either Username or Password is wrong.\n Please try with correct credentials.");*/
	//				   return FALSE;bool notification = false;
	//char server_data[sizeof(NetworkData)];
	//while (notification == false)
	//{
	//	int data_length = client_instance->Receive(server_data, sizeof(server_data), 0);

	//	if (data_length <= 0)
	//	{
	//		//no data recieved
	//		continue;
	//	}
	//	notification = true;
	//	string server_data_to_decode(server_data, data_length);
	//	networkprocessor.DecodeString(server_data_to_decode);


	//		// now checking what type of data is received.

	//		switch (networkprocessor.packet.command)
	//		{

	//		case SIGN_IN_SUCCESSFULL:
	//		{
	//									ClientName = username;

	//									return TRUE;
	//									
	//		}
	//		case SIGN_IN_FAILURE:
	//		{
	//								AfxMessageBox("Either Username or Password is wrong.\n Please try with correct credentials.");
	//								return FALSE;
	//								

					  

	//		}



	//		}
	//	}
	// 
	}



void CConnectionManager::OnReceive(int nErrorCode)
{
	char buff[4096];
	int nRead;
	nRead = Receive(buff, 4096,0);
	switch (nRead)
	{
	case 0:
		break;
	case SOCKET_ERROR:

		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Error occured while recieving."));
			break;
		}

	default:
	{
			   //identifying the type of data.
			   string server_data_to_decode(buff, nRead);
			   NetworkDataProcessor *networkprocessor = new NetworkDataProcessor();
			   networkprocessor->DecodeString(server_data_to_decode);
			   CFrameWnd *pFrame = (CFrameWnd*)AfxGetApp()->GetMainWnd();
			   // Get the active view attached to the active MDI child window.
			   CView *pView = (CView*)pFrame->GetActiveView();
			  switch (networkprocessor->packet.command)
			   {
				  	 //  notification = true;
					  // string server_data_to_decode(server_data, data_length);
					    //now checking what type of data is received.
			   case SIGN_IN_SUCCESSFULL:
				
										   ClientName = networkprocessor->packet.Username;
										   AfxMessageBox("Sign In successfull.");
										   break;
					
			  case SIGN_IN_FAILURE:
					
									  PostMessage(pView->m_hWnd, WM_SIGN_IN_FAILURE, 0, 10);
									  break;

			  case UPDATE_LIST:
				  {
								   //Here we update the list by calling the function.
								   string Update_list(networkprocessor->packet.data, strlen(networkprocessor->packet.data));
		                           networkprocessor->UpdateList(Update_list);
								   PostMessage(pView->m_hWnd , WM_LIST_UPDATED, 0, 0);
								   break;
				   }
			  case RECEIVE_FROM:
				
				   { 
									if ((ChatWindows.count(CString(networkprocessor->packet.Username, strlen(networkprocessor->packet.Username))) == 0))
									{
										CChattingDialog *chattingDlg = new CChattingDialog(NULL, CString(networkprocessor->packet.Username, strlen(networkprocessor->packet.Username)));
										ChatWindows[CString(networkprocessor->packet.Username, strlen(networkprocessor->packet.Username))] = chattingDlg;
										CWnd *pMainWnd = (CWnd*)AfxGetApp()->GetMainWnd();
									    chattingDlg->Create(IDD_CHATTING_DIALOG);
									    chattingDlg->ShowWindow(TRUE);
									}
									std::map<CString, CChattingDialog*>::iterator iter;
									for (iter = ChatWindows.begin(); iter != ChatWindows.end(); ++iter)
									{
										if (iter->first == networkprocessor->packet.Username)
										{
											iter->second->RecievedFromUser = CString(networkprocessor->packet.Username, strlen(networkprocessor->packet.Username));
											iter->second->m_recieved_message = CString(networkprocessor->packet.data, strlen(networkprocessor->packet.data));
											HWND hwnd = iter->second->GetSafeHwnd();
											if (hwnd != INVALID_HANDLE_VALUE)
											{
												PostMessage(hwnd, WM_MESSAGE_RECIEVED, 0, 0);
											}											
											
											break;
										}
									}
									break;
						
			   }
			  
			   default:
			   {
						  AfxMessageBox("Error in data packet.");
						  break;


			   }

			   }
			   delete networkprocessor;
			  
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}



void CConnectionManager:: OnSend(int nErrorCode)
{

	/*m_nBytesSent = this->Send((LPCTSTR)m_sendBuffer, strlen((LPCTSTR)m_sendBuffer), 0);
		if (m_nBytesSent <= 0)
		{
			int error = GetLastError();
			return;
		}*/
	//CAsyncSocket::OnSend(nErrorCode);


}



void CConnectionManager::OnClose(int nErrorCode)
{
	CFrameWnd *pFrame = (CFrameWnd*)AfxGetApp()->GetMainWnd();
	//Get the active view attached to the active window View.
	CView *pView = (CView*)pFrame->GetActiveView();
	PostMessage(pView->m_hWnd,WM_CLEAR_VIEW,0,1);
	CAsyncSocket::OnClose(nErrorCode);
}
void CConnectionManager::OnConnect(int nErrorCode)
{

	if (0 != nErrorCode)
	{
		switch (nErrorCode)
		{
		case WSAEADDRINUSE:
			AfxMessageBox(_T("The specified address is already in use.\n"));
			break;
		case WSAEADDRNOTAVAIL:
			AfxMessageBox(_T("The specified address is not available from ")
				_T("the local machine.\n"));
			break;
		case WSAEAFNOSUPPORT:
			AfxMessageBox(_T("Addresses in the specified family cannot be ")
				_T("used with this socket.\n"));
			break;
		case WSAECONNREFUSED:
			AfxMessageBox(_T("The attempt to connect was forcefully rejected.\n"));
			break;
		case WSAEDESTADDRREQ:
			AfxMessageBox(_T("A destination address is required.\n"));
			break;
		case WSAEFAULT:
			AfxMessageBox(_T("The lpSockAddrLen argument is incorrect.\n"));
			break;
		case WSAEINVAL:
			AfxMessageBox(_T("The socket is already bound to an address.\n"));
			break;
		case WSAEISCONN:
			AfxMessageBox(_T("The socket is already connected.\n"));
			break;
		case WSAEMFILE:
			AfxMessageBox(_T("No more file descriptors are available.\n"));
			break;
		case WSAENETUNREACH:
			AfxMessageBox(_T("The network cannot be reached from this host ")
				_T("at this time.\n"));
			break;
		case WSAENOBUFS:
			AfxMessageBox(_T("No buffer space is available. The socket ")
				_T("cannot be connected.\n"));
			break;
		case WSAENOTCONN:
			AfxMessageBox(_T("The socket is not connected.\n"));
			break;
		case WSAENOTSOCK:
			AfxMessageBox(_T("The descriptor is a file, not a socket.\n"));
			break;
		case WSAETIMEDOUT:
			AfxMessageBox(_T("The attempt to connect timed out without ")
				_T("establishing a connection. \n"));
			break;
		default:
			TCHAR szError[256];
			_stprintf_s(szError, _T("OnConnect error: %d"), nErrorCode);
			AfxMessageBox(szError);
			break;
		}
		AfxMessageBox(_T("Please close the application"));
	}
	Connected = TRUE;
	CAsyncSocket::OnConnect( nErrorCode);
}

BOOL CConnectionManager::IsConnected()
{ 
	return Connected;
}