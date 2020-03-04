#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include <Types.h>

#define SERVER_IP "192.168.1.45"
#define SERVER_PORT 55556

///// CLIENT /////

int main()
{
	PlayerInfo playerInfo;
	Graphics g;
	
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
		system("CLS");
		packet << static_cast<int32_t>(Comands::READY) << nickname;
		socket.send(packet);
		connected = true;
		int aux;
		Comands comand;
		std::string data;
		std::string name;
		CardType type;

		initializeCards();
		g.InitDungeon();

		while (connected)
		{
			packet.clear();
			if (socket.receive(packet) == sf::Socket::Done)
			{
				packet >> aux;
				comand = (Comands)aux;
				switch (comand)
				{
					case Comands::CHOOSE_NUM_PLAYERS:
						int numPlayers;
						std::cout << "Choose how many player will play this game (from 3 to 6 players)" << std::endl;
						std::cin >> numPlayers;
						system("CLS");
						if (numPlayers < 3)
						{
							std::cout << "3 players will play this match" << std::endl;
							numPlayers = 3;
						}
						else if(numPlayers > 6)
						{
							std::cout << "6 players will play this match" << std::endl;
							numPlayers = 6;
						}
						else std::cout << numPlayers << " players will play this match" << std::endl;
						packet.clear();
						packet << static_cast<int32_t>(Comands::NUM_PLAYERS) << numPlayers;
						break;
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

						packet >> playerInfo.position.x >> playerInfo.position.y >> playerInfo.color.r >> playerInfo.color.g >> playerInfo.color.b >> playerInfo.color.a;

						for (auto card : myCards)
						{
							std::cout << card.name << std::endl;
						}
						packet.clear();
						break;
					case Comands::ROOM_FOUND:
						packet >> data;
						std::cout << "Room " << data << " has been found" << std::endl;
						packet.clear();
						do
						{
							std::cout << "Do you want to make a deduction? (Y/N)" << std::endl;
							data.clear();
							std::cin >> data;
							system("CLS");
						} while (data != "y" && data != "Y" && data != "N" && data != "n");
						if (data == "y" || data == "Y")
						{
							packet << static_cast<int32_t>(Comands::DEDUCTION) << true;

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
							for (int i = 0; i < character.size(); i++)
								std::cout << i + 1 << "-" << character[i] << std::endl;

							int cardAnswer = 0;

							while (cardAnswer < 1 || cardAnswer > character.size()) {
								std::cin >> cardAnswer;
							}

							packet << character[cardAnswer - 1];

							cardAnswer = 0;
							system("CLS");
							std::cout << "SELECT A WEAPON" << std::endl;
							for (int i = 0; i < weapon.size(); i++)
								std::cout << i + 1 << "-" << weapon[i] << std::endl;

							while (cardAnswer < 1 || cardAnswer > weapon.size()) {
								std::cin >> cardAnswer;
							}

							packet << weapon[cardAnswer - 1];

							cardAnswer = 0;
							system("CLS");
							std::cout << "SELECT A ROOM" << std::endl;
							for (int i = 0; i < room.size(); i++)
								std::cout << i + 1 << "-" << room[i] << std::endl;

							while (cardAnswer < 1 || cardAnswer > room.size()) {
								std::cin >> cardAnswer;
							}

							packet << room[cardAnswer - 1];

							
							system("CLS");
						}
						else packet << static_cast<int32_t>(Comands::DEDUCTION) << false;
						socket.send(packet);
						break;
					case Comands::DENY:
						system("CLS");
						std::cout << "Asks for: " << std::endl;
						std::string cardsToDeny[3];
						for (int i = 0; i < 3; i++)
						{
							packet >> cardsToDeny[i];
							std::cout << "-" << cardsToDeny[i] << std::endl;
						}
						std::vector<card> cardsPlayerCanDeny;
						std::cout << "And you have: " << std::endl;
						for (auto card : myCards)
						{
							for (auto denyCard : cardsToDeny) {
								if (card.name == denyCard) {
									cardsPlayerCanDeny.push_back(card);
									std::cout << "-" << denyCard << std::endl;
								}
							}
						}
						break;
				}
				//std::cout << username << " has joined the game" << std::endl;
			}
			g.DrawDungeon();
		}
		g.ClearDungeon();
	}

	return 0;
}