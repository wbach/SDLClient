#pragma once
#include "Card.h"
#include <vector>

class Player
{
public:
	std::vector<uint> indexes; //Send randed index
	std::vector<SCard> cards_in_deck;
	std::vector<SCard> cards_in_hand;
	std::vector<SCard> cards_on_table;
	std::vector<SCard> cards_in_trash;
	//Cards
	void TakeCardsStart()
	{
		for (uint x = 0; x < 10; x++)
		{
			TakeSingleCard();
		}
	}
	bool TakeSingleCard()
	{
		if (cards_in_deck.empty())
			return false;

		auto i = rand() % cards_in_deck.size();
		cards_in_hand.push_back(cards_in_deck[i]);
		cards_in_deck.erase(cards_in_deck.begin() + i);
		indexes.push_back(i);
		return true;
	}
private:
	
};