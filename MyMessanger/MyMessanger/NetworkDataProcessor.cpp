#include "stdafx.h"
#include "NetworkDataProcessor.h"
#include"ClientConnection.h"

vector<CString> NetworkDataProcessor::List = {};
void NetworkDataProcessor::serialize(char*data)
{
	memcpy(data, &packet, sizeof(NetworkData));
}


void NetworkDataProcessor::deserialize(char*data)
{
	memcpy(&packet, data, sizeof(NetworkData));
}


char* NetworkDataProcessor::EncodeData(Command cmd, CString username, CString client_data)
{
		char packet_data[MAX_BUFFER_SIZE];
		sprintf_s(packet_data, "%d~%s~%s", cmd, (LPCTSTR)username, (LPCTSTR)client_data);
		return packet_data;	
}

NetworkDataProcessor* NetworkDataProcessor::DecodeString(string &server_data)
{

	NetworkData *Packet = new NetworkData();
		int i, j, k;
		std::string::iterator iter;

		for (i = 0, iter = server_data.begin(); server_data.at(i) != '~'; i++, iter++)
		{
			int cmd = atoi(&server_data.at(i));
			Packet->command = (Command)cmd;

		}
		iter++;
		//checking the option for command UPDATELIST.
		if (Packet->command == UPDATE_LIST)
		{
			for (k = 0; iter != server_data.end(); iter++, k++)
			{
				Packet->data[k] = *iter;

			}
			Packet->data[k] = '\0';
			this->packet = *Packet;
			return this;
		}
		else
		{
			for (j = 0; *iter != '~'; j++, iter++)
			{
				Packet->Username[j] = *iter;
			}
			Packet->Username[j] = '\0';
			iter++;
			for (k = 0; iter != server_data.end(); iter++, k++)
			{
				Packet->data[k] = *iter;

			}
			Packet->data[k] = '\0';
			this->packet = *Packet;
			return this;
		}
	
}

void NetworkDataProcessor::UpdateList(string ActiveClientList)
{
	std::string::iterator iter;
	NetworkDataProcessor::List.clear();
	string delimiter = "_";
	string name;
	size_t pos = 0;
	//char Username[20];
	CConnectionManager* client = CConnectionManager::GetClientInstance();
	while ((pos = ActiveClientList.find(delimiter)) != string::npos)
	{
		name = ActiveClientList.substr(0, pos);
		CString cstring(name.c_str());
		if (cstring!=client->ClientName)
		{
			//Here is my code for changing the usernames into a human readable code.
			for (iter = name.begin(); iter != name.end(); iter++)
			{
				if (iter == name.begin())
				{
					*iter = toupper(*iter);

				}
				if (*iter == '.')
				{
					*iter = ' ';
					iter++;
					*iter = toupper(*iter);
				}
			}
			CString cstr(name.c_str());
			NetworkDataProcessor::List.push_back(cstr);
		}
		
		ActiveClientList.erase(0, pos + delimiter.length());
	}
	
}