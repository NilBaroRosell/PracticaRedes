#pragma once
#include <SFML\Graphics.hpp>
#include "Types.h"

class PlayerInfo
{
public:
	std::string nickname;
	sf::Vector2i position;
	sf::Color color;
	std::vector<card> playerCards;

	PlayerInfo();
	~PlayerInfo();
};