#pragma once
#include "Card.h"

class GameLine
{
public:
	GameLine() {}
	GameLine(glm::vec3 card_pos)
		: card_pos(card_pos)
	{
	}

	void AddCard(SCard card)
	{
		float card_width = 0.09f;

		card.position.z = -1.9f;
		card.position.y = card_pos.y;
		card.position.x = card_pos.x + card_width*cards.size();
		cards.push_back(card);
		//if(cards.)
		float len = card_pos.z - card_pos.x;
		bool update_pos = false;
		while (card_width*cards.size() > len)
		{
			card_width -= 0.001f;
			update_pos = true;
		}
		 
		if (update_pos)
		{
			int x = 0;
			for (auto& c : cards)
			{
				c.position.x = card_pos.x + card_width*x++;
			}
		}		
	}

	int CalculateStrengthLine()
	{
		strength = 0;
		int morale_boost = 0;
		for (const auto& card : cards)
		{
			if (card.FindSkill(Skills::Hero))
			{
				strength += card.strength;
				continue;
			}

			int s = card.strength + morale_boost;

			if (card.FindSkill(Skills::MoraleBoost))
			{
				morale_boost += 1;
			}

			if (water_reduce)
				s = 1;
			if (doubled)
				s *= 2;

			strength += s;
		}
		return strength;
	}
	const std::vector<SCard>& GetCards() const
	{
		return cards;
	}

	bool doubled = false;
	bool water_reduce = false;
	int	 strength = 0;
private:
	//x -limit, y -constant, z- limit
	const glm::vec3 card_pos;
	std::vector<SCard> cards;
};