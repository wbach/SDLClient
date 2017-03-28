#pragma once
#include <string>

class IGetway
{
public:
	virtual ~IGetway() {}
	virtual void Init() = 0;
	virtual void SendMessage(const std::string& message) = 0;
	virtual bool GetMessage(std::string& message) = 0;
};