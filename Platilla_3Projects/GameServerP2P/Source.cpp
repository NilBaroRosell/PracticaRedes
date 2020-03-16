#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Types.h>
#include <list>

#define SERVER_PORT 55000

///// SERVER /////

struct socketInfo
{
	std::string ip;
	int port;

	socketInfo(std::string _ip, int _port)
	{
		ip = _ip;
		port = _port;
	}
};

int main()
{
	// Controla que el servidor este abierto
	bool serverRunning = true;

	// TCPListener para escuchar las conexiones entrantes
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(SERVER_PORT);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	
	std::vector<socketInfo> clientsInfo;
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
					std::cout << matchPlayers << " " << clientsInfo.size();
					for (int i = 0; i < clientsInfo.size(); i++)
					{
						std::cout << ", " << clientsInfo[i].port << " " << clientsInfo[i].ip;
					}
					std::cout << std::endl;
					if (clientsInfo.empty())
					{
						packet.clear();
						packet << static_cast<int32_t>(Events::CHOOSE_NUM_PLAYERS);
						client->send(packet);
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						clientsInfo.push_back({ client->getRemoteAddress().toString(), (int)client->getRemotePort() });
						clients.push_back(client);
						selector.add(*client);
					}
					else if (clientsInfo.size() <= matchPlayers - 1)
					{
						packet.clear();
						packet << static_cast<int32_t>(Events::CONNECT) << (int)clientsInfo.size();
						for (auto it = clientsInfo.begin(); it != clientsInfo.end(); it++)
						{
							socketInfo aux = *it;
							packet << aux.port << aux.ip;
						}
						packet << matchPlayers;
						client->send(packet);
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						clientsInfo.push_back({ client->getRemoteAddress().toString(), (int)client->getRemotePort() });
						clients.push_back(client);
						selector.add(*client);
					}
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
							Events events;
							std::string data;
							packet >> aux;
							events = (Events)aux;
							if(events == Events::NUM_PLAYERS)
							{
								packet >> matchPlayers;
								if (matchPlayers > 6) matchPlayers = 6;
								else if (matchPlayers < 3) matchPlayers = 3;
								packet.clear();
								packet << static_cast<int32_t>(Events::LISTEN) << matchPlayers;
								sf::TcpSocket& client = *clients.front();
								client.send(packet);
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
			}
		}
	}

	listener.close();

	return 0;
}