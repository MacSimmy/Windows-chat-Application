#include "stdafx.h"
#include"Server Functions.h"
#include<string>
#include <sstream>
using namespace std;
#define MAX_BUFFER_SIZE 10000
char credential_error[] = "Invalid Credentials. Please login with valid credentials.";
char SignIn_Successfull[] = "Singn In successfull.";

ServerFunctions::ServerFunctions()
{

	// variable of WSADATA
	WSADATA wsaData;
	// initializing all the socket at server side;
	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;
	// creating a addrinfo struct variable for getting the address info;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	// now  atatring the ws api.
	cout << " Starting The WinSock.................";
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// check for error
	if (iResult != 0)
	{
		cerr << "Unable to startup the WS API from  server side with error";
		cout << iResult;
		exit(1);
	}
	cout << "Initialized.";

	// now setting the address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;// tcp connection
	hints.ai_flags = AI_PASSIVE;
	// server does not need address if it is local machine.
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		cerr << "Unable to  get the address side with error";
		cout << iResult;
		WSACleanup();
		exit(1);

	}
	// creating a socket for listening;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	//check for error
	if (ListenSocket == INVALID_SOCKET)
	{
		cerr << "Unable to create a socket for listening purpose.";
		cout << iResult;
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}
	//set the mode of socket to be nonblocking
	u_long iMode = 1;
	iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);


	if (iResult == SOCKET_ERROR)
	{

		cerr << "Unable to control socket by octlsocket() function.";
		cout << iResult;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
	// set up tcp listening socket

	cout << endl << endl;
	cout << "Binding the Server socket to the port......................";

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{

		cerr << "bind failed with error.";
		cout << WSAGetLastError();
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// else if socket binded successfully, free the address  information..  doesnt needed now
	cout << " BIND Successfull.";

	// use the same socket for listening to  other socket.
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{

		cerr << "Listen failed  due top error.";
		cout << WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
	
	freeaddrinfo(result);



	// clients login validation data

	string user_name[] = { "ankit.jain", "mahendra.chhimwal", "j.singh","anshul.garg","ruchin.agrwal" };
	string user_password[] = { "ankit@123", "mahendra@123", "j.singh@123","anshul@123","ruchin@123" };

	for (int i = 0; i <= 4; i++)
	{
		ClientValidation.insert(pair<string, string>(user_name[i], user_password[i]));
	}
}


void ServerFunctions ::UpdateServer()
{

	AcceptClients();

	RecFromClients();
}

// validating client credentials;
bool ServerFunctions::ValidateClient(string username, string password)
{
	bool matched = false;

	std::map<string, string>::iterator iter;
	for (iter = ClientValidation.begin(); iter != ClientValidation.end(); iter++)
	{
		if (username == iter->first)
		{
			if (password == iter->second)
			{
				matched = true;
				break;
			}

		}
		else
			continue;
	}

	// check if credentials are matched or not

	if (matched == true)
		return true;
	else
		return false;


}



// Acceptiong the clients

void ServerFunctions::AcceptClients()
{
	SOCKET Clientsocket;
	Clientsocket = accept(ListenSocket, NULL, NULL);
	if (Clientsocket != INVALID_SOCKET)
	{
		char value = 1;
		setsockopt(Clientsocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
		 RecievFirstTimefromclient(Clientsocket);
	}

	

}



// Recieving from client
void ServerFunctions::RecFromClients()
{
	// go through all clients
	if (clients.size() > 0)
	{

		char client_data[MAX_BUFFER_SIZE];
		int data_length;
		std::map<string, SOCKET>::iterator iter = clients.begin();
		while (iter != clients.end())
		{
				data_length = recv(iter->second, client_data, MAX_BUFFER_SIZE, 0);
				if (data_length <= 0)
				{
					//no data recieved
					++iter;
					continue;
				}

				else
				{
					string client_data_str(client_data, data_length);
					Data *packet = DecodeString(client_data_str);
					// now checking what type of data is received.
					switch (packet->command)
					{
					case SIGN_IN:
					{
									cout << endl << " New Client request is coming...";
									cout << endl << "validating client....";
									if (ValidateClient(packet->Username, packet->data) != false)
									{
										cout << "Client connected to server. Sending the update list to all clients.";

										send(iter->second, SignIn_Successfull, sizeof(SignIn_Successfull), 0);
										UpdateClientList(iter->first, iter->second, ADD);
										iter++;
										break;

									}
									else
									{
										send(iter->second, credential_error, sizeof(credential_error), 0);
										++iter;
									}
									break;

					}


					case SIGN_OUT:
					{
									 cout << endl << "Sign Out request is coming from a client. Removing the client from the active clients list.........";

									 string username = iter->first;
									 SOCKET rClient = iter->second;
									 ++ iter;				
									 UpdateClientList(username, rClient, REMOVE);
									 cout << "Removed";
									 break;
					}
					case SEND_TO:
					{
									string send_to_user(packet->Username);
									SendTo(send_to_user, iter->second, iter->first, packet->data);
									++iter;
									break;
					}
					default:
						cout << endl << "invalid data format , Please send a proper packet of data.";
						iter++;
						break;
				  }
		    }
		 		
		}

     }
	
  }







void ServerFunctions::UpdateClientList(string username, SOCKET new_client, Update_Command cmd)
{
	int sendbytes;
	// this function will add or remove the active client list. It also sends the new list bto all clients.
	if (cmd == ADD)
	{
		clients.insert(pair<string, SOCKET>(username, new_client));
		if (clients.size()>1)
		{

			std::map<string, SOCKET>::iterator iter;
			//now updating all other active clients.
			std::stringstream s;
			for (iter = clients.begin(); iter != clients.end(); iter++)
			{	
				s << iter->first<<"_";
			}
			// now encoding the data
			string data = s.str();		
			char encodeddata[100];
			sprintf_s(encodeddata,100, "%d~%s", UPDATE_LIST,data.c_str() );
			for (iter = clients.begin(); iter != clients.end(); ++iter)
			{		
				sendbytes=	send(iter->second, encodeddata, strlen(encodeddata), 0);
			}

		}

	}

	else
	{
		if (!clients.empty())
		{
			if (clients.count(username)!=0)
			clients.erase(username);

			if (!clients.empty())
			{

				std::map<string, SOCKET>::iterator iter;
				// now updating all other active clients.
				std::stringstream s;
				for (iter = clients.begin(); iter != clients.end(); ++iter)
				{

					s << iter->first<<"_";

				}
				string data = s.str();
				// now encoding the data
				char encodeddata [100];
				sprintf_s(encodeddata,"%d~%s", UPDATE_LIST, data.c_str());

				for (iter = clients.begin(); iter != clients.end(); iter++)
				{
					sendbytes=send(iter->second, encodeddata, strlen(encodeddata), 0);
				}
			
			}
		}
	}
}





char* ServerFunctions::EncodeData(Command cmd, string username, string client_data)
{
	char packet_data[100];
	sprintf_s(packet_data, "%d~%s~%s", cmd, username.c_str(), client_data.c_str());
	return packet_data;

}




void ServerFunctions::SendTo(string send_to_user, SOCKET Sending_client, string send_from_user, char* client_data)
{
	SOCKET Recieving_client;
	std::map<string, SOCKET>::iterator iter;
	for (iter = clients.begin(); iter != clients.end(); iter++)
	{
		if (iter->first == send_to_user)
		{
			Recieving_client = iter->second;
			break;
		}

	}


	char data_to_send[100];
	sprintf_s(data_to_send, "%d~%s~%s", RECEIVE_FROM, send_from_user.c_str(), client_data);
	int iResult=send(Recieving_client, data_to_send, sizeof(data_to_send), 0);
	if (iResult <= 0)
	{
		cout << endl << "Sending message from" << send_from_user << "to " << send_to_user << " is failed" << endl;

	}
	else
		cout << endl<<"sending is successfull from " << send_from_user << " to " << send_to_user;

}



void ServerFunctions::RecievFirstTimefromclient(SOCKET newSocket)
{
	
	char client_data[MAX_BUFFER_SIZE];
	int data_length;
	bool initailal_data_recieved = false;
	while (initailal_data_recieved == false)
	{

		data_length = recv(newSocket, client_data, MAX_BUFFER_SIZE, 0);
		if (data_length <= 0)
		{
			continue;
		}
		else
		{
			// now checking what type of data is received.
			string client_data_str(client_data, data_length);
			Data *packet = DecodeString(client_data_str);
			switch (packet->command)
			{

			case SIGN_IN:

			{

							cout << endl << " New Client request is coming...";
							cout << endl << "validating client....";
							if (ValidateClient(packet->Username, packet->data) != false)
							{
								cout <<endl<< "Client connected to server."<<endl<<" Sending the update list to all clients.";
								char successfull_message[100];
								sprintf_s(successfull_message, "%d~%s~%s", SIGN_IN_SUCCESSFULL, packet->Username, "");
								send(newSocket, successfull_message, strlen(successfull_message), 0);
								UpdateClientList(packet->Username, newSocket, ADD);
								
							}
							else
							{
								cout << endl << "Client cannot connected to server due to wrong credentials";
								char Failure_message[10];
								sprintf_s(Failure_message, "%d~%s~%s", SIGN_IN_FAILURE, "", "");
								send(newSocket, Failure_message, strlen(Failure_message), 0);
								cout << endl << endl << credential_error;
								closesocket(newSocket);
							}
							break;

			}

			default:
				cout << "Invalid data packet received. Unable to decode the packet.";
				break;
			}

			initailal_data_recieved = true;
		}

		}
	}





Data* ServerFunctions::DecodeString(string & client_data)
{
	
	Data *packet = new Data();
	int i, j, k;
	std::string::iterator iter;

	for (i = 0,iter=client_data.begin(); client_data.at(i) != '~'; i++,iter++)
	{
		int cmd = atoi(&client_data.at(i));
		packet->command =(Command) cmd;

	}
	iter++;
	if (packet->command == SIGN_OUT)
	{
		return packet;
	}
	else
	{

		for (j = 0; *iter != '~'; j++, iter++)
		{
			packet->Username[j] = *iter;
		}
		packet->Username[j] = '\0';
		iter++;
		for (k = 0; iter != client_data.end(); iter++, k++)
		{
			packet->data[k] = *iter;

		}
		packet->data[k] = '\0';
		return packet;
	}
}