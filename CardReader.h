#pragma once
#include "Card.h"
#include "../GameEngine/Resources/ResourceManager.h"
#include <rapidxml-1.13/rapidxml.hpp>
#include <list>

namespace CardReader
{	
	static std::vector<Skills> ParseSkills(rapidxml::xml_node<> * node)
	{
		std::vector<Skills> skills;
		for (auto snode = node->first_node(); snode; snode = snode->next_sibling())
		{
			std::string node_name = snode->name();
			if (node_name == "skill")
			{
				std::string type = snode->value();
				if (type == "Hero")
					skills.push_back(Skills::Hero);
				else if (type == "Agile")
					skills.push_back(Skills::Agile);
				else if (type == "Medic")
					skills.push_back(Skills::Medic);
				else if (type == "MoraleBoost")
					skills.push_back(Skills::MoraleBoost);
				else if (type == "Muster")
					skills.push_back(Skills::Muster);
				else if (type == "Scorch")
					skills.push_back(Skills::Scorch);
				else if (type == "ScorchALL")
					skills.push_back(Skills::ScorchALL);
				else if (type == "Spy")
					skills.push_back(Skills::Spy);
				else if (type == "TightBond")
					skills.push_back(Skills::TightBond);
			}
		}
		return skills;
	}

	static SCard ParseCard(rapidxml::xml_node<> * node)
	{
		SCard card;
		for (auto snode = node->first_node(); snode; snode = snode->next_sibling())
		{
			std::string node_name = snode->name();
			
			if (node_name == "name")
				card.name = snode->value();
			else if (node_name == "texture")
				card.texture_path = snode->value();
			else if (node_name == "lineType")
			{
				std::string type = snode->value();
				if (type == "CLOSE_COMBAT")
					card.type = LineTypes::CLOSE_COMBAT;
				else if (type == "ARCHEER")
					card.type = LineTypes::ARCHEER;
				else if (type == "BALIST")
					card.type = LineTypes::BALIST;
				else
					card.type = LineTypes::CLOSE_COMBAT;
			}
			else if (node_name == "strength")
				card.strength = std::stoi(snode->value());
			else if (node_name == "skills")
			{
				card.skills = ParseSkills(snode);
			}
		}
		return card;
	}
	static std::vector<SCard> ReadCardFromString(const std::string& xml_str)
	{
		std::vector<SCard> cards;
		rapidxml::xml_document<> document;
		try
		{
			document.parse<0>(const_cast<char*>(xml_str.c_str()));
		}
		catch (rapidxml::parse_error p)
		{
			p.what();
			return cards;
		}

		auto node = document.first_node();

		if (node != nullptr)
		{
			std::string root_name = node->name();
			if (root_name != "GwintCards")
			{
				document.clear();
				Error("Not gwint cards message but it is excepted.");
				return cards;
			}

			for (auto snode = node->first_node(); snode; snode = snode->next_sibling())
			{
				std::string node_name = snode->name();
				if (node_name == "card")
					cards.push_back(ParseCard(snode));
			}
		}
		else
		{
			Error("Cant read cards file.");
		}
		
		document.clear();
		return cards;
	}
	static std::vector<SCard> ReadCardFromXML(const std::string& file_name)
	{
		auto cards_file = Utils::ReadFile(file_name);		
		return ReadCardFromString(cards_file);
	}
}