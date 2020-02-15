#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"

#define SERVER_IP "192.168.56.1"
#define SERVER_PORT 55556

int main()
{
	/*PlayerInfo playerInfo;
	Graphics g;
	g.DrawDungeon();*/

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::milliseconds(15.f));
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
	}

	return 0;
}