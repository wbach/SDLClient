#pragma once

enum class NetworkState
{
	DOWNLOAD_DECKS,
	RAND_START_CARDS,
	SWAP_CARDS,
	WAIT_FOR_START,
	PLAYER_TURN,
	ENEMY_TURN,
	END_GAME
};