#include <string>

#include "LobbySystem.h"


LobbySystem::LobbySystem()
{
}

LobbySystem::LobbySystem(string lobby, int maxPlay, int currPlay, int reqPlay, int currSpecs, int maxSpecs, bool state)
{
	lobbyName = lobby;
	maxPlayers = maxPlay;
	currentPlayers = currPlay;
	reqPlayers = reqPlay;
	currentSpectators = currSpecs;
	maxSpectators = maxSpecs;
	gameState = state;
}

LobbySystem::~LobbySystem()
{
}

