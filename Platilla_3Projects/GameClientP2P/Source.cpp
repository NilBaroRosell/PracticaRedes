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

#define SERVER_IP "localhost"
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

void ShuffleCards(std::vector<card> &_shuffledCards, std::vector<card> _deck)
{
	while (!_deck.empty())
	{
		//agafar random
		int ran = rand() % _deck.size();

		_shuffledCards.push_back(_deck[ran]);

		for (auto it = _deck.begin(); it != _deck.end(); it++)
		{
			if (it->name == _deck[ran].name)
			{
				_deck.erase(it);
				break;
			}
		}
	}
}

void TakeFinalCards(std::vector<card> &_finalCards, std::vector<card> &_shuffledCards)
{
	bool hasCharacter = false;
	bool hasWeapon = false;
	bool hasRoom = false;
	std::vector<card>::iterator aux = _shuffledCards.end();

	int i = 0;
	for (auto cards : _shuffledCards)
	{
		if (!hasCharacter && cards.type == CardType::CHARACTER)
		{
			_finalCards.push_back(cards);
			_shuffledCards.erase(_shuffledCards.begin() + i);
			aux = _shuffledCards.end();
			hasCharacter = true;
		}
		else if (!hasWeapon && cards.type == CardType::WEAPON)
		{
			_finalCards.push_back(cards);
			_shuffledCards.erase(_shuffledCards.begin() + i);
			aux = _shuffledCards.end();
			hasWeapon = true;
		}
		else if (!hasRoom && cards.type == CardType::ROOM)
		{
			_finalCards.push_back(cards);
			_shuffledCards.erase(_shuffledCards.begin() + i);
			aux = _shuffledCards.end();
			hasRoom = true;
		}

		if (hasCharacter && hasWeapon && hasRoom)
		{
			break;
		}
		i++;
	}
}

void AssignCards(std::vector<card> &shuffledCards, std::vector<card> &_myCards, int _numPlayers)
{
	int size = shuffledCards.size();
	for(int i = 0; i < size/_numPlayers; i++)
	{
		_myCards.push_back(shuffledCards.front());
		shuffledCards.erase(shuffledCards.begin());
	}
}

struct Player
{
	PlayerInfo* info;
	sf::TcpSocket* socket;

	Player(PlayerInfo* _info, sf::TcpSocket* _socket)
	{
		info = _info;
		socket = _socket;
	}
};

void InitializeBoard(std::string _board[COLUMNS][ROWS])
{
	//Invernadero
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_board[i][j] = "Invernadero";
		}
	}

	//Sala de billar
	for (int i = 12; i < 18; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_board[i][j] = "Sala de Billar";
		}
	}

	//Biblioteca
	for (int i = 22; i < 28; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_board[i][j] = "Biblioteca";
		}
	}

	//Estudio
	for (int i = 32; i < 40; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_board[i][j] = "Estudio";
		}
	}

	//Sala de baile
	for (int i = 0; i < 8; i++)
	{
		for (int j = 12; j < 18; j++)
		{
			_board[i][j] = "Sala de baile";
		}
	}

	//vestibulo
	for (int i = 30; i < 40; i++)
	{
		for (int j = 12; j < 20; j++)
		{
			_board[i][j] = "Vestibulo";
		}
	}

	//cocina
	for (int i = 0; i < 10; i++)
	{
		for (int j = 20; j < 30; j++)
		{
			_board[i][j] = "Cocina";
		}
	}

	//comedor
	for (int i = 13; i < 26; i++)
	{
		for (int j = 20; j < 30; j++)
		{
			_board[i][j] = "Comedor";
		}
	}

	//salon
	for (int i = 30; i < 40; i++)
	{
		for (int j = 22; j < 30; j++)
		{
			_board[i][j] = "Salon";
		}
	}

	//medio
	for (int i = 12; i < 26; i++)
	{
		for (int j = 12; j < 18; j++)
		{
			_board[i][j] = "Medio";
		}
	}

	//nada
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (_board[j][i].empty()) _board[j][i] = "Nada";
		}
	}
}

