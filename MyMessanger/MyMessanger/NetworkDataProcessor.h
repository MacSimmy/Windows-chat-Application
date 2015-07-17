#pragma once
#include<cstring>
#include<afxwin.h>
#include<string>
#include<vector>
#include<cstring>
using namespace std;
#define MAX_BUFFER_SIZE 1000
#define SIZE_USERNAME 20

using namespace std;
enum Command {
	SIGN_IN = 1,
	SIGN_OUT,
	SEND_TO,
	RECEIVE_FROM,
	UPDATE_LIST,
	SIGN_IN_SUCCESSFULL,
	SIGN_IN_FAILURE
};

struct NetworkData
{
	Command command;
	char Username[SIZE_USERNAME];
	char data[500];
};

class NetworkDataProcessor{
public:
	static vector<CString>List;
	NetworkData packet;
	void serialize(char*data);
	void UpdateList(string);
	void deserialize(char*data);
	NetworkDataProcessor* DecodeString(string &server_data);
	char* EncodeData(Command cmd, CString username, CString client_data);

};

