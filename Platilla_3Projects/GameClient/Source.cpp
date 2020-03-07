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
void initializeCards(std::vector<card> &_fullDeck)
{
	//Characters
	_fullDeck.push_back({ "Prado", CardType::CHARACTER });
	_fullDeck.push_back({ "Rubio", CardType::CHARACTER });
	_fullDeck.push_back({ "Orquidea", CardType::CHARACTER });
	_fullDeck.push_back({ "Celeste", CardType::CHARACTER });
	_fullDeck.push_back({ "Mora", CardType::CHARACTER });
	_fullDeck.push_back({ "Amapola", CardType::CHARACTER });

	//Weapons
	_fullDeck.push_back({ "Candelabro", CardType::WEAPON });
	_fullDeck.push_back({ "Punal", CardType::WEAPON });
	_fullDeck.push_back({ "Tuberia de plomo", CardType::WEAPON });
	_fullDeck.push_back({ "Pistola", CardType::WEAPON });
	_fullDeck.push_back({ "Cuerda", CardType::WEAPON });
	_fullDeck.push_back({ "Herramienta", CardType::WEAPON });

	//Rooms
	_fullDeck.push_back({ "Sala de baile", CardType::ROOM });
	_fullDeck.push_back({ "Sala de billar", CardType::ROOM });
	_fullDeck.push_back({ "Invernadero", CardType::ROOM });
	_fullDeck.push_back({ "Comedor", CardType::ROOM });
	_fullDeck.push_back({ "Vestibulo", CardType::ROOM });
	_fullDeck.push_back({ "Cocina", CardType::ROOM });
	_fullDeck.push_back({ "Biblioteca", CardType::ROOM });
	_fullDeck.push_back({ "Salon", CardType::ROOM });
	_fullDeck.push_back({ "Estudio", CardType::ROOM });
}


int main()
{
	std::vector<PlayerInfo> playersInfo;
	std::vector<sf::Vector2f> playersPositions;
	std::vector<sf::Color> playersColors;
	/*
	PlayerInfo playerInfo;*/
	playersInfo.push_back({});
	Graphics g;
	
	bool connected = false;

	std::cout << "Bienvenido a CLUEDO, escoge tu nombre:\n";
	std::cin >> playersInfo[0].nickname;

	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::milliseconds(15.f));
	std::vector<card> full_Deck;
	

	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		system("CLS");
		packet << static_cast<int32_t>(Comands::READY) << playersInfo[0].nickname;
		socket.send(packet);
		connected = true;
		int aux;
		Comands comand;
		std::string data;
		std::string name;
		CardType type;

		initializeCards(full_Deck);
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
					{
						int numPlayers;
						std::cout << "Choose how many player will play this game (from 3 to 6 players)" << std::endl;
						std::cin >> numPlayers;
						system("CLS");
						if (numPlayers < 3)
						{
							std::cout << "3 players will play this match" << std::endl;
							numPlayers = 3;
						}
						else if (numPlayers > 6)
						{
							std::cout << "6 players will play this match" << std::endl;
							numPlayers = 6;
						}
						else std::cout << numPlayers << " players will play this match" << std::endl;
						packet.clear();
						packet << static_cast<int32_t>(Comands::NUM_PLAYERS) << numPlayers;
						break;
					}
					case Comands::WAIT:
					{
						packet >> data;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Waiting for players" << std::endl;
						break;
					}
					case Comands::START:
					{
						int numCards;
						packet >> data >> numCards;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Start game" << std::endl;

						packet >> playersInfo[0].position.x >> playersInfo[0].position.y >> playersInfo[0].color.r >> playersInfo[0].color.g >> playersInfo[0].color.b >> playersInfo[0].color.a;

						std::cout << "At position-> " << playersInfo[0].position.x << "-" << playersInfo[0].position.y << std::endl;

						for (int i = 0; i < numCards; i++)
						{
							packet >> name >> aux;
							type = (CardType)aux;
							playersInfo[0].playerCards.push_back({ name, type });
							std::cout << playersInfo[0].playerCards.back().name << std::endl;
						}

						packet.clear();
						packet << static_cast<int32_t>(Comands::READY_START);
						socket.send(packet);
						break;
					}
					case Comands::ROOM_FOUND:
					{
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
					}
					case Comands::DENY:
					{
						system("CLS");
						std::string deductionPlayer;
						packet >> deductionPlayer;
						std::cout << deductionPlayer + " asks for: " << std::endl;
						std::string cardsToDeny[3];
						for (int i = 0; i < 3; i++)
						{
							packet >> cardsToDeny[i];
							std::cout << "-" << cardsToDeny[i] << std::endl;
						}
						packet.clear();
						std::vector<card> cardsPlayerCanDeny;
						std::cout << "And you have: " << std::endl;
						for (auto card : playersInfo[0].playerCards)
						{
							for (auto denyCard : cardsToDeny) {
								if (card.name == denyCard) {
									cardsPlayerCanDeny.push_back(card);
									std::cout << "-" << denyCard << std::endl;
								}
							}
						}
						int answer = 0;
						std::cout << "SELECT A DENY ANSWER" << std::endl;
						for (int i = 0; i < cardsPlayerCanDeny.size(); i++)
							std::cout << i + 1 << "-" << cardsPlayerCanDeny[i].name << std::endl;
						card denyCard;
						while (answer < 1 || answer > cardsPlayerCanDeny.size()) {
							std::cin >> answer;
						}
						denyCard = cardsPlayerCanDeny[answer - 1];
						packet << static_cast<int32_t>(Comands::DENY_RESPONSE) << denyCard.name << static_cast<int32_t>(denyCard.type);
						socket.send(packet);
						break;
					}
					case Comands::SHOW_DENYRESPONE:
					{
						card denyCard;
						int aux;
						std::string nickname;
						packet >> nickname >> denyCard.name >> aux;
						denyCard.type = (CardType)aux;
						std::cout << "Player " << nickname << " denied card-> " << denyCard.name << std::endl;
						break;
					}
				}
				//std::cout << username << " has joined the game" << std::endl;
			}
			for (int i = 0; i < playersInfo.size(); i++)
			{
				playersPositions.push_back({});
				playersPositions[i] = playersInfo[i].position;
				playersColors.push_back({});
				playersColors[i] = playersInfo[i].color;
			}
			g.DrawDungeon(playersPositions, playersColors);
			for (int i = 0; i < playersInfo.size(); i++)
			{
				playersColors.pop_back();
				playersPositions.pop_back();
			}
		}
		g.ClearDungeon();
	}

	return 0;
}