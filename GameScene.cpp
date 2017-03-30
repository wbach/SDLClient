#include "GameScene.h"
#include "States/ChooseCardState.h"
#include "States/PlayerMoveState.h"
#include "States/EnemyState.h"
#include "../GameEngine/Camera/FirstPersonCamera.h"
#include "../GameEngine/Renderers/GUI/GuiRenderer.h"

GwintGameScene::GwintGameScene(CEngine & engine)
	: engine(engine)
	, renderer(nullptr)
{
	m_State = std::make_unique<CChooseCardState>(player, &engine.m_InputManager, GameLines);
	GameLines =
	{
		{ LineTypes::CLOSE_COMBAT, GameLine(glm::vec3(-.225f, 0.015, .63f)) },
		{ LineTypes::ARCHEER, GameLine(glm::vec3(-.225f, -0.225, .63f)) },
		{ LineTypes::BALIST, GameLine(glm::vec3(-.225f, -0.48, .63f)) }
	};

	EnemyGameLines =
	{
		{ LineTypes::CLOSE_COMBAT, GameLine(glm::vec3(-.225f, 0.26, .63f)) },
		{ LineTypes::ARCHEER, GameLine(glm::vec3(-.225f, 0.51, .63f)) },
		{ LineTypes::BALIST, GameLine(glm::vec3(-.225f, 0.76, .63f)) }
	};

	CGUIRenderer* gui_renderer = new CGUIRenderer();
	guiText = new CGuiText("../Data/GUI/consola.ttf", engine.m_Projection.GetWindowSize());
	gui_renderer->AddElement(guiText);

	SGuiTextElement score;
	score.text = "Test";
	score.colour = glm::vec3(0, 162.f / 255.f, 232.f / 255.f);

	score.position = glm::vec2(-0.55, -0.0);
	guiText->texts["Line_p1"] = score;
	score.position = glm::vec2(-0.55, -0.3);
	guiText->texts["Line_p2"] = score;
	score.position = glm::vec2(-0.55, -0.6);
	guiText->texts["Line_p3"] = score;

	score.position = glm::vec2(-0.75, -0.15);
	guiText->texts["Sum_p1"] = score;

	score.position = glm::vec2(-0.75, 0.4);
	guiText->texts["Sum_e1"] = score;

	score.position = glm::vec2(-0.55, 0.25);
	guiText->texts["Line_e1"] = score;
	score.position = glm::vec2(-0.55, 0.55);
	guiText->texts["Line_e2"] = score;
	score.position = glm::vec2(-0.55, 0.825);
	guiText->texts["Line_e3"] = score;

	engine.m_Renderers.emplace_back(gui_renderer);
	gui_renderer->Init();

}

int GwintGameScene::Initialize()
{
	m_Camera = std::make_unique<CFirstPersonCamera>(&engine.m_InputManager, &engine.m_DisplayManager);
	m_Camera->SetYaw(0);
	m_Camera->SetPitch(0);
	m_Camera->UpdateViewMatrix();

	//for (uint x = 0; x < 10; x++)
	//{
	//	auto i = rand() % player.cards_in_deck.size();
	//	player.cards_in_hand.push_back(player.cards_in_deck[i]);
	//	player.cards_in_hand.push_back(player.cards_in_deck[i]);

	//	player.cards_in_hand.push_back(player.cards_in_deck[i]);
	//	player.cards_in_hand.push_back(player.cards_in_deck[i]);
	//	player.cards_in_deck.erase(player.cards_in_deck.begin() + i);
	//}

	return 0;
}

int GwintGameScene::Update()
{
	if (renderer == nullptr)
		return 0;

	renderer->cards.clear();

	//system("cls");
	//std::cout << "My : \n";
	int player_sum = 0, enemy_sum = 0;
	for (auto& line : GameLines)
		player_sum += line.second.CalculateStrengthLine();
	for (auto& line : EnemyGameLines)
		enemy_sum += line.second.CalculateStrengthLine();


	guiText->texts["Sum_p1"].text = std::to_string(player_sum);
	guiText->texts["Sum_e1"].text = std::to_string(enemy_sum);


	guiText->texts["Line_p1"].text = std::to_string(GameLines[LineTypes::CLOSE_COMBAT].strength);
	guiText->texts["Line_p2"].text = std::to_string(GameLines[LineTypes::ARCHEER].strength);
	guiText->texts["Line_p3"].text = std::to_string(GameLines[LineTypes::BALIST].strength);

	guiText->texts["Line_e1"].text = std::to_string(EnemyGameLines[LineTypes::CLOSE_COMBAT].strength);
	guiText->texts["Line_e2"].text = std::to_string(EnemyGameLines[LineTypes::ARCHEER].strength);
	guiText->texts["Line_e3"].text = std::to_string(EnemyGameLines[LineTypes::BALIST].strength);


	//RenderCardsInHand();
	RenderLine(GameLines);
	RenderLine(EnemyGameLines);

	if (m_State == nullptr)
		return 0;

	m_State->KeyInputDir();

	auto state_return = m_State->KeyInputRetrun();
	if (state_return == GameStates::END_TURN)
	{
		std::cout << "New State! " << std::endl;
		std::cout << "Cards in deck : " << player.cards_in_deck.size() << "\n";
		std::cout << "Enemy Cards in deck : " << enemy.cards_in_deck.size() << "\n";

		auto current_State = m_State->m_GameStateType;
		m_State.reset();

		if (current_State == GameStates::CHOOSE_CARDS_START)
		{
			auto i = rand() % 1;

			if (i == 1)
			{
				m_State = std::make_unique<CPlayerMoveState>(player, &engine.m_InputManager, GameLines);
			}
			else
			{
				m_State = std::make_unique<CEnemyStateMock>(enemy, &engine.m_InputManager, EnemyGameLines);
			}
		}
		else if (current_State == GameStates::PLAYER_TURN)
		{
			m_State = std::make_unique<CEnemyStateMock>(enemy, &engine.m_InputManager, EnemyGameLines);
		}
		else if (current_State == GameStates::ENEMY_TURN)
		{
			m_State = std::make_unique<CPlayerMoveState>(player, &engine.m_InputManager, GameLines);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	for (const auto& card : m_State->CardRender())
	{
		renderer->cards.push_back(card);
	}

	return 0;
}

void GwintGameScene::RenderLine(const std::map<LineTypes, GameLine>& line) const
{
	for (const auto& line : line)
	{
		RenderCards(renderer->cards, line.second.GetCards(), table_card_size);
	}

}

void GwintGameScene::RenderCards(std::list<SCard>& target, const std::vector<SCard>& cards, const glm::vec3 & scale) const
{
	for (const auto& card : cards)
	{
		target.push_back(card);
		target.back().scale = scale;
	}
}
