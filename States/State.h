#pragma once
#include "../../GameEngine/Objects/RenderAble/Entity.h"
#include "../Card.h"

namespace GameStates
{
	enum State
	{
		RAND_CARDS,
		CHOOSE_CARDS_START,
		CHOOSE_CARD_RECOVERY,
		CHOOSE_CARD_SWAP,
		PLAYER_TURN,
		ENEMY_TURN,
		END_TURN,
		NONE
	};
}

class CState 
{
public:
	virtual ~CState() {}
	virtual void KeyInputDir() = 0;
	virtual void KeyInputConfirm() = 0;
	virtual GameStates::State KeyInputRetrun() = 0;
	virtual std::list<SCard> CardRender() = 0;

	GameStates::State m_GameStateType;
};