#pragma once
#include <SFML\Graphics.hpp>

class PlayerInfo
{
public:
	std::string nickname;
	sf::Vector2i position;

	PlayerInfo();
	~PlayerInfo();
};