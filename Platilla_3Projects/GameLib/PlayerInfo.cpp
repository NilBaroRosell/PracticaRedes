#include "PlayerInfo.h"
#include <random>


PlayerInfo::PlayerInfo()
{
	numDice = Vector2{ 0,0 };
	hasClue = false;
	direction = 0;
	movesLeft = 0;
}

PlayerInfo::~PlayerInfo()
{
}
