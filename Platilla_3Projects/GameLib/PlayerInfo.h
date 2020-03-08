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

	PlayerInfo();
	~PlayerInfo();
};