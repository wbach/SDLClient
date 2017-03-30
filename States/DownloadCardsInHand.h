#pragma once
#include "State.h"
#include "../Player.h"
#include "../GameLine.h"
#include "../Network/SDLClientGetway.h"
#include "../Messages/Messages.h"


class DownloadCardsInHand : public CState
{
public:
	DownloadCardsInHand(Player& player)
		: player(player)
	{
	}
	virtual void KeyInputDir() override {}
	virtual void KeyInputConfirm() override {}
	virtual GameStates::State KeyInputRetrun() override
	{
		std::string message;
		if (SDLClientGetway::Instance().GetMessage(message))
		{
			if (message == "START_SEND_CARDS_IN_HAND")
			{
				while (1)
				{
					if (!SDLClientGetway::Instance().GetMessage(message))
						continue;

					if (message == "END_SEND_CARDS_IN_HAND")
						break;

					GwentMessages::SwapCardMessage msg;
					if (msg.Serialized(message))
					{
						auto card = player.cards_in_deck[msg.index];

						if (card.name == msg.card_name)
						{
							player.cards_in_hand.push_back(card);
							player.cards_in_deck.erase(player.cards_in_deck.begin() + msg.index);
						}
						else
						{
							Error("Founded card in deck is diffrent than in message.");
						}
					}
				}
			}
		}
	}
	virtual std::list<SCard> CardRender() override 
	{
		return std::list<SCard>();
	}
private:
	Player& player;
};