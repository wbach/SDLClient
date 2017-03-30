#include "SDLClientGetway.h"

SDLClientGetway & SDLClientGetway::Instance()
{
	static SDLClientGetway getway;
	return getway;
}

SDLClientGetway::~SDLClientGetway()
{
	if (!init)
		return;

	SDLNet_FreeSocketSet(socketSet);
	SDLNet_Quit();
}

void SDLClientGetway::Init()
{
	// Initialise SDL_net

	std::string userInput = "";    // A string to hold our user input
	int inputLength = 0;     // The length of our string in characters
	char buffer[BUFFER_SIZE]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.

	memset(buffer, 0, BUFFER_SIZE);


	if (SDLNet_Init() < 0)
	{
		Error("Failed to intialise SDN_net: " + std::string(SDLNet_GetError()));
		exit(-1); // Quit!
	}
	socketSet = SDLNet_AllocSocketSet(1);

	int hostResolved = SDLNet_ResolveHost(&serverIP, serverName.c_str(), PORT);
	const char* host = SDLNet_ResolveIP(&serverIP);
	clientSocket = SDLNet_TCP_Open(&serverIP);

	SDLNet_TCP_AddSocket(socketSet, clientSocket);
	int activeSockets = SDLNet_CheckSockets(socketSet, 5000);
	int gotServerResponse = SDLNet_SocketReady(clientSocket);

	bool is_connected = false;

	if (gotServerResponse != 0)
	{
		//cout << "Got a response from the server... " << endl;
		int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);

		//cout << "Got the following from server: " << buffer << "(" << serverResponseByteCount << " bytes)" << endl;

		std::string buff_string(buffer);

		// We got an okay from the server, so we can join!
		if (buff_string == "OK")
		{
			is_connected = true;
			// So set the flag to say we're not quitting out just yet
			//shutdownClient = false;

			std::cout << "Joining server now..." << std::endl << std::endl;
		}
		else
		{
			//cout << "Server is full... Terminating connection." << endl;
		}
	}
	else
	{
		//cout << "No response from server..." << endl;
	}
	init = true;
}

void SDLClientGetway::SendMessage(const std::string & message)
{
	if (message.size() > 1500)
	{
		std::cout << "Message size is over 1500! " << std::endl;
	}

	auto sended_bytes = SDLNet_TCP_Send(clientSocket, (void *)message.c_str(), message.length() + 1);
	std::cout << "Sended message, size : " << sended_bytes << "/" << message.size() + 1 << " (" << (int)(((float)sended_bytes / (float)(message.size() + 1))*100.f) << "%)" << std::endl;
}

bool SDLClientGetway::GetMessage(std::string & message)
{
	message.clear();

	std::string tmp_msg;
	auto is_msg = GetSingleMessage(tmp_msg);

	if (!is_msg)
		return false;
	std::string start_part_msg = "START_SEND_PARTS";

	auto position = tmp_msg.find("START_SEND_PARTS");
	if (position != std::string::npos)
	{
		tmp_msg = tmp_msg.substr(position + start_part_msg.size());		
		Log("###############################################################################\n");
		Log(tmp_msg);
		message += tmp_msg;

		while (tmp_msg != "END_SEND_PARTS")
		{
			auto is_msg = GetSingleMessage(tmp_msg);
			Log("***********************************************************************\n");
			Log(tmp_msg);
			Log("***********************************************************************\n");
			if (is_msg)
			{
				auto position = tmp_msg.find("END_SEND_PARTS");
				if (position != std::string::npos)
				{
					auto sub = tmp_msg.substr(0, position);
					message += sub;
					break;
				}
				else
				{
					message += tmp_msg;
				}
			}
		}
		Log("###############################################################################\n");
	}
	else
		message = tmp_msg;

	if (message == "END_SEND_PARTS" || message == "START_SEND_PARTS")
		message.clear();

	return true;
}

bool SDLClientGetway::GetSingleMessage(std::string & message)
{
	int socketActive = SDLNet_CheckSockets(socketSet, 0);

	if (socketActive == 0)
		return false;

	int messageFromServer = SDLNet_SocketReady(clientSocket);

	if (messageFromServer == 0)
		return false;

	char buffer[BUFFER_SIZE];
	int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
	//message = std::string(buffer);
	message.clear();
	for (int x = 0; x < serverResponseByteCount; x++)
	{
		message += buffer[x];
	}
	//message += '\0';
	Log("Message recv (buffer): " + message);
	if (static_cast<uint>(serverResponseByteCount) > message.size())
	{
		Error("Message size < serverResponseByteCount " + std::to_string(message.size()) + "/" + std::to_string(serverResponseByteCount));
	}
	return true;
}
