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

int main()
{
	// Controla que el servidor este abierto
	bool serverRunning = true;

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
							packet >> aux >> data;
							comand = (Comands)aux;
							switch (comand)
							{
								case Comands::START:
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
					for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
					{
						//tirar daus
						//mirar si hi ha clue card
						packet << static_cast<int32_t>(Comands::START) << usernames.back();
						sf::TcpSocket& client = **it;
						client.send(packet);
						packet.clear();
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