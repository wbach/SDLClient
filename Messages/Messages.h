#pragma once
#include <rapidxml-1.13/rapidxml.hpp>
#include <string>

namespace GwentMessages
{
	struct SwapCardMessage
	{
		enum class MessageType
		{
			REQUEST,
			RESPONSE,
			NOT_SET
		};
		std::string card_name;
		uint index;
		MessageType type = MessageType::NOT_SET;

		std::string ToString()
		{
			std::string type_str = "SWAP_CARD_REQ";
			if(type == MessageType::REQUEST)
				type_str = "SWAP_CARD_RESP";

			return
				"<?xml version = \"1.0\" encoding=\"UTF - 8\"?>"
				"<" + type_str + ">"
				"<index>" + std::to_string(index) + "</index>"
				"<name>" + card_name + "</name>"
				"</" + type_str + ">";
		}

		bool Serialized(const std::string& message)
		{
			if (message.empty())
				return false;

			rapidxml::xml_document<> document;
			try
			{
				document.parse<0>(const_cast<char*>(message.c_str()));

				auto node = document.first_node();

				if (node != nullptr)
				{
					std::string name = node->name();
					int swaped_index = -1;
					
					if (name == "SWAP_CARD_RESP")
						type = MessageType::RESPONSE;
					if (name == "SWAP_CARD_REQ")
						type = MessageType::REQUEST;

					if (type != MessageType::NOT_SET)
					{
						for (auto snode = node->first_node(); snode; snode = snode->next_sibling())
						{
							name = snode->name();
							if (name == "index")
							{
								swaped_index = std::stoi(snode->value());
							}
							else if (name == "name")
							{
								card_name = snode->value();
							}
						}
					}
					document.clear();

					if (swaped_index >= 0)
					{
						index = static_cast<uint>(swaped_index);
					}
					
					if (swaped_index < 0 && card_name.empty())
					{						
						return false;
					}
				}
			}
			catch (rapidxml::parse_error p)
			{
				p.what();
				return false;
			}
			return true;
		}
	};
}
