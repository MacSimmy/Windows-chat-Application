// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"Server Functions.h"
void serverLoop();
ServerFunctions * server;

int _tmain(int argc, _TCHAR* argv[])
{
	server = new ServerFunctions();
	serverLoop();
	return 0;
}

void serverLoop()
{
	while (true)
	{
				server->UpdateServer();
	}
}
