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

void initializeCards()
{
	//Characters
	full_Deck.push_back({ "Prado", CardType::CHARACTER });
	full_Deck.push_back({ "Rubio", CardType::CHARACTER });
	full_Deck.push_back({ "Orquidea", CardType::CHARACTER });
	full_Deck.push_back({ "Celeste", CardType::CHARACTER });
	full_Deck.push_back({ "Mora", CardType::CHARACTER });
	full_Deck.push_back({ "Amapola", CardType::CHARACTER });

	//Weapons
	full_Deck.push_back({ "Candelabro", CardType::WEAPON });
	full_Deck.push_back({ "Punal", CardType::WEAPON });
	full_Deck.push_back({ "Tuberia de plomo", CardType::WEAPON });
	full_Deck.push_back({ "Pistola", CardType::WEAPON });
	full_Deck.push_back({ "Cuerda", CardType::WEAPON });
	full_Deck.push_back({ "Herramienta", CardType::WEAPON });

	//Rooms
	full_Deck.push_back({ "Sala de baile", CardType::ROOM });
	full_Deck.push_back({ "Sala de billar", CardType::ROOM });
	full_Deck.push_back({ "Invernadero", CardType::ROOM });
	full_Deck.push_back({"Comedor", CardType::ROOM });
	full_Deck.push_back({ "Vestibulo", CardType::ROOM });
	full_Deck.push_back({ "Cocina", CardType::ROOM });
	full_Deck.push_back({ "Biblioteca", CardType::ROOM });
	full_Deck.push_back({ "Salon", CardType::ROOM });
	full_Deck.push_back({ "Estudio", CardType::ROOM });
}

/*void initializeBoard()
{

}*/

void shuffleCards(std::vector<card> &_shuffledCards, std::vector<card> _deck)
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

void takeFinalCards(std::vector<card> &_finalCards, std::vector<card> &_shuffledCards)
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

void assignCards(std::vector<card> shuffledCards, std::vector<card> _playersCards[], int _numPlayers)
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

	//Inicializar todas las cartas
	initializeCards();
	//initializeBoard();
	std::vector<card> shuffledCards;
	std::vector<card> finalCards;
	shuffleCards(shuffledCards, full_Deck);
	takeFinalCards(finalCards, shuffledCards);
	std::vector<card> playerCards[6];

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

	std::list<sf::TcpSocket*> clients;	
	std::list<PlayerInfo> players;	
	std::list<std::string> usernames;
	sf::SocketSelector selector;
	sf::Packet packet;
	selector.add(listener);

	while (serverRunning)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				// Si el listener esta "ready" ha habido una conexi�n
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// A�adimos el nuevo cliente a la lista
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					clients.push_back(client);
					selector.add(*client);
					numPlayers++;
				}
				else
				{
					// Error de conexi�n, eliminamos al cliente
					std::cout << "Error al recoger conexi�n nueva\n";
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
						// El cliente envia informaci�n, la recibimos
						sf::Packet packet;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							int aux;
							Comands comand;
							std::string data;
							packet >> aux >> data;
							comand = (Comands)aux;
							switch (comand)
							{
								case Comands::READY:
									usernames.push_back(data);
									break;
								default:
									break;
							}
						}
						else if (status == sf::Socket::Disconnected)
						{
							selector.remove(client);
							std::cout << "Elimino el socket que se ha desconectado\n";
						}
						else
						{
							std::cout << "Error al recibir de " << client.getRemotePort() << std::endl;
						}
					}
				}

				packet.clear();

				if (numPlayers >= 3) //mirar si hay mas de 3 clientes
				{ 
					std::cout << numPlayers << std::endl;
					assignCards(shuffledCards, playerCards, numPlayers - 1);
					int i = 0;
					for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
					{
						std::cout << playerCards[i].size() << std::endl;
						packet << static_cast<int32_t>(Comands::START) << usernames.back() << static_cast<int32_t>(playerCards[i].size());
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

	return 0;
}