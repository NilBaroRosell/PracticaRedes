#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <list>

int main()
{
	//PlayerInfo playerInfo;
	bool serverRunning = true;	//Controla que el servidor este abierto

	// Creamos TCPListener para escuchar las conexiones entrantes
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(55556);	
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	// Creamos lista de clientes
	std::list<sf::TcpSocket*> clients;	
	sf::SocketSelector selector;
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
				//std::cout << "O" << std::endl;
				// The listener socket is not ready, test all other sockets (the clients)
				/*for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							std::string strRec;
							packet >> strRec;
							std::cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << std::endl;
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
				}*/
			}
		}
	}

	return 0;
}