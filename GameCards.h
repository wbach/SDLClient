#pragma once
#include "CardReader.h"
#include "../GameEngine/Resources/ResourceManager.h"
#include <map>

enum class DeckType
{
	HEROS,
	NORTH
};

class GameCards
{
public:
	static GameCards& Instance()
	{
		static GameCards game_cards;
		return game_cards;
	}
	void LoadCards()
	{
		decks[DeckType::HEROS] = CardReader::ReadCardFromXML("../Data/Gwint/Cards/Cards.xml");
		decks[DeckType::NORTH] = CardReader::ReadCardFromXML("../Data/Gwint/Cards/KrolestwaPolnocy.xml");
	}

	bool LoadCards(DeckType deck, const std::string& xml_str)
	{
		decks[deck] = CardReader::ReadCardFromString(xml_str);
		return !decks[deck].empty();
	}

	void LoadTexturesInCards(CResourceManager& manager)
	{
		for (auto& cards : decks)
		{
			for (auto& card : cards.second)
			{
				card.texture = manager.GetTextureLaoder().LoadTexture(card.texture_path);
			}
		}
	}

	const std::map<DeckType, std::vector<SCard>>& GetCards() const
	{
		return decks;
	}
private:
	GameCards() 
	{
	}
	std::map<DeckType, std::vector<SCard>> decks;
};