#pragma once
#include "Card.h"
#include <vector>

class Player
{
public:
	std::vector<SCard> cards_in_hand;
	std::vector<SCard> cards_on_table;
	std::vector<SCard> cards_in_trash;
	std::vector<SCard> cards_in_deck;
};