#pragma once
#include <iostream>
#include <string>

using namespace std;

class LobbySystem
{
public:

	string lobbyName;
	int maxPlayers, currentPlayers, reqPlayers, currentSpectators, maxSpectators;
	bool gameState;
	
	// Lobby Constructor
	LobbySystem(string lobbyName, int maxPlayers, int currentPlayers, int reqPlayers, int currentSpectators, int maxSpectators, bool gameState);
	
	LobbySystem();
	~LobbySystem();
};

