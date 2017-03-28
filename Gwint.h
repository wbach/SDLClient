#pragma once
#include "../GameEngine/Engine/Engine.h"
#include "GameLine.h"
#include "Renderer.h"
#include "GameScene.h"
#include "GameCards.h"

class CGwintGame
{
public:
	void Start();
private:	
	void NetworkStartProcedure();
	void NetworkDownloadDeck();
	void NetworkWaitForStart();
	CGwitRenderer* renderer;
	GwintGameScene* scene;
	SDLGetway getway;
	CEngine engine;
};