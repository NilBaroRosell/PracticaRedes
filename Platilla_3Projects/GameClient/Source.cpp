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
	std::vector<card> myCards;

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
		std::string name;
		CardType type;

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
						int numCards;
						packet >> data >> numCards;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Start game" << std::endl;
						for (int i = 0; i < numCards; i++)
						{
							packet >> name >> aux;
							type = (CardType)aux;
							myCards.push_back({ name, type });
						}

						for (auto card : myCards)
						{
							std::cout << card.name << std::endl;
						}
						packet.clear();
						break;
					case Comands::ROOM_FOUND:
						packet >> data;
						std::cout << "Room " << data << " has been found" << std::endl;
						std::cout << "Do you want to make a deduction? (Y/N)" << std::endl;
						packet.clear();
						std::string answer = "jaja";
						while (answer != "y" || answer != "Y" || answer != "N" || answer != "n")
						{
							answer.clear();
							std::cin >> answer;
						}
						bool wantDeduction;
						if (answer == "y" || answer == "Y") wantDeduction = true;
						else wantDeduction = false;

						std::vector<std::string> character, weapon, room;

						for (auto _card : full_Deck)
						{
							switch (_card.type)
							{
							case CardType::CHARACTER:
								character.push_back(_card.name);
								break;
							case CardType::WEAPON:
								weapon.push_back(_card.name);
								break;
							case CardType::ROOM:
								room.push_back(_card.name);
								break;
							default:
								break;
							}
						}

						std::cout << "SELECT A CHARACTER" << std::endl;
						for (int i = 0; character.size(); i++)
							std::cout << i + 1 << "-" << character[i] << std::endl;

						int cardAnswer = 0;

						while (cardAnswer < 1 || cardAnswer > character.size()) {
							std::cin >> cardAnswer;
						}

						packet << static_cast<int32_t>(Comands::DEDUCTION) << wantDeduction << character[cardAnswer-1];

						cardAnswer = 0;
						system("CLS");
						std::cout << "SELECT A WEAPON" << std::endl;
						for (int i = 0; weapon.size(); i++)
							std::cout << i + 1 << "-" << weapon[i] << std::endl;

						while (cardAnswer < 1 || cardAnswer > weapon.size()) {
							std::cin >> cardAnswer;
						}

						packet << weapon[cardAnswer - 1];

						cardAnswer = 0;
						system("CLS");
						std::cout << "SELECT A ROOM" << std::endl;
						for (int i = 0; room.size(); i++)
							std::cout << i + 1 << "-" << room[i] << std::endl;

						while (cardAnswer < 1 || cardAnswer > room.size()) {
							std::cin >> cardAnswer;
						}

						packet << room[cardAnswer - 1];

						socket.send(packet);
						system("CLS");
						break;
				}
				//std::cout << username << " has joined the game" << std::endl;
			}
		}
	}

	return 0;
}