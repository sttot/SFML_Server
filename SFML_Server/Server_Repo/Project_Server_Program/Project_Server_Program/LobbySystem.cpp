#include <string>

#include "LobbySystem.h"


LobbySystem::LobbySystem()
{
}

LobbySystem::LobbySystem(string lobby, int maxPlay, int currPlay, int reqPlay, int currSpecs, int maxSpecs, bool game)
{
	lobbyName = lobby;
	maxPlayers = maxPlay;
	currentPlayers = currPlay;
	reqPlayers = reqPlay;
	currentSpectators = currSpecs;
	maxSpectators = maxSpecs;
	gameState = game;
}

LobbySystem::~LobbySystem()
{
}
