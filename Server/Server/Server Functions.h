// this file  contains the class server functions.
#pragma once
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H
#include<map>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstring>
#include<iostream>
#include"NetworkData.h"
#include <exception>


using namespace std;
#pragma comment(lib,"Ws2_32.lib")




#define DEFAULT_PORT "6881" 


enum Update_Command
{
	ADD=1,
	REMOVE
};



class ServerFunctions
{
private:
	std::map<string, string >ClientValidation;
	string usercase;
	Data data;

public:
	int iResult;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	ServerFunctions();
	~ServerFunctions();
	bool ValidateClient(string, string);
	void AcceptClients();
	std::map< string, SOCKET>clients;
	void update_server();
	void RecFromClients();
	void UpdateClientList(string,SOCKET, Update_Command);
	static unsigned int client_id;
	char* EncodeData(Command command, string username, string data);
	void UpdateServer();
	void RecievFirstTimefromclient(SOCKET);

	Data* DecodeString(string &client_data);
	
	void SendTo(string send_to_user, SOCKET Sending_client,string send_from_user, char* client_data);
};
#endif