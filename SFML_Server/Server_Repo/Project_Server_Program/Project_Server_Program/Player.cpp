#include "Player.h"


Player::Player()
{
}

Player::Player(string player, int ID, string pass)
{
	name = player;
	playerID = ID;
	password = pass;
}

Player::~Player()
{
}
