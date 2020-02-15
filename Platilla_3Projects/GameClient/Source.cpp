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

///// CLIENT /////

int main()
{
	/*PlayerInfo playerInfo;
	Graphics g;
	g.DrawDungeon();*/
	std::string nickname;

	std::cout << "Bienvenido a CLUEDO, escoge tu nombre:\n";
	std::cin >> nickname;

	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::milliseconds(15.f));
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		packet << "READY" << nickname;
		socket.send(packet);
	}

	return 0;
}