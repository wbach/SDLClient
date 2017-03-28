#pragma once
#include "../GameEngine/Scene/Scene.hpp"
#include "../GameEngine/Engine/Engine.h"
#include "../GameEngine/Camera/FirstPersonCamera.h"
#include "States/ChooseCardState.h"
#include "States/PlayerMoveState.h"
#include "States/EnemyState.h"
#include "Renderer.h"
#include "Player.h"
#include "Network/SDLGetway.h"
#include <map>

class GwintGameScene : public CScene
{
public:
	GwintGameScene(CEngine& engine);
	virtual int		Initialize() override;
	virtual void	PostInitialize() override {};
	virtual int		Update() override;
	void RenderLine(const std::map<LineTypes, GameLine>& line) const;
	void RenderCards(std::list<SCard>& target, const std::vector<SCard>& cards, const glm::vec3& scale) const;	

	CGuiText* guiText;
	CEngine& engine;
	CGwitRenderer* renderer;
	std::map<LineTypes, GameLine> GameLines;
	std::map<LineTypes, GameLine> EnemyGameLines;
	std::unique_ptr<CState> m_State;

	Player player;
	Player enemy;

//	SDLGetway m_SdlNetGetway;
	
};