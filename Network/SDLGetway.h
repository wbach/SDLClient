#pragma once
#include "Getway.h"
#include <SDL2/SDL_net.h>
#include "../../GameEngine/Debug_/Log.h"

const unsigned short PORT = 1234; // The port we are connecting to
const unsigned short BUFFER_SIZE = 40000;  // Size of our message buffer (i.e. maximum length of characters in a message)

class SDLGetway : public IGetway
{
public:
	virtual ~SDLGetway() override
	{
		if (!init)
			return;

		SDLNet_FreeSocketSet(socketSet);
		SDLNet_Quit();
	}
	virtual void Init() override
	{
		// Initialise SDL_net

		std::string userInput = "";    // A string to hold our user input
		int inputLength = 0;     // The length of our string in characters
		char buffer[BUFFER_SIZE]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.
		
		memset(buffer, 0, BUFFER_SIZE);


		if (SDLNet_Init() < 0)
		{
			Error("Failed to intialise SDN_net: " +  std::string(SDLNet_GetError()));
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
	virtual void SendMessage(const std::string& message) override
	{
		std::cout << "Sending message, size : " << message.size() << std::endl;
		SDLNet_TCP_Send(clientSocket, (void *)message.c_str(), message.length() + 1);
	}
	virtual bool GetMessage(std::string& message) override
	{
		int socketActive = SDLNet_CheckSockets(socketSet, 0);

		if (socketActive == 0)
			return false;

		int messageFromServer = SDLNet_SocketReady(clientSocket);

		if (messageFromServer == 0)
			return false;

		char buffer[BUFFER_SIZE];
		int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
		message = std::string(buffer);

		return true;
	}
private:
	SDLNet_SocketSet socketSet;
	IPaddress serverIP;       // The IP we will connect to
	TCPsocket clientSocket;   // The socket to use
	std::string   serverName;     // The server name

	bool init = false;
};