Vector2 GetRandomPosition(std::string _board[COLUMNS][ROWS])
{
	int rx, ry;
	rx = rand() % COLUMNS - 1;
	ry = rand() % ROWS - 1;
	Vector2 pos;
	if (_board[rx][ry] != "Nada") pos = GetRandomPosition(_board);
	else
	{
		pos.x = rx;
		pos.y = ry;
	}
	return pos;
}

void SetRandomInitialPositions(std::string _board[COLUMNS][ROWS], PlayerInfo* &_player)
{
	Vector2 newPosition = GetRandomPosition(_board);
	_board[newPosition.x][newPosition.y] = "Nada" + _player->nickname;
	_player->position.x = newPosition.x;
	_player->position.y = newPosition.y;
}

bool CheckNextMovement(int direction, PlayerInfo* player, std::string board[COLUMNS][ROWS], bool &roomFound)
{
	int x = 0;
	int y = 0;
	switch (direction)
	{
	case 1:
		x = player->position.x - 1;
		y = player->position.y;
		break;
	case 2:
		x = player->position.x + 1;
		y = player->position.y;
		break;
	case 3:
		x = player->position.x;
		y = player->position.y - 1;
		break;
	case 4:
		x = player->position.x;
		y = player->position.y + 1;
		break;
	default:
		break;
	}

	Vector2 lastPos = Vector2(player->position.x, player->position.y);

	if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS || board[x][y] == "Medio")
	{
		roomFound = false;
		return false;
	}
	else if (board[x][y] == "Invernadero" || board[x][y] == "Sala de Billar" || board[x][y] == "Biblioteca" || board[x][y] == "Estudio" || board[x][y] == "Sala de baile" || board[x][y] == "Vestibulo" || board[x][y] == "Cocina" || board[x][y] == "Comedor" || board[x][y] == "Salon")
	{
		for (int i = 0; i < player->nickname.size(); i++)
		{
			board[lastPos.x][lastPos.y].pop_back();
		}
		board[x][y] += player->nickname;
		player->position.x = x;
		player->position.y = y;
		roomFound = true;
		return true;
	}
	else if (board[x][y] == "Nada")
	{
		for (int i = 0; i < player->nickname.size(); i++)
		{
			board[lastPos.x][lastPos.y].pop_back();
		}
		board[x][y] += player->nickname;
		player->position.x = x;
		player->position.y = y;
		roomFound = false;
		return true;
	}
	else
	{
		roomFound = false;
		return false;
	}

	return true;
}

