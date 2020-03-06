#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Types.h>
#include <list>

///// SERVER /////

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

void InitializeBoard(std::string _board[ROWS][COLUMNS])
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
			_board[i][j] = "Sala de billar";
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
		for (int j = 12; j < 18; j++)
		{
			_board[i][j] = "vestibulo";
		}
	}

	//cocina
	for (int i = 0; i < 10; i++)
	{
		for (int j = 20; j < 30; j++)
		{
			_board[i][j] = "cocina";
		}
	}

	//comedor
	for (int i = 13; i < 26; i++)
	{
		for (int j = 20; j < 30; j++)
		{
			_board[i][j] = "comedor";
		}
	}

	//salon
	for (int i = 30; i < 40; i++)
	{
		for (int j = 22; j < 30; j++)
		{
			_board[i][j] = "salon";
		}
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if(_board[i][j].empty()) _board[i][j] = "EMPTY";
		}
	}
}

Vector2 GetRandomPosition(std::string _board[ROWS][COLUMNS])
{
	int rx, ry;
	rx = rand() % ROWS - 1;
	ry = rand() % COLUMNS - 1;
	if (_board[rx][ry] != "EMPTY") GetRandomPosition(_board);
	Vector2 pos{ rx, ry };
	return pos;
}

void SetRandomInitialPositions(std::string _board[ROWS][COLUMNS], std::vector<PlayerInfo> &_players, int _numPlayers)
{
	for (int i = 0; i < _numPlayers; i++)
	{
		Vector2 newPosition = GetRandomPosition(_board);
		_board[newPosition.x][newPosition.y] = _players[i].nickname;
		_players[i].position.x = newPosition.x;
		_players[i].position.y = newPosition.y;
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			std::cout << _board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void ShuffleCards(std::vector<card> &_shuffledCards, std::vector<card> _deck)
{
	while(!_deck.empty())
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

	for (auto it = _shuffledCards.begin(); it != _shuffledCards.end(); it++)
	{
		if (!hasCharacter && it->type == CardType::CHARACTER)
		{
			_finalCards.push_back(*it);
			_shuffledCards.erase(it);
			hasCharacter = true;
		}
		else if (!hasWeapon && it->type == CardType::WEAPON)
		{
			_finalCards.push_back(*it);
			_shuffledCards.erase(it);
			hasWeapon = true;
		}
		else if (!hasRoom && it->type == CardType::ROOM)
		{
			_finalCards.push_back(*it);
			_shuffledCards.erase(it);
			hasRoom = true;
		}

		if (hasCharacter && hasWeapon && hasRoom)
		{
			break;
		}
	}
}

void AssignCards(std::vector<card> shuffledCards, std::vector<PlayerInfo> &_players, int _numPlayers)
{
	int i = 0;
	while (!shuffledCards.empty())
	{
		_players[i].playerCards.push_back(shuffledCards.front());
		shuffledCards.erase(shuffledCards.begin());
		i++;
		if (i > _numPlayers)
		{
			i = 0;
		}
	}
}



int main()
{
	srand(time(NULL));
	// Controla que el servidor este abierto
	bool serverRunning = true;
	bool matchStarted = false;

	std::string board[ROWS][COLUMNS];

	InitializeBoard(board);

	
	//initializeBoard();
	std::vector<card> shuffledCards;
	std::vector<card> finalCards;
	std::vector<card> full_Deck;
	//Inicializar todas las cartas
	initializeCards(full_Deck);
	ShuffleCards(shuffledCards, full_Deck);
	TakeFinalCards(finalCards, shuffledCards);
	// TCPListener para escuchar las conexiones entrantes
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(55556);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	// Creamos lista de clientes
	int numPlayers = 0;

	
	sf::Color playersColors[6]{ sf::Color(255, 0, 0, 255), sf::Color(0, 255, 0, 255), sf::Color(0, 0, 255, 255), sf::Color(255, 255, 0, 255), sf::Color(255, 0, 255, 255), sf::Color(0, 255, 255, 255) };
	std::list<sf::TcpSocket*> clients;
	std::vector<PlayerInfo> players;
	sf::SocketSelector selector;
	sf::Packet packet;
	selector.add(listener);
	int matchPlayers = 0;

	while (serverRunning)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				// Si el listener esta "ready" ha habido una conexión
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Añadimos el nuevo cliente a la lista
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					clients.push_back(client);
					selector.add(*client);
					numPlayers++;
				}
				else
				{
					// Error de conexión, eliminamos al cliente
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
				}
			}
			else
			{
				// Si el listener no esta "ready", miramos el resto de sockets
				
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					int i = 0;
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// El cliente envia información, la recibimos
						sf::Packet packet;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							int aux;
							Comands comand;
							std::string data;
							packet >> aux;
							comand = (Comands)aux;
							switch (comand)
							{
							case Comands::READY:
							{packet >> data;
							PlayerInfo newPlayer;
							newPlayer.nickname = data;
							players.push_back(newPlayer);
							if (numPlayers == 1)
							{
								packet.clear();
								packet << static_cast<int32_t>(Comands::CHOOSE_NUM_PLAYERS);
								sf::TcpSocket& client = *clients.front();
								client.send(packet);
								matchPlayers = 1;
							}
							break;
							}
							case Comands::NUM_PLAYERS:
							{
								packet >> matchPlayers;
								if (matchPlayers > 6) matchPlayers = 6;
								else if (matchPlayers < 3) matchPlayers = 3;
								break;
							}
							case Comands::GO_TO:
							{
								packet.clear();
								packet << static_cast<int32_t>(Comands::ROOM_FOUND);
								client.send(packet);
							}
							case Comands::DEDUCTION:
							{
								bool wantDeduction;
								packet >> wantDeduction;
								if (wantDeduction)
								{
									std::string _character, _gun, _room;
									packet >> _character >> _gun >> _room;
									packet.clear();
									packet << static_cast<int32_t>(Comands::DENY) << players[i].nickname << _character << _gun << _room;
									std::list<sf::TcpSocket*>::iterator leftPlayerIt = it;
									leftPlayerIt++;
									if (leftPlayerIt == clients.end()) leftPlayerIt = clients.begin();
									sf::TcpSocket& leftPlayer = **leftPlayerIt;
									leftPlayer.send(packet);
								}
								else
								{
									std::cout << "No deduction" << std::endl;
								}
								break;
							}
							case Comands::DENY_RESPONSE:
							{
								card denyCard;
								int aux;
								packet >> denyCard.name >> aux;
								denyCard.type = (CardType)aux;
								packet.clear();
								packet << static_cast<int32_t>(Comands::SHOW_DENYRESPONE) << players[i].nickname <<denyCard.name << static_cast<int32_t>(denyCard.type);
								for (std::list<sf::TcpSocket*>::iterator it_aux = clients.begin(); it_aux != clients.end(); ++it_aux)
								{
									sf::TcpSocket& aux_client = **it_aux;
									aux_client.send(packet);
								}
								break;
							}
							default:
								break;
							}
						}
						else if (status == sf::Socket::Disconnected)
						{
							selector.remove(client);
							std::cout << "Elimino el socket que se ha desconectado\n";
							matchStarted = false;
						}
						else
						{
							std::cout << "Error al recibir de " << client.getRemotePort() << std::endl;
						}
					}
				}

				packet.clear();
				if (!matchStarted) {
					if (numPlayers == matchPlayers) //mirar si hay mas de 3 clientes
					{
						matchStarted = true;
						std::cout << numPlayers << std::endl;
						AssignCards(shuffledCards, players, numPlayers - 1);
						SetRandomInitialPositions(board, players, numPlayers);
						int i = 0;
						for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
						{
							std::cout << players[i].playerCards.size() << std::endl;
							int position[2] = { players[i].position.x, players[i].position.y };
							int color[4]{ playersColors[i].r, playersColors[i].g, playersColors[i].b, playersColors[i].a };
							players[i].color = playersColors[i];
							packet << static_cast<int32_t>(Comands::START) << players.back().nickname << static_cast<int32_t>(players[i].playerCards.size()) << players[i].position.x << players[i].position.y << playersColors[i].r << playersColors[i].g << playersColors[i].b << playersColors[i].a;
							for (auto it = players[i].playerCards.begin(); it != players[i].playerCards.end(); it++)
							{
								packet << it->name << static_cast<int32_t>(it->type);
							}
							sf::TcpSocket& client = **it;
							client.send(packet);
							packet.clear();
							i++;
						}
					}
					else
					{
						packet << static_cast<int32_t>(Comands::WAIT) << players.back().nickname;
						for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
						{
							sf::TcpSocket& client = **it;
							client.send(packet);
						}
					}
				}
			}
		}
	}

	return 0;
}