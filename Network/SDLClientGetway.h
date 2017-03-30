#pragma once
#include <SDL2/SDL_net.h>
#include "../../GameEngine/Debug_/Log.h"
#include <iostream>

const unsigned short PORT = 1234; // The port we are connecting to
const unsigned short BUFFER_SIZE = 1024;  // Size of our message buffer (i.e. maximum length of characters in a message)

class SDLClientGetway
{
public:
	static SDLClientGetway& Instance();
	~SDLClientGetway();
	void Init();
	void SendMessage(const std::string& message);
	bool GetMessage(std::string& message);
	bool GetSingleMessage(std::string& message);
private:
	SDLNet_SocketSet socketSet;
	IPaddress serverIP;       // The IP we will connect to
	TCPsocket clientSocket;   // The socket to use
	std::string   serverName;     // The server name

	bool init = false;

	SDLClientGetway() {}
};