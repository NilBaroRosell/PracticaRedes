#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Types.h>
#include <list>

///// SERVER /////

void AddNewPlayer(std::list<PlayerInfo> &_players, std::string _username) {
	
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

void SetRandomInitialPositions(std::string _board[ROWS][COLUMNS], std::vector<std::string> _usernames, Vector2 _playersPositions[6], int _numPlayers)
{
	for (int i = 0; i < _numPlayers; i++)
	{
		Vector2 newPosition = GetRandomPosition(_board);
		_board[newPosition.x][newPosition.y] = _usernames[i];
		_playersPositions[i].x = newPosition.x;
		_playersPositions[i].y = newPosition.y;
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

void AssignCards(std::vector<card> shuffledCards, std::vector<card> _playersCards[], int _numPlayers)
{
	int i = 0;
	while (!shuffledCards.empty())
	{
		_playersCards[i].push_back(shuffledCards.front());
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

	//Inicializar todas las cartas
	initializeCards();
	//initializeBoard();
	std::vector<card> shuffledCards;
	std::vector<card> finalCards;
	ShuffleCards(shuffledCards, full_Deck);
	TakeFinalCards(finalCards, shuffledCards);
	std::vector<card> playerCards[6];
	Vector2 playersPositions[6];

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
	std::vector<std::string> usernames;
	sf::SocketSelector selector;
	sf::Packet packet;
	selector.add(listener);
	int matchPlayers = 3;

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
								packet >> data;
								usernames.push_back(data);
								if (numPlayers == 1)
								{
									packet.clear();
									packet << static_cast<int32_t>(Comands::CHOOSE_NUM_PLAYERS);
									sf::TcpSocket& client = *clients.front();
									client.send(packet);
								}
								break;
							case Comands::NUM_PLAYERS:
								packet >> matchPlayers;
								if (matchPlayers > 6) matchPlayers = 6;
								else if (matchPlayers < 3) matchPlayers = 3;
								break;
							case Comands::DEDUCTION:
								bool wantDeduction;
								packet >> wantDeduction;
								if (wantDeduction)
								{
									std::string _character, _gun, _room;
									packet >> _character >> _gun >> _room;
									packet.clear();
									packet << static_cast<int32_t>(Comands::DENY) << _character << _gun << _room;
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
					if (numPlayers >= matchPlayers) //mirar si hay mas de 3 clientes
					{
						matchStarted = true;
						std::cout << numPlayers << std::endl;
						AssignCards(shuffledCards, playerCards, numPlayers - 1);
						SetRandomInitialPositions(board, usernames, playersPositions, numPlayers);
						int i = 0;
						for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
						{
							std::cout << playerCards[i].size() << std::endl;
							int position[2] = { playersPositions[i].x, playersPositions[i].y };
							int color[4]{ playersColors[i].r, playersColors[i].g, playersColors[i].b, playersColors[i].a };
							packet << static_cast<int32_t>(Comands::START) << usernames.back() << static_cast<int32_t>(playerCards[i].size()) << playersPositions[i].x << playersPositions[i].y << playersColors[i].r << playersColors[i].g << playersColors[i].b << playersColors[i].a;
							for (auto it = playerCards[i].begin(); it != playerCards[i].end(); it++)
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
						packet << static_cast<int32_t>(Comands::WAIT) << usernames.back();
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