#include "Gwint.h"
#include "Network/SDLClientGetway.h"
#include "Messages/Messages.h"

void CGwintGame::Start()
{
	engine.m_Projection = SProjection(wb::vec2i{ 640, 600 }, 0.01f, 100.f, 50);
	renderer = new CGwitRenderer(&engine.m_Projection);
	engine.m_Renderers.emplace_back(renderer);

	engine.Init();
	scene = new GwintGameScene(engine);
	engine.m_Scene = std::unique_ptr<GwintGameScene>(scene);

	renderer->renderer_quad = new CEntity(engine.m_Scene->GetResourceManager(), "../Data/Gwint/RenderQuad.obj", glm::vec3(0, 0, 0));
	renderer->renderer_quad->m_WorldTransform.SetScale(glm::vec3(-1, 1, 1));
	renderer->renderer_quad->m_WorldTransform.SetPosition(glm::vec3(0, 0, -2));
	renderer->bg_texture = engine.m_Scene->GetResourceManager().GetTextureLaoder().LoadTexture("../Data/Gwint/gwint_table.jpg");

	scene->renderer = renderer;

	NetworkStartProcedure();

	//GameCards::Instance().LoadCards();
	GameCards::Instance().LoadTexturesInCards(engine.m_Scene->GetResourceManager());

	for (const auto& decks : GameCards::Instance().GetCards())
	{
		for (const auto& card : decks.second)
		{
			scene->player.cards_in_deck.push_back(card);
		}
	}

	NetworkGetCardsInHand();	

	engine.PreperaScene();
	engine.GameLoop();
}

void CGwintGame::NetworkStartProcedure()
{	
	std::cout << "Connecting to server..." << std::endl;
	SDLClientGetway::Instance().Init();

	NetworkDownloadDeck();
	NetworkWaitForStart();	
}

void CGwintGame::NetworkDownloadDeck()
{
	std::cout << "Waiting for deck..." << std::endl;

	std::string deck;
	while (1)
	{
		auto response = SDLClientGetway::Instance().GetMessage(deck);
		if (response)
		{
			std::cout << deck << std::endl;		

			auto set = GameCards::Instance().LoadCards(DeckType::NORTH, deck);
			if (set)
			{
				SDLClientGetway::Instance().SendMessage("DECK_OK");
				break;
			}
		}
	}
}

void CGwintGame::NetworkWaitForStart()
{
	std::string msg;
	std::cout << "Waiting for start game..." << std::endl;
	while (1)
	{
		auto response = SDLClientGetway::Instance().GetMessage(msg);
		if (response)
		{
			std::cout << msg << std::endl;

			if (msg == "START_GAME")
				break;
		}
	}
}

void CGwintGame::NetworkGetCardsInHand()
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

				Log("Dwonload cards in hand : " + message);

				if (message == "END_SEND_CARDS_IN_HAND")
				{
					SDLClientGetway::Instance().SendMessage("HAND_CARDS_OK");
					break;
				}
				GwentMessages::SwapCardMessage msg;
				if (msg.Serialized(message))
				{
					auto card = scene->player.cards_in_deck[msg.index];

					if (card.name == msg.card_name)
					{
						scene->player.cards_in_hand.push_back(card);
						scene->player.cards_in_deck.erase(scene->player.cards_in_deck.begin() + msg.index);
					}
					else
					{
						Error("Founded card in deck is diffrent than in message. " + card.name + " != " + msg.card_name);
					}
				}
			}
		}
	}
}
