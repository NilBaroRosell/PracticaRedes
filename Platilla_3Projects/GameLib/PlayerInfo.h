#pragma once
#include <SFML\Graphics.hpp>
#include "Types.h"

class PlayerInfo
{
public:
	std::string nickname;
	sf::Vector2f position;
	sf::Color color;
	std::vector<card> playerCards;
	Vector2 numDice;
	bool hasClue;
	int direction;
	int movesLeft;

	PlayerInfo();
	~PlayerInfo();
};

bool operator== (const PlayerInfo &p1, const PlayerInfo &p2)
{
	return (p1.color == p2.color);
}
bool operator!= (const PlayerInfo &p1, const PlayerInfo &p2)
{
	return !(p1.color == p2.color);
}