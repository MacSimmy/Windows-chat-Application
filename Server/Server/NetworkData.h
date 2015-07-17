#pragma  once

#include<string>

using namespace std;
//char* EncodeData(Command, string, string);
enum Command {
	SIGN_IN = 1,
	SIGN_OUT,
	SEND_TO,
	RECEIVE_FROM,
	UPDATE_LIST,
	SIGN_IN_SUCCESSFULL,
	SIGN_IN_FAILURE
};

struct Data{
	Command command;
	char Username[30];
	char data[500];
	void serialize(char*data)
	{
		memcpy(data, this, sizeof(Data));
	}
	void deserialize(char*data)
	{
		memcpy(this, data, sizeof(Data));
	}
};

//char* EncodeData(Command cmd, string username, string client_data)
//{
//
//	const unsigned int packet_size = sizeof(Data);
//	char *packet_data = new char[packet_size];
//	Data packet;
//	packet.command = cmd;
//	strncpy_s(packet.Username, username.c_str(), sizeof(username));
//	strncpy_s(packet.data, client_data.c_str(), sizeof(client_data));
//	packet.serialize(packet_data);
//
//	return packet_data;
//}

