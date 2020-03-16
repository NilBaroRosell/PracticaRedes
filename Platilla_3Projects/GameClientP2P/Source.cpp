#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include <Types.h>
#include <list>

#define SERVER_IP "192.168.1.43"
#define SERVER_PORT 55000

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
	playersInfo.push_back({});
	Graphics g;

	bool connected = false;
	bool draw = false;
	bool firstTurn = true;
	int matchPlayers = 0;

	std::string nickname;

	std::cout << "Bienvenido a CLUEDO, escoge tu nombre:\n";
	std::cin >> nickname;
	playersInfo[0].nickname = nickname;

	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::milliseconds(15.f));
	std::vector<card> full_Deck, myDeck;

	sf::SocketSelector selector;
	std::list<sf::TcpSocket*> playersSockets;

	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		system("CLS");
		//packet << static_cast<int32_t>(Comands::READY) << playersInfo[0].nickname;
		//socket.send(packet);
		int aux;
		Comands comand;
		std::string data;
		std::string name;
		CardType type;
		bool waiting = true;
		int clientPort;

		while (waiting)
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
					system("CLS");
					int numPlayers = 0;
					while (numPlayers < 3 || numPlayers > 6)
					{
						std::cout << "Choose how many player will play this game (from 3 to 6 players)" << std::endl;
						std::cin >> numPlayers;
					}
					packet.clear();
					packet << static_cast<int32_t>(Comands::NUM_PLAYERS) << numPlayers;
					socket.send(packet);
					break;
				}
				case Comands::CONNECT:
				{
					packet >> aux;
					int port;
					for (int i = 0; i < aux; i++)
					{
						packet >> port >> data;
						sf::TcpSocket* sock = new sf::TcpSocket;
						sf::Socket::Status st = sock->connect(data, port, sf::seconds(5.f));
						playersSockets.push_back(sock);
					}
					packet >> matchPlayers;
					std::cout << matchPlayers << std::endl;
					clientPort = socket.getLocalPort();
					socket.disconnect();
					waiting = false;
					break;
				}
				case Comands::LISTEN:
				{
					packet >> matchPlayers;
					std::cout << matchPlayers << std::endl;
					clientPort = socket.getLocalPort();
					socket.disconnect();
					waiting = false;
					break;
				}
				default:
					break;
				}
			}
		}
		
		packet.clear();

		if (playersSockets.size() != matchPlayers - 1)
		{
			sf::TcpListener listener;
			sf::Socket::Status statusL;

			statusL = listener.listen(clientPort);
			selector.add(listener);

			if (statusL != sf::Socket::Done)
			{
				std::cout << "Error al abrir listener\n";
				exit(0);
			}
			listener.setBlocking(false);

			if (statusL == sf::Socket::Done)
			{
				while (playersSockets.size() < matchPlayers - 1)
				{
					if (statusL != sf::Socket::Done)
					{
						std::cout << "Error al recoger conexión nueva\n";
					}

					sf::TcpSocket *newPlayer = new sf::TcpSocket;
					sf::Socket::Status statusNP = listener.accept(*newPlayer);
					if (statusNP == sf::Socket::Done)
					{
						std::cout << "Conectado Nuevo Usuario" << std::endl;
						playersSockets.push_back(newPlayer);
					}
				}
			}
		}

		for (std::list<sf::TcpSocket*>::iterator it = playersSockets.begin(); it != playersSockets.end(); ++it)
		{
			sf::TcpSocket& sock = **it;

			sock.setBlocking(false);
		}

		initializeCards(full_Deck);
		g.InitDungeon();

		/*for (std::list<sf::TcpSocket*>::iterator it = playersSockets.begin(); it != playersSockets.end(); ++it)
		{
			sf::TcpSocket& client = **it;
			std::cout << (int)client.getLocalPort() << std::endl;
		}*/

		packet.clear();
		packet << static_cast<int32_t>(Comands::WAIT) << nickname;

		for (std::list<sf::TcpSocket*>::iterator it = playersSockets.begin(); it != playersSockets.end(); ++it)
		{
			sf::TcpSocket& pSocket = **it;
			pSocket.send(packet);
		}

		packet.clear();

		connected = true;

		while (connected)
		{
			packet.clear();

			for (std::list<sf::TcpSocket*>::iterator it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{

				sf::TcpSocket& pSocket = **it;
				status = pSocket.receive(packet);
				if (status == sf::Socket::NotReady)
				{

				}
				else if (status == sf::Socket::Done)
				{
					int aux2;
					Comands comand2;
					std::string data2;
					packet >> aux2;
					comand2 = (Comands)aux2;
					if (comand2 == Comands::WAIT)
					{
						packet >> data2;
						std::cout << data2 << std::endl;
						break;
					}
				}
				else if (status == sf::Socket::Disconnected)
				{
					std::cout << "Elimino el socket que se ha desconectado\n";
				}
				else
				{
					std::cout << "Error al recibir\n";
				}
			}

			/*if (selector.isReady(listener))
			{
				std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
				// Si el listener esta "ready" ha habido una conexión
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					playersSockets.push_back(client);
					selector.add(*client);
				}
				else
				{
					// Error de conexión, eliminamos al cliente
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
				}
			}
			else
			{*/
				if (1==2/*socket.receive(packet) == sf::Socket::Done*/)
				{
					packet >> aux;
					comand = (Comands)aux;
					switch (comand)
					{
					/*case Comands::CHOOSE_NUM_PLAYERS:
					{
						system("CLS");
						int numPlayers = 0;
						while (numPlayers < 3 || numPlayers > 6)
						{
							std::cout << "Choose how many player will play this game (from 3 to 6 players)" << std::endl;
							std::cin >> numPlayers;
						}
						packet.clear();
						packet << static_cast<int32_t>(Comands::NUM_PLAYERS) << numPlayers;
						socket.send(packet);
						break;
					}
					case Comands::CONNECT:
					{
						packet >> aux;
						int port;
						for (int i = 0; i < aux; i++)
						{
							packet >> port >> data;
							status = socket.connect(data, port, sf::milliseconds(15.f));
							playersSockets.push_back(&socket);
						}
						break;
					}
					case Comands::CONNECT_AND_LISTEN:
					{
						packet >> aux;
						int port;
						for (int i = 0; i < aux; i++)
						{
							packet >> port >> data;
							status = socket.connect(data, port, sf::milliseconds(15.f));
							playersSockets.push_back(&socket);
						}

						std::cout << aux << std::endl;

						statusL = listener.listen(socket.getLocalPort);

						selector.add(listener);
						if (statusL != sf::Socket::Done)
						{
							std::cout << "Error al abrir listener\n";
							exit(0);
						}
						listener.setBlocking(false);
						break;
					}
					case Comands::LISTEN:
					{
						statusL = listener.listen(socket.getLocalPort);
						selector.add(listener);
						if (statusL != sf::Socket::Done)
						{
							std::cout << "Error al abrir listener\n";
							exit(0);
						}
						listener.setBlocking(false);
						break;
					}*/
					case Comands::WAIT:
					{
						/*packet >> data;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Waiting for players" << std::endl;*/
						break;
					}
					case Comands::START:
					{
						/*int numCards;
						packet >> data >> numCards;
						std::cout << data << " has joined" << std::endl;
						std::cout << "Start game" << std::endl;

						packet >> playersInfo[0].position.x >> playersInfo[0].position.y >> playersInfo[0].color.r >> playersInfo[0].color.g >> playersInfo[0].color.b >> playersInfo[0].color.a;

						std::cout << "Your initial position is: " << playersInfo[0].position.x << ", " << playersInfo[0].position.y;

						if (playersInfo[0].color.r == 255)
						{
							if (playersInfo[0].color.g == 255)
							{
								std::cout << " and your color is: yellow" << std::endl;
							}
							else
							{
								if (playersInfo[0].color.b == 255)
								{
									std::cout << " and your color is: magenta" << std::endl;
								}
								else
								{
									std::cout << " and your color is: red" << std::endl;
								}
							}
						}
						else if (playersInfo[0].color.g == 255)
						{
							if (playersInfo[0].color.b == 255)
							{
								std::cout << " and your color is: cyan" << std::endl;
							}
							else
							{
								std::cout << " and your color is: green" << std::endl;
							}
						}
						else if (playersInfo[0].color.b == 255)
						{
							std::cout << " and your color is: blue" << std::endl;
						}

						std::cout << "Your cards are:" << std::endl;
						for (int i = 0; i < numCards; i++)
						{
							packet >> name >> aux;
							type = (CardType)aux;
							playersInfo[0].playerCards.push_back({ name, type });
							std::cout << playersInfo[0].playerCards.back().name << std::endl;
						}
						myDeck = playersInfo[0].playerCards;

						packet.clear();
						packet << static_cast<int32_t>(Comands::READY_START);
						socket.send(packet);
						draw = true;*/
						break;
					}
					case Comands::TURN:
					{
						/*if (firstTurn) firstTurn = false;
						else system("CLS");
						while (!playersInfo.empty())
						{
							playersInfo.pop_back();
						}
						int numPlayers = 0;
						packet >> name >> numPlayers;
						std::cout << "START TURN, " << name << " is playing" << std::endl;
						for (int i = 0; i < numPlayers; i++)
						{
							PlayerInfo newPlayer;
							packet >> newPlayer.position.x >> newPlayer.position.y >> newPlayer.color.r >> newPlayer.color.g >> newPlayer.color.b >> newPlayer.color.a;
							if (i == 0)
							{
								newPlayer.nickname = nickname;
							}
							playersInfo.push_back(newPlayer);
						}

						for (int i = 0; i < playersInfo.size(); i++)
						{
							playersPositions.push_back({});
							playersPositions[i] = playersInfo[i].position;
							playersColors.push_back({});
							playersColors[i] = playersInfo[i].color;
						}
						g.DrawDungeon(playersPositions, playersColors, draw, playersInfo[0].direction);
						while (!playersColors.empty() && !playersPositions.empty())
						{
							playersColors.pop_back();
							playersPositions.pop_back();
						}

						if (name == playersInfo[0].nickname)
						{
							packet >> playersInfo[0].numDice.x >> playersInfo[0].numDice.y >> playersInfo[0].hasClue;
							std::cout << "Your dice score is: " << playersInfo[0].numDice.x + playersInfo[0].numDice.y << "(" << playersInfo[0].numDice.x << ", " << playersInfo[0].numDice.y << ")" << std::endl;
							playersInfo[0].movesLeft = playersInfo[0].numDice.x + playersInfo[0].numDice.y;

							if (playersInfo[0].hasClue)
							{
								CardType type;
								packet >> aux;
								type = (CardType)aux;
								packet.clear();

								int numCard = 0;
								std::string nameCard;
								switch (type)
								{
								case CardType::CHARACTER:
									while (numCard < 1 || numCard > 6)
									{
										std::cout << "And you got a clue card. Choose a character (from 1 to 6):" << std::endl;
										std::cout << "1. Prado" << std::endl;
										std::cout << "2. Rubio" << std::endl;
										std::cout << "3. Orquidea" << std::endl;
										std::cout << "4. Celeste" << std::endl;
										std::cout << "5. Mora" << std::endl;
										std::cout << "6. Amapola" << std::endl;
										std::cin >> numCard;
									}

									switch (numCard)
									{
									case 1:
										nameCard = "Prado";
										break;
									case 2:
										nameCard = "Rubio";
										break;
									case 3:
										nameCard = "Orquidea";
										break;
									case 4:
										nameCard = "Celeste";
										break;
									case 5:
										nameCard = "Mora";
										break;
									case 6:
										nameCard = "Amapola";
										break;
									default:
										break;
									}

									break;
								case CardType::WEAPON:
									while (numCard < 1 || numCard > 6)
									{
										std::cout << "And you got a clue card. Choose a weapon (from 1 to 6):" << std::endl;
										std::cout << "1. Candelabro" << std::endl;
										std::cout << "2. Punal" << std::endl;
										std::cout << "3. Tuberia de plomo" << std::endl;
										std::cout << "4. Pistola" << std::endl;
										std::cout << "5. Cuerda" << std::endl;
										std::cout << "6. Herramienta" << std::endl;
										std::cin >> numCard;
									}

									switch (numCard)
									{
									case 1:
										nameCard = "Candelabro";
										break;
									case 2:
										nameCard = "Punal";
										break;
									case 3:
										nameCard = "Tuberia de plomo";
										break;
									case 4:
										nameCard = "Pistola";
										break;
									case 5:
										nameCard = "Cuerda";
										break;
									case 6:
										nameCard = "Herramienta";
										break;
									default:
										break;
									}
									break;
								case CardType::ROOM:
									while (numCard < 1 || numCard > 9)
									{
										std::cout << "And you got a clue card. Choose a room (from 1 to 9):" << std::endl;
										std::cout << "1. Sala de baile" << std::endl;
										std::cout << "2. Sala de billar" << std::endl;
										std::cout << "3. Invernadero" << std::endl;
										std::cout << "4. Comedor" << std::endl;
										std::cout << "5. Vestibulo" << std::endl;
										std::cout << "6. Cocina" << std::endl;
										std::cout << "7. Biblioteca" << std::endl;
										std::cout << "8. Salon" << std::endl;
										std::cout << "9. Estudio" << std::endl;
										std::cin >> numCard;
									}

									switch (numCard)
									{
									case 1:
										nameCard = "Sala de baile";
										break;
									case 2:
										nameCard = "Sala de billar";
										break;
									case 3:
										nameCard = "Invernadero";
										break;
									case 4:
										nameCard = "Comedor";
										break;
									case 5:
										nameCard = "Vestibulo";
										break;
									case 6:
										nameCard = "Cocina";
										break;
									case 7:
										nameCard = "Biblioteca";
										break;
									case 8:
										nameCard = "Salon";
										break;
									case 9:
										nameCard = "Estudio";
										break;
									default:
										break;
									}

									break;
								default:
									break;
								}

								packet << static_cast<int32_t>(Comands::CLUE_REQUEST) << nameCard;
								socket.send(packet);
							}
							else
							{
								packet.clear();
								if (name == playersInfo[0].nickname)
								{
									system("CLS");
									int direction = 0;
									std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
									while (direction < 1 || direction > 4)
									{
										std::cout << "Choose a direction (1=left, 2=right, 3=up, 4=down)" << std::endl;
										std::cin >> direction;
									}
									playersInfo[0].direction = direction;
									packet << static_cast<int32_t>(Comands::GO_TO) << playersInfo[0].direction;
									socket.send(packet);
								}
							}
						}

						packet.clear();*/
						break;
					}
					case Comands::CLUE_RESPONSE:
					{
						/*system("CLS");
						std::string playerWithCard;

						packet >> name >> data >> playerWithCard;
						packet.clear();

						std::cout << name << " has requested for " << data << " card and " << playerWithCard << " has it" << std::endl;

						if (playersInfo[0].nickname == name)
						{
							int direction = 0;
							std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
							while (direction < 1 || direction > 4)
							{
								std::cout << "Choose a direction (1=left, 2=right, 3=up, 4=down)" << std::endl;
								std::cin >> direction;
							}
							playersInfo[0].direction = direction;
							packet << static_cast<int32_t>(Comands::GO_TO) << playersInfo[0].direction;
							socket.send(packet);
						}
						*/
						break;
					}
					case Comands::MOVE:
					{
						/*system("CLS");
						MoveOptions options;
						int movesLeft = 0;
						packet >> movesLeft >> aux;
						playersInfo[0].movesLeft = movesLeft;
						options = (MoveOptions)aux;
						switch (options)
						{
						case MoveOptions::MOVE:
						{
							packet >> name;
							for (int i = 0; i < playersInfo.size(); i++)
							{
								if (playersInfo[i].nickname == name)
								{
									int aux1 = 0;
									int aux2 = 0;
									packet >> aux1 >> aux2;
									playersInfo[i].position.x = aux1;
									playersInfo[i].position.y = aux2;
								}
							}

							for (int i = 0; i < playersInfo.size(); i++)
							{
								playersPositions.push_back({});
								playersPositions[i] = playersInfo[i].position;
								playersColors.push_back({});
								playersColors[i] = playersInfo[i].color;
							}
							g.DrawDungeon(playersPositions, playersColors, draw, playersInfo[0].direction);
							while (!playersColors.empty() && !playersPositions.empty())
							{
								playersColors.pop_back();
								playersPositions.pop_back();
							}

							packet.clear();
							if (playersInfo[0].nickname == name)
							{
								std::cout << "You moved successfully" << std::endl;
								std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
								int direction = 0;
								while (direction < 1 || direction > 4)
								{
									std::cout << "Choose a direction (1=left, 2=right, 3=up, 4=down)" << std::endl;
									std::cin >> direction;
								}
								playersInfo[0].direction = direction;
								packet << static_cast<int32_t>(Comands::GO_TO) << playersInfo[0].direction;
								socket.send(packet);
							}
							break;
						}
						case MoveOptions::LAST:
						{
							packet >> name;
							std::cout << "You moved successfully" << std::endl;
							std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
							for (int i = 0; i < playersInfo.size(); i++)
							{
								if (playersInfo[i].nickname == name)
								{
									int aux1 = 0;
									int aux2 = 0;
									packet >> aux1 >> aux2;
									playersInfo[i].position.x = aux1;
									playersInfo[i].position.y = aux2;
								}
							}

							for (int i = 0; i < playersInfo.size(); i++)
							{
								playersPositions.push_back({});
								playersPositions[i] = playersInfo[i].position;
								playersColors.push_back({});
								playersColors[i] = playersInfo[i].color;
							}
							g.DrawDungeon(playersPositions, playersColors, draw, playersInfo[0].direction);
							while (!playersColors.empty() && !playersPositions.empty())
							{
								playersColors.pop_back();
								playersPositions.pop_back();
							}

							bool isRoom;

							packet >> isRoom;
							packet.clear();

							if (isRoom)
							{
								packet << static_cast<int32_t>(Comands::GO_TO);
							}
							else
							{
								packet << static_cast<int32_t>(Comands::TURN_FINISHED);
							}

							socket.send(packet);
							break;
						}
						case MoveOptions::BLOCKED:
						{
							/*packet.clear();
							std::cout << "You are blocked" << std::endl;
							std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
							int direction = 0;
							while (direction < 1 || direction > 4)
							{
								std::cout << "Choose a direction (1=left, 2=right, 3=up, 4=down)" << std::endl;
								std::cin >> direction;
							}
							playersInfo[0].direction = direction;
							packet << static_cast<int32_t>(Comands::GO_TO) << playersInfo[0].direction;
							socket.send(packet);
							break;
						}
						case MoveOptions::STOPPED:
						{
							packet.clear();
							std::cout << playersInfo[0].movesLeft << " moves left" << std::endl;
							packet << static_cast<int32_t>(Comands::TURN_FINISHED);
							break;
						}
						default:
							break;
						}*/
						break;
					}
					case Comands::ROOM_FOUND:
					{
						/*packet >> data;
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
							packet << static_cast<int32_t>(Comands::DEDUCTION);

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
						else packet << static_cast<int32_t>(Comands::TURN_FINISHED);
						socket.send(packet);*/
						break;
					}
					case Comands::DENY:
					{
						/*system("CLS");
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
						for (auto card : myDeck)
						{
							for (auto denyCard : cardsToDeny) {
								if (card.name == denyCard) {
									cardsPlayerCanDeny.push_back(card);
									std::cout << "-" << denyCard << std::endl;
								}
							}
						}
						if (!cardsPlayerCanDeny.empty()) {
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
						}
						else
						{
							system("CLS");
							std::cout << "You don't have any cards to deny" << std::endl;
							packet << static_cast<int32_t>(Comands::DEDUCTION) << true << cardsToDeny[0] << cardsToDeny[1] << cardsToDeny[2];
						}
						socket.send(packet);*/
						break;
					}
					case Comands::SHOW_DENYRESPONE:
					{
						/*card denyCard;
						int aux;
						std::string nickname;
						packet >> nickname;
						if (nickname == "NONE")
						{
							packet.clear();
							std::string answer;
							do
							{
								std::cout << "Nobody could deny any card. Do you want to make a formal accusation? (Y/N)" << std::endl;
								answer.clear();
								std::cin >> answer;
								system("CLS");
							} while (answer != "y" && answer != "Y" && answer != "N" && answer != "n");
							if (answer == "y" || answer == "Y") {
								packet << static_cast<int32_t>(Comands::ACCUSATION);
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
							else packet << static_cast<int32_t>(Comands::TURN_FINISHED);
						}
						else {
							packet >> denyCard.name >> aux;
							denyCard.type = (CardType)aux;
							std::cout << "Player " << nickname << " denied card-> " << denyCard.name << std::endl;
							packet.clear();
							packet << static_cast<int32_t>(Comands::TURN_FINISHED);
						}
						socket.send(packet);*/
						break;
					}
					case Comands::ACCUSATION_RESULT:
					{
						/*std::string result, action;
						packet >> result >> action;
						packet.clear();
						std::cout << result << std::endl;
						if (action == "ACCUSE")
						{
							packet << static_cast<int32_t>(Comands::ACCUSATION);
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

							socket.send(packet);
						}*/
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
				g.DrawDungeon(playersPositions, playersColors, draw, playersInfo[0].direction);
				while (!playersColors.empty() && !playersPositions.empty())
				{
					playersColors.pop_back();
					playersPositions.pop_back();
				}
			//}
		}
		g.ClearDungeon();
	}

	return 0;
}