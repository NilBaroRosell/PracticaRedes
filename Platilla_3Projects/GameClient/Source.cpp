#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include <Types.h>

#define SERVER_IP "192.168.1.131"
#define SERVER_PORT 55556

///// CLIENT /////

int main()
{
	/*PlayerInfo playerInfo;
	Graphics g;
	g.DrawDungeon();*/
	std::string nickname;
	bool connected = false;

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
		packet << static_cast<int32_t>(Comands::START) << nickname;
		socket.send(packet);
		connected = true;
		system("CLS");
		int aux;
		Comands comand;
		std::string data;

		while (connected)
		{

			packet.clear();
			if (socket.receive(packet) == sf::Socket::Done)
			{
				packet >> aux;
				comand = (Comands)aux;
				switch (comand)
				{
					case Comands::WAIT:
						packet >> data;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Waiting for players" << std::endl;
						break;
					case Comands::START:
						packet >> data;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Start game" << std::endl;
						break;
					default:
						break;
				}
				//std::cout << username << " has joined the game" << std::endl;
			}
		}
	}

	return 0;
}