int main()
{
	srand(time(NULL));
	std::vector<sf::Vector2f> playersPositions;
	std::vector<sf::Color> playersColors;
	Graphics g;

	bool connected = false;
	bool draw = false;
	bool firstTurn = true;
	bool roomFound = false;
	bool moving = false;
	int matchPlayers = 0;
	int movesLeft;

	std::string board[COLUMNS][ROWS];
	InitializeBoard(board);

	std::string nickname;

	std::cout << "Bienvenido a CLUEDO, escoge tu nombre:\n";
	std::cin >> nickname;

	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::milliseconds(15.f));
	std::vector<card> full_Deck, shuffledCards, finalCards, myDeck;
	initializeCards(full_Deck);

	sf::SocketSelector selector;
	std::vector<Player> playersSockets;
	int index = 0;

	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		int aux;
		Events events;
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
				events = (Events)aux;
				switch (events)
				{
				case Events::CHOOSE_NUM_PLAYERS:
				{
					system("CLS");
					int numPlayers = 0;
					while (numPlayers < 3 || numPlayers > 6)
					{
						std::cout << "Choose how many player will play this game (from 3 to 6 players)" << std::endl;
						std::cin >> numPlayers;
					}
					packet.clear();
					packet << static_cast<int32_t>(Events::NUM_PLAYERS) << numPlayers;
					socket.send(packet);
					break;
				}
				case Events::CONNECT:
				{
					packet >> aux;
					int port;
					for (int i = 0; i < aux; i++)
					{
						packet >> port >> data;
						sf::TcpSocket* sock = new sf::TcpSocket;
						sf::Socket::Status st = sock->connect(data, port, sf::seconds(5.f));
						playersSockets.push_back({ new PlayerInfo, sock });
					}
					packet >> matchPlayers;
					clientPort = socket.getLocalPort();
					playersSockets.push_back({ new PlayerInfo, nullptr });
					playersSockets.back().info->nickname = nickname;
					index = aux;
					switch (aux)
					{
					case 1:
					{
						playersSockets.back().info->color = sf::Color(0, 255, 0, 255);
						break;
					}
					case 2:
					{
						playersSockets.back().info->color = sf::Color(0, 0, 255, 255);
						break;
					}
					case 3:
					{
						playersSockets.back().info->color = sf::Color(255, 255, 0, 255);
						break;
					}
					case 4:
					{
						playersSockets.back().info->color = sf::Color(255, 0, 255, 255);
						break;
					}
					case 5:
					{
						playersSockets.back().info->color = sf::Color(0, 255, 255, 255);
						break;
					}
					default:
						break;
					}
					socket.disconnect();
					waiting = false;
					break;
				}
				case Events::LISTEN:
				{
					packet >> matchPlayers;
					clientPort = socket.getLocalPort();
					playersSockets.push_back({ new PlayerInfo, nullptr });
					playersSockets.back().info->nickname = nickname;
					playersSockets.back().info->color = sf::Color(255, 0, 0, 255);
					index = 0;
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

		if (playersSockets.size() != matchPlayers)
		{
			system("CLS");
			std::cout << "WWaiting for players" << std::endl;
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
				while (playersSockets.size() < matchPlayers)
				{
					if (statusL != sf::Socket::Done)
					{
						std::cout << "Error al recoger conexión nueva\n";
					}

					sf::TcpSocket *newPlayer = new sf::TcpSocket;
					sf::Socket::Status statusNP = listener.accept(*newPlayer);
					if (statusNP == sf::Socket::Done)
					{
						std::cout << "A new player has joined" << std::endl;
						playersSockets.push_back({ new PlayerInfo, newPlayer });
						if (playersSockets.size() != matchPlayers)
						{
							std::cout << "WWaiting for players" << std::endl;
						}
					}
				}
			}
		}

		for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
		{
			Player& player = *it;
			if (player.socket != nullptr)
			{
				sf::TcpSocket& socket = *player.socket;

				socket.setBlocking(false);
			}
		}

		g.InitDungeon();
		draw = true;

		packet.clear();
		packet << static_cast<int32_t>(Events::READY) << nickname << playersSockets[index].info->color.r << playersSockets[index].info->color.g << playersSockets[index].info->color.b << playersSockets[index].info->color.a;

		for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
		{
			Player& player = *it;
			if (player.socket != nullptr)
			{
				sf::TcpSocket& pSocket = *player.socket;
				pSocket.send(packet);
			}
		}

		packet.clear();

		if (index == 0)
		{
			SetRandomInitialPositions(board, playersSockets[index].info);
			packet << static_cast<int32_t>(Events::SET_POSITION) << index << playersSockets[index].info->position.x << playersSockets[index].info->position.y;

			for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{
				Player& player = *it;
				if (player.socket != nullptr)
				{
					sf::TcpSocket& pSocket = *player.socket;
					pSocket.send(packet);
				}
			}

			packet.clear();

			ShuffleCards(shuffledCards, full_Deck);
			TakeFinalCards(finalCards, shuffledCards);

			packet << static_cast<int32_t>(Events::FINAL_CARDS) << (int)finalCards.size();
			for (int i = 0; i < finalCards.size(); i++)
			{
				packet << finalCards[i].name << static_cast<int32_t>(finalCards[i].type);
			}

			for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{
				Player& player = *it;
				if (player.socket != nullptr)
				{
					sf::TcpSocket& pSocket = *player.socket;
					pSocket.send(packet);
				}
			}

			packet.clear();

			AssignCards(shuffledCards, myDeck, matchPlayers - index);

			for (int i = 0; i < myDeck.size(); i++)
			{
				std::cout << myDeck[i].name << std::endl;
			}

			packet << static_cast<int32_t>(Events::PICK_CARDS) << (int)shuffledCards.size();
			for (int i = 0; i < shuffledCards.size(); i++)
			{
				packet << shuffledCards[i].name << static_cast<int32_t>(shuffledCards[i].type);
			}

			int i = 0;
			for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{
				if (i == index + 1)
				{
					Player& player = *it;
					if (player.socket != nullptr)
					{
						sf::TcpSocket& pSocket = *player.socket;
						pSocket.send(packet);
					}
				}
				i++;
			}	
		}
		
		system("CLS");
		connected = true;

		while (connected)
		{
			packet.clear();

			for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{
				Player& player = *it;
				if (player.socket != nullptr)
				{
					sf::TcpSocket& pSocket = *player.socket;
					status = pSocket.receive(packet);
					if (status == sf::Socket::NotReady)
					{

					}
					else if (status == sf::Socket::Done)
					{
						int aux2;
						Events events2;
						std::string data2;
						packet >> aux2;
						events2 = (Events)aux2;
						switch (events2)
						{
						case Events::READY:
						{
							sf::Color c;
							packet >> data2 >> c.r >> c.g >> c.b >> c.a;
							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								Player& player2 = *it;
								if (player2.socket != nullptr)
								{
									sf::TcpSocket& pSocket2 = *player2.socket;
									if (pSocket.getRemoteAddress() == pSocket2.getRemoteAddress() && pSocket.getRemotePort() == pSocket2.getRemotePort())
									{
										player2.info->nickname = data2;
										player2.info->color = c;
									}
								}
							}
							break;
						}
						case Events::SET_POSITION:
						{
							sf::Vector2f pos;
							packet >> aux >> pos.x >> pos.y;
							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								Player& player2 = *it;
								if (player2.socket != nullptr)
								{
									sf::TcpSocket& pSocket2 = *player2.socket;
									if (pSocket.getRemoteAddress() == pSocket2.getRemoteAddress() && pSocket.getRemotePort() == pSocket2.getRemotePort())
									{
										player2.info->position = pos;
										board[(int)player2.info->position.x][(int)player2.info->position.y] += player2.info->nickname;
									}
								}
							}

							packet.clear();

							if (index == aux + 1)
							{
								SetRandomInitialPositions(board, playersSockets[index].info);
								packet << static_cast<int32_t>(Events::SET_POSITION) << index << playersSockets[index].info->position.x << playersSockets[index].info->position.y;

								for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
								{
									Player& player3 = *it;
									if (player3.socket != nullptr)
									{
										sf::TcpSocket& pSocket3 = *player3.socket;
										pSocket3.send(packet);
									}
								}
							}

							break;
						}
						case Events::FINAL_CARDS:
						{
							int size = 0;
							packet >> size;
							for (int i = 0; i < size; i++)
							{
								card aux2;
								packet >> aux2.name >> aux;
								aux2.type = (CardType)aux;
								finalCards.push_back(aux2);
							}
							packet.clear();
							break;
						}
						case Events::PICK_CARDS:
						{
							int size = 0;
							packet >> size;
							for (int i = 0; i < size; i++)
							{
								card aux2;
								packet >> aux2.name >> aux;
								aux2.type = (CardType)aux;
								shuffledCards.push_back(aux2);
							}
							packet.clear();

							AssignCards(shuffledCards, myDeck, matchPlayers - index);

							if (index < matchPlayers - 1)
							{
								packet << static_cast<int32_t>(Events::PICK_CARDS) << (int)shuffledCards.size();
								for (int i = 0; i < shuffledCards.size(); i++)
								{
									packet << shuffledCards[i].name << static_cast<int32_t>(shuffledCards[i].type);
								}

								int i = 0;
								for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
								{
									if (i == index + 1)
									{
										Player& player = *it;
										if (player.socket != nullptr)
										{
											sf::TcpSocket& pSocket = *player.socket;
											pSocket.send(packet);
										}
									}
									i++;
								}
							}
							else
							{
								packet << static_cast<int32_t>(Events::TURN) << 0;
								Player& player = playersSockets.front();
								sf::TcpSocket& pSocket = *player.socket;
								pSocket.send(packet);
							}
							break;
						}
						case Events::TURN:
						{
							packet >> aux;

							if (aux == index)
							{
								system("CLS");
								std::cout << playersSockets[index].info->nickname;
								switch (index)
								{
								case 0:
									std::cout << ", your color is red and your cards are: " << myDeck.front().name;
									break;
								case 1:
									std::cout << ", your color is green and your cards are: " << myDeck.front().name;
									break;
								case 2:
									std::cout << ", your color is blue and your cards are: " << myDeck.front().name;
									break;
								case 3:
									std::cout << ", your color is yellow and your cards are: " << myDeck.front().name;
									break;
								case 4:
									std::cout << ", your color is magenta and your cards are: " << myDeck.front().name;
									break;
								case 5:
									std::cout << ", your color is cyab and your cards are: " << myDeck.front().name;
									break;
								default:
									break;
								}
								for (int i = 1; i < myDeck.size(); i++)
								{
									std::cout << ", " << myDeck[i].name;
								}
								std::cout << std::endl;

								int first = (rand() % 6) + 1;
								int second = (rand() % 6) + 1;
								movesLeft = first + second;
								std::cout << "Your dice score is: " << movesLeft << "(" << first << ", " << second << ")" << std::endl;

								packet.clear();

								if (first == 1 || second == 1)
								{
									moving = false;
									CardType type = CardType((rand() % 3));
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

									for (int i = 0; i < finalCards.size(); i++)
									{
										if (nameCard == finalCards[i].name)
										{
											packet << static_cast<int32_t>(Events::CLUE_RESPONSE) << index << nameCard << -1;
											std::cout << playersSockets[index].info->nickname << " has requested for " << nameCard << " card and nobody has it" << std::endl;
											moving = true;
											for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
											{
												Player& player = *it;
												if (player.socket != nullptr)
												{
													sf::TcpSocket& pSocket = *player.socket;
													pSocket.send(packet);
												}
											}
										}
									}

									packet.clear();

									for (int i = 0; i < myDeck.size(); i++)
									{
										if (nameCard == myDeck[i].name)
										{
											packet << static_cast<int32_t>(Events::CLUE_RESPONSE) << index << nameCard << index;									
											std::cout << playersSockets[index].info->nickname << " has requested for " << nameCard << " card and " << playersSockets[index].info->nickname << " has it" << std::endl;
											moving = true;
											for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
											{
												Player& player = *it;
												if (player.socket != nullptr)
												{
													sf::TcpSocket& pSocket = *player.socket;
													pSocket.send(packet);
												}
											}
										}
									}

									packet.clear();

									packet << static_cast<int32_t>(Events::CLUE_REQUEST) << nameCard << index;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										Player& player = *it;
										if (player.socket != nullptr)
										{
											sf::TcpSocket& pSocket = *player.socket;
											pSocket.send(packet);
										}
									}
								}
								else moving = true;
							}

							packet.clear();
							break;
						}
						case Events::CLUE_REQUEST:
						{
							int _index;
							packet >> data >> _index;
							packet.clear();

							for (int i = 0; i < myDeck.size(); i++)
							{
								if (myDeck[i].name == data)
								{
									packet << static_cast<int32_t>(Events::CLUE_RESPONSE) << _index << data << index;
									std::cout << playersSockets[_index].info->nickname << " has requested for " << data << " card and " << playersSockets[index].info->nickname << " has it" << std::endl;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										Player& player = *it;
										if (player.socket != nullptr)
										{
											sf::TcpSocket& pSocket = *player.socket;
											pSocket.send(packet);
										}
									}
								}
							}

							break;
						}
						case Events::CLUE_RESPONSE:
						{
							int i1, i2;
							packet >> i1 >> data >> i2;
							packet.clear();

							if (i2 == -1) std::cout << playersSockets[i1].info->nickname << " has requested for " << data << " card and nobody has it" << std::endl;
							else std::cout << playersSockets[i1].info->nickname << " has requested for " << data << " card and " << playersSockets[i2].info->nickname << " has it" << std::endl;

							if(index == i1) moving = true;

							break;
						}
						case Events::ACTUALIZE_POSITION:
						{
							int _index = 0;
							packet >> _index;
							packet >> playersSockets[_index].info->position.x >> playersSockets[_index].info->position.y;
							packet.clear();
							break;
						}
						case Events::DENY_RESPONSE:
						{
							system("CLS");
							std::string deductionPlayer;
							int carmen;
							std::string cardsToDeny[3];
							for (int i = 0; i < 3; i++)
							{
								packet >> cardsToDeny[i];
								std::cout << "-" << cardsToDeny[i] << std::endl;
							}
							packet >> carmen;
							std::cout << carmen << std::endl;
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
								packet << static_cast<int32_t>(Events::SHOW_DENY_RESPONSE) << denyCard.name << carmen << static_cast<int32_t>(denyCard.type);
								std::cout << "Player " << playersSockets[index].info->nickname << " denied card-> " << denyCard.name << std::endl;
								int i = 0;
								for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
								{
									if (i == carmen)
									{
										Player& player = *it;
										if (player.socket != nullptr)
										{
											sf::TcpSocket& pSocket = *player.socket;
											pSocket.send(packet);
										}
									}
									i++;
								}
							}
							else
							{
								system("CLS");
								std::cout << "You don't have any cards to deny" << std::endl;
								int hola = index + 1;
								if (hola == playersSockets.size()) hola = 0;
								std::cout << hola << " " << carmen << std::endl;
								if (hola == carmen)
								{
									packet << static_cast<int32_t>(Events::SHOW_DENY_RESPONSE) << "NONE" << carmen;
									int j = 0;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										if (j == carmen)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												sf::TcpSocket& pSocket = *player.socket;
												pSocket.send(packet);
											}
										}
										j++;
									}
								}
								else
								{
									packet << static_cast<int32_t>(Events::DENY_RESPONSE) << cardsToDeny[0] << cardsToDeny[1] << cardsToDeny[2] << carmen;
									Player& player = playersSockets[hola];
									sf::TcpSocket& pSocket = *player.socket;
									pSocket.send(packet);
								}
							}
							break;
						}
						case Events::SHOW_DENY_RESPONSE:
						{
							card denyCard;
							int aux;
							std::string _nickname;
							int _index = 0;
							packet >> _nickname >> _index;
							if (_nickname == "NONE" && _index == index)
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
									std::string answere[3];

									while (cardAnswer < 1 || cardAnswer > character.size()) {
										std::cin >> cardAnswer;
									}

									answere[0] = character[cardAnswer - 1];

									cardAnswer = 0;
									system("CLS");
									std::cout << "SELECT A WEAPON" << std::endl;
									for (int i = 0; i < weapon.size(); i++)
										std::cout << i + 1 << "-" << weapon[i] << std::endl;

									while (cardAnswer < 1 || cardAnswer > weapon.size()) {
										std::cin >> cardAnswer;
									}

									answere[1] = weapon[cardAnswer - 1];

									cardAnswer = 0;
									system("CLS");
									std::cout << "SELECT A ROOM" << std::endl;
									for (int i = 0; i < room.size(); i++)
										std::cout << i + 1 << "-" << room[i] << std::endl;

									while (cardAnswer < 1 || cardAnswer > room.size()) {
										std::cin >> cardAnswer;
									}

									answere[2] = room[cardAnswer - 1];

									int count = 0;
									for (int i = 0; i < 3; i++)
									{
										for (int j = 0; j < 3; j++)
										{
											if (answere[i] == finalCards[j].name)
											{
												count++;
											}
										}
									}

									if (count == 3)
									{
										packet << static_cast<int32_t>(Events::ACCUSATION_RESULT) << true << index;
										system("CLS");
										std::cout << playersSockets[index].info->nickname << " has won" << std::endl;
										for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												sf::TcpSocket& pSocket = *player.socket;
												pSocket.send(packet);
											}
										}
										for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												player.socket->disconnect();
											}
										}
									}
									else
									{
										int i = index + 1;
										if (i == playersSockets.size()) i = 0;
										if (i == _index)
										{
											packet << static_cast<int32_t>(Events::ACCUSATION_RESULT) << false;
											for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
											{
												Player& player = *it;
												if (player.socket != nullptr)
												{
													sf::TcpSocket& pSocket = *player.socket;
													pSocket.send(packet);
												}
											}
										}
										else
										{
											packet << static_cast<int32_t>(Events::ACCUSATION) << _index;
											int j = 0;
											for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
											{
												if (j == i)
												{
													Player& player = *it;
													if (player.socket != nullptr)
													{
														sf::TcpSocket& pSocket = *player.socket;
														pSocket.send(packet);
													}
												}
												j++;
											}
										}
									}

									system("CLS");
								}
								else
								{
									packet.clear();
									system("CLS");
									std::cout << "Turn finished" << std::endl;
									int t = index + 1;
									if (t == matchPlayers) t = 0;
									packet << static_cast<int32_t>(Events::TURN) << t;
									int i = 0;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										if (i == t)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												sf::TcpSocket& pSocket = *player.socket;
												pSocket.send(packet);
											}
										}
										i++;
									}
								}
							}
							else if(_nickname != "NONE")
							{
								packet >> denyCard.name >> aux;
								denyCard.type = (CardType)aux;
								std::cout << "Player " << _nickname << " denied card-> " << denyCard.name << std::endl;
								packet.clear();
								{
									packet.clear();
									system("CLS");
									std::cout << "Turn finished" << std::endl;
									int t = index + 1;
									if (t == matchPlayers) t = 0;
									packet << static_cast<int32_t>(Events::TURN) << t;
									int i = 0;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										if (i == t)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												sf::TcpSocket& pSocket = *player.socket;
												pSocket.send(packet);
											}
										}
										i++;
									}
								}
							}
							break;
						}
						case Events::ACCUSATION:
						{
							int _index = 0;
							packet >> _index;
							packet.clear();

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
							std::string answere[3];

							while (cardAnswer < 1 || cardAnswer > character.size()) {
								std::cin >> cardAnswer;
							}

							answere[0] = character[cardAnswer - 1];

							cardAnswer = 0;
							system("CLS");
							std::cout << "SELECT A WEAPON" << std::endl;
							for (int i = 0; i < weapon.size(); i++)
								std::cout << i + 1 << "-" << weapon[i] << std::endl;

							while (cardAnswer < 1 || cardAnswer > weapon.size()) {
								std::cin >> cardAnswer;
							}

							answere[1] = weapon[cardAnswer - 1];

							cardAnswer = 0;
							system("CLS");
							std::cout << "SELECT A ROOM" << std::endl;
							for (int i = 0; i < room.size(); i++)
								std::cout << i + 1 << "-" << room[i] << std::endl;

							while (cardAnswer < 1 || cardAnswer > room.size()) {
								std::cin >> cardAnswer;
							}

							answere[2] = room[cardAnswer - 1];

							int count = 0;
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 3; j++)
								{
									if (answere[i] == finalCards[j].name)
									{
										count++;
									}
								}
							}

							if (count == 3)
							{
								packet << static_cast<int32_t>(Events::ACCUSATION_RESULT) << true << index;
								system("CLS");
								std::cout << playersSockets[index].info->nickname << " has won" << std::endl;

								for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
								{
									Player& player = *it;
									if (player.socket != nullptr)
									{
										sf::TcpSocket& pSocket = *player.socket;
										pSocket.send(packet);
									}
								}

								for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
								{
									Player& player = *it;
									if (player.socket != nullptr)
									{
										player.socket->disconnect();
									}
								}
							}
							else
							{
								int i = index + 1;
								if (i == playersSockets.size()) i = 0;
								if (i == _index)
								{
									packet << static_cast<int32_t>(Events::ACCUSATION_RESULT) << false;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										Player& player = *it;
										if (player.socket != nullptr)
										{
											sf::TcpSocket& pSocket = *player.socket;
											pSocket.send(packet);
										}
									}
								}
								else
								{
									packet << static_cast<int32_t>(Events::ACCUSATION) << _index;
									int j = 0;
									for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
									{
										if (j == i)
										{
											Player& player = *it;
											if (player.socket != nullptr)
											{
												sf::TcpSocket& pSocket = *player.socket;
												pSocket.send(packet);
											}
										}
										j++;
									}
								}
							}
							break;
						}
						case Events::ACCUSATION_RESULT:
						{
							bool result;
							packet >> result;

							if (result)
							{
								int _index = 0;
								packet >> _index;
								std::cout << playersSockets[_index].info->nickname << " has won" << std::endl;
							}
							else
							{
								std::cout << "Nobody has won" << std::endl;
							}

							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								Player& player = *it;
								if (player.socket != nullptr)
								{
									player.socket->disconnect();
								}
							}

							break;
						}
						default:
							break;
						}
					}
					else if (status == sf::Socket::Disconnected)
					{
						std::cout << "Elimino el socket que se ha desconectado\n";
						pSocket.disconnect();
					}
					else
					{
						std::cout << "Error al recibir\n";
						return 0;
					}
				}
			}

			while (moving)
			{
				std::cout << movesLeft << " moves left" << std::endl;
				int direction = 0;
				while (direction < 1 || direction > 4)
				{	
					std::cout << "Choose a direction (1=left, 2=right, 3=up, 4=down)" << std::endl;
					std::cin >> direction;
				}
				system("CLS");
				roomFound = false;
				bool canMove = CheckNextMovement(direction, playersSockets[index].info, board, roomFound);
				if (canMove)
				{
					int i = 0;
					for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
					{
						Player& player = *it;
						playersPositions.push_back((sf::Vector2f)player.info->position);
						playersColors.push_back(player.info->color);
						i++;
					}
					g.DrawDungeon(playersPositions, playersColors, draw, playersSockets.front().info->direction);
					while (!playersColors.empty() && !playersPositions.empty())
					{
						playersColors.pop_back();
						playersPositions.pop_back();
					}

					if (roomFound)
					{
						movesLeft = 0;
						moving = false;
						packet.clear();
						packet << static_cast<int32_t>(Events::ACTUALIZE_POSITION) << index << playersSockets[index].info->position.x << playersSockets[index].info->position.y;
						for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
						{
							Player& player = *it;
							if (player.socket != nullptr)
							{
								sf::TcpSocket& pSocket = *player.socket;
								pSocket.send(packet);
							}
						}

						packet.clear();

						std::cout << "Room " << board[(int)playersSockets[index].info->position.x][(int)playersSockets[index].info->position.y] << " has been found" << std::endl;
						
						do
						{
							std::cout << "Do you want to make a deduction? (Y/N)" << std::endl;
							data.clear();
							std::cin >> data;
							system("CLS");
						} while (data != "y" && data != "Y" && data != "N" && data != "n");
						if (data == "y" || data == "Y")
						{
							packet << static_cast<int32_t>(Events::DENY_RESPONSE);

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

							packet << room[cardAnswer - 1] << index;


							system("CLS");

							int i = index + 1;
							if (i == playersSockets.size()) i = 0;
							Player& player = playersSockets[i];
							sf::TcpSocket& pSocket = *player.socket;
							pSocket.send(packet);
						}
						else
						{
							packet.clear();
							system("CLS");
							std::cout << "Turn finished" << std::endl;
							int t = index + 1;
							if (t == matchPlayers) t = 0;
							packet << static_cast<int32_t>(Events::TURN) << t;
							int i = 0;
							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								if (i == t)
								{
									Player& player = *it;
									if (player.socket != nullptr)
									{
										sf::TcpSocket& pSocket = *player.socket;
										pSocket.send(packet);
									}
								}
								i++;
							}
						}
					}
					else
					{
						movesLeft--;
						if (movesLeft == 0)
						{
							moving = false;
							packet.clear();
							packet << static_cast<int32_t>(Events::ACTUALIZE_POSITION) << index << playersSockets[index].info->position.x << playersSockets[index].info->position.y;
							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								Player& player = *it;
								if (player.socket != nullptr)
								{
									sf::TcpSocket& pSocket = *player.socket;
									pSocket.send(packet);
								}
							}
							packet.clear();
							system("CLS");
							std::cout << "Turn finished" << std::endl;
							int t = index + 1;
							if (t == matchPlayers) t = 0;
							packet << static_cast<int32_t>(Events::TURN) << t;
							int i = 0;
							for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
							{
								if (i == t)
								{
									Player& player = *it;
									if (player.socket != nullptr)
									{
										sf::TcpSocket& pSocket = *player.socket;
										pSocket.send(packet);
									}
								}
								i++;
							}
						}
					}
				}
			}

			int i = 0;
			for (auto it = playersSockets.begin(); it != playersSockets.end(); ++it)
			{
				Player& player = *it;
				playersPositions.push_back((sf::Vector2f)player.info->position);
				playersColors.push_back(player.info->color);
				i++;
			}
			g.DrawDungeon(playersPositions, playersColors, draw, playersSockets.front().info->direction);
			while (!playersColors.empty() && !playersPositions.empty())
			{
				playersColors.pop_back();
				playersPositions.pop_back();
			}
		}
		g.ClearDungeon();
	}

	return 0;
}