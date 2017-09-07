#pragma once
#include <iostream>
#include <string>

using namespace std;

class Player
{
public:

	string name, password;
	int playerID;

	Player(string name, int playerID, string password);
	Player();
	~Player();
};

