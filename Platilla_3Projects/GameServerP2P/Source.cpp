#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Types.h>
#include <list>

///// SERVER /////

int main()
{
	// Controla que el servidor este abierto
	bool serverRunning = true;

	// TCPListener para escuchar las conexiones entrantes
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(55000);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	
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
					if (clients.empty())
					{
						packet.clear();
						packet << static_cast<int32_t>(Comands::CHOOSE_NUM_PLAYERS);
						client->send(packet);
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						clients.push_back(client);
						selector.add(*client);
					}
					else if(clients.size() < matchPlayers)
					{
						packet.clear();
						packet << static_cast<int32_t>(Comands::CONNECT_AND_LISTEN) << clients.size();
						for (auto it = clients.begin(); it != clients.end(); it++)
						{
							sf::TcpSocket& aux = **it;
							packet << aux.getRemotePort();
						}
						client->send(packet);
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						clients.push_back(client);
						selector.add(*client);
					}
					else if (clients.size() == matchPlayers)
					{
						packet.clear();
						packet << static_cast<int32_t>(Comands::CONNECT);
						client->send(packet);
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						clients.push_back(client);
						selector.add(*client);
					}
					else
					{
						std::cout << "No se admiten mas jugadores" << client->getRemotePort() << std::endl;
						delete client;
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
				int pIdx = 0;
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
							{
								/*packet >> data;
								PlayerInfo newPlayer;
								newPlayer.nickname = data;
								players.push_back(newPlayer);
								if (numPlayers == 1)
								{
									packet.clear();
									packet << static_cast<int32_t>(Comands::CHOOSE_NUM_PLAYERS);
									sf::TcpSocket& client = *clients.front();
									client.send(packet);
								}*/
								break;
							}
							case Comands::NUM_PLAYERS:
							{
								packet >> matchPlayers;
								if (matchPlayers > 6) matchPlayers = 6;
								else if (matchPlayers < 3) matchPlayers = 3;
								packet.clear();
								packet << static_cast<int32_t>(Comands::LISTEN);
								sf::TcpSocket& client = *clients.front();
								client.send(packet);
								break;
							}
							case Comands::READY_START:
							{
								/*packet.clear();
								playersReady++;

								if (playersReady == matchPlayers)
								{
									playersReady = 0;
									int first = (rand() % 6) + 1;
									int second = (rand() % 6) + 1;
									movesLeft = first + second;
									int i = 0;
									for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
									{
										packet << static_cast<int32_t>(Comands::TURN) << players[turn].nickname << numPlayers << players[i].position.x << players[i].position.y << playersColors[i].r << playersColors[i].g << playersColors[i].b << playersColors[i].a;
										for (int j = 0; j < numPlayers; j++)
										{
											if (j != i)
											{
												packet << players[j].position.x << players[j].position.y << playersColors[j].r << playersColors[j].g << playersColors[j].b << playersColors[j].a;
											}
										}
										if (players[i].nickname == players[turn].nickname)
										{
											packet << first << second;
											if (first == 1 || second == 1)
											{
												packet << true << static_cast<int32_t>(rand() % 3);
											}
											else packet << false;
										}
										sf::TcpSocket& client = **it;
										client.send(packet);
										packet.clear();
										i++;
									}
								}
								*/
								break;
							}
							case Comands::CLUE_REQUEST:
							{
								/*CardType type;
								int numCard;
								packet >> data;								
								type = (CardType)aux;
								packet.clear();

								std::string cardHolder = "nobody";

								for (int i = 0; i < players.size(); i++)
								{
									for (int j = 0; j < players[i].playerCards.size(); j++)
									{
										if (players[i].playerCards[j].name == data)
										{
											cardHolder = players[i].nickname;
											break;
										}
									}
								}

								for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
								{
									packet << static_cast<int32_t>(Comands::CLUE_RESPONSE) << players[turn].nickname << data << cardHolder;
									sf::TcpSocket& client = **it;
									client.send(packet);
									packet.clear();
								}
								*/
								break;
							}
							case Comands::GO_TO:
							{
								/*int direction;
								packet >> direction;
								packet.clear();

								if (movesLeft > 0)
								{
									int x = 0;
									int y = 0;
									switch (direction)
									{
										case 1:
											x = players[turn].position.x - 1;
											y = players[turn].position.y;
											break;
										case 2:
											x = players[turn].position.x + 1;
											y = players[turn].position.y;
											break;
										case 3:
											x = players[turn].position.x;
											y = players[turn].position.y - 1;
											break;
										case 4:
											x = players[turn].position.x;
											y = players[turn].position.y + 1;
											break;
										default:
											break;
									}

									if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS || board[x][y] == "Medio")
									{
										packet << static_cast<int32_t>(Comands::MOVE) << movesLeft << static_cast<int32_t>(MoveOptions::BLOCKED);
										int i = 0;
										for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
										{
											if (i == turn)
											{
												sf::TcpSocket& client = **it;
												client.send(packet);
												packet.clear();
												break;
											}
											i++;
										}
									}
									else if (board[x][y] == "Invernadero" || board[x][y] == "Sala de Billar" || board[x][y] == "Biblioteca" || board[x][y] == "Estudio" || board[x][y] == "Sala de baile" || board[x][y] == "Vestibulo" || board[x][y] == "Cocina" || board[x][y] == "Comedor" || board[x][y] == "Salon")
									{
										movesLeft = 0;
										packet << static_cast<int32_t>(Comands::MOVE) << movesLeft << static_cast<int32_t>(MoveOptions::LAST) << players[turn].nickname << x << y << true;

										Vector2 lastPos = Vector2(players[turn].position.x, players[turn].position.y);
										for (int i = 0; i < players[turn].nickname.size(); i++)
										{
											board[lastPos.x][lastPos.y].pop_back();
										}
										board[x][y] += players[turn].nickname;
										players[turn].position.x = x;
										players[turn].position.y = y;

										int i = 0;
										for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
										{
											if (i == turn)
											{
												sf::TcpSocket& client = **it;
												client.send(packet);
												packet.clear();
												break;
											}
											i++;
										}
										
									}
									else if (board[x][y] == "Nada")
									{
										if (movesLeft == 1)
										{
											packet << static_cast<int32_t>(Comands::MOVE) << movesLeft - 1 << static_cast<int32_t>(MoveOptions::LAST) << players[turn].nickname << x << y << false;
										}
										else
										{
											packet << static_cast<int32_t>(Comands::MOVE) << movesLeft - 1 << static_cast<int32_t>(MoveOptions::MOVE) << players[turn].nickname << x << y;
										}

										Vector2 lastPos = Vector2(players[turn].position.x, players[turn].position.y);
										for (int i = 0; i < players[turn].nickname.size(); i++)
										{
											board[lastPos.x][lastPos.y].pop_back();
										}
										board[x][y] += players[turn].nickname;
										players[turn].position.x = x;
										players[turn].position.y = y;

										int i = 0;
										for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
										{
											if (i == turn)
											{
												sf::TcpSocket& client = **it;
												client.send(packet);
												packet.clear();
												break;
											}
											i++;
										}
										movesLeft--;

									}
									else
									{
										packet << static_cast<int32_t>(Comands::MOVE) << movesLeft << static_cast<int32_t>(MoveOptions::BLOCKED);
										int i = 0;
										for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
										{
											if (i == turn)
											{
												sf::TcpSocket& client = **it;
												client.send(packet);
												packet.clear();
												break;
											}
											i++;
										}
									}
								}
								else
								{
									Vector2 actualPos = Vector2(players[turn].position.x, players[turn].position.y);
									std::string roomName = board[actualPos.x][actualPos.y];
									for (int i = 0; i < players[turn].nickname.size(); i++)
									{
										roomName.pop_back();
									}
									std::cout << roomName << std::endl;
									packet << static_cast<int32_t>(Comands::ROOM_FOUND) << roomName;
									int i = 0;
									for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
									{
										if (i == turn)
										{
											sf::TcpSocket& client = **it;
											client.send(packet);
											packet.clear();
											break;
										}
										i++;
									}
								}*/
								break;
							}
							case Comands::DEDUCTION:
							{
									/*std::string _character, _gun, _room;
									if (deductingPlayer == nullptr) {
										deductingPlayer = new PlayerInfo(players[pIdx]);
									}
									packet >> _character >> _gun >> _room;
									packet.clear();
									packet << static_cast<int32_t>(Comands::DENY) << deductingPlayer->nickname << _character << _gun << _room;
									std::list<sf::TcpSocket*>::iterator leftPlayerIt = it;
									int i = pIdx;
									leftPlayerIt++;
									i++;
									if (leftPlayerIt == clients.end()) {
										leftPlayerIt = clients.begin();
										i = 0;
									}
									if (players[i] == *deductingPlayer)
									{
										std::cout << "Any players can deny" << std::endl;
										packet.clear();
										packet << static_cast<int32_t>(Comands::SHOW_DENYRESPONE) << "NONE";
										delete deductingPlayer;
										deductingPlayer = nullptr;
									}
									sf::TcpSocket& leftPlayer = **leftPlayerIt;
									leftPlayer.send(packet);*/
								break;
							}
							case Comands::DENY_RESPONSE:
							{
								/*card denyCard;
								int aux;
								packet >> denyCard.name >> aux;
								denyCard.type = (CardType)aux;
								packet.clear();
								packet << static_cast<int32_t>(Comands::SHOW_DENYRESPONE) << players[pIdx].nickname <<denyCard.name << static_cast<int32_t>(denyCard.type);
								int i = 0;
								for (std::list<sf::TcpSocket*>::iterator it_aux = clients.begin(); it_aux != clients.end(); ++it_aux)
								{
									if (players[i] == *deductingPlayer) {
										sf::TcpSocket& aux_client = **it_aux;
										aux_client.send(packet);
										break;
									}
									i++;
								}
								delete deductingPlayer;
								deductingPlayer = nullptr;*/
								break;
							}
							case Comands::ACCUSATION:
							{
								/*accusationTurn++;
								if (accusationTurn == 3)
								{
									accusationMode = false;
									matchStarted = false;
									break;
								}
								accusationMode = true;
								std::string accusationCards[3];
								packet >> accusationCards[0] >> accusationCards[1] >> accusationCards[2];
								packet.clear();
								int counter = 0;
								for (int i = 0; i < accusationCards->size(); i++)
								{
									for (auto card : finalCards)
									{
										if (card.name == accusationCards[i]) { counter++; }
									}
								}

								if (counter == 3) {
									packet << static_cast<int32_t>(Comands::ACCUSATION_RESULT) << players[pIdx].nickname + " won the game." << "DISCONNECT";
									for (std::list<sf::TcpSocket*>::iterator it_aux = clients.begin(); it_aux != clients.end(); ++it_aux)
									{
										sf::TcpSocket& aux_client = **it_aux;
										aux_client.send(packet);
										//clients.remove(&aux_client);
										//clients
									}

								}
								else {
									packet << static_cast<int32_t>(Comands::ACCUSATION_RESULT) << players[pIdx].nickname + " lost the game." << "NULL";
									client.send(packet);
									//clients.remove(&client);
									if (clients.size() >= 2)
									{
										std::list<sf::TcpSocket*>::iterator leftPlayerIt = it;
										leftPlayerIt++;
										if (leftPlayerIt == clients.end()) {
											leftPlayerIt = clients.begin();
										}
										sf::TcpSocket& accusePlayer = **leftPlayerIt;
										packet.clear();
										packet << static_cast<int32_t>(Comands::ACCUSATION_RESULT) << players[pIdx].nickname + " lost the game." << "ACCUSE";
										accusePlayer.send(packet);
										if (clients.size() >= 3)
										{
											packet.clear();
											packet << static_cast<int32_t>(Comands::ACCUSATION_RESULT) << players[pIdx].nickname + " lost the game." << "WAIT";
											for (std::list<sf::TcpSocket*>::iterator it_aux = clients.begin(); it_aux != clients.end(); ++it_aux)
											{
												sf::TcpSocket& aux_client = **it_aux;
												if(aux_client.getRemoteAddress() != client.getRemoteAddress() && aux_client.getRemoteAddress() != accusePlayer.getRemoteAddress())
													aux_client.send(packet);
											}
										}
									}
								}*/
								break;
							}
							case Comands::TURN_FINISHED:
							{
								/*packet.clear();

								system("CLS");
								turn++;
								if (turn == matchPlayers) turn = 0;

								int first = (rand() % 6) + 1;
								int second = (rand() % 6) + 1;
								movesLeft = first + second;
								int i = 0;
								for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
								{
									packet << static_cast<int32_t>(Comands::TURN) << players[turn].nickname << numPlayers << players[i].position.x << players[i].position.y << playersColors[i].r << playersColors[i].g << playersColors[i].b << playersColors[i].a;
									for (int j = 0; j < numPlayers; j++)
									{
										if (j != i)
										{
											packet << players[j].position.x << players[j].position.y << playersColors[j].r << playersColors[j].g << playersColors[j].b << playersColors[j].a;
										}
									}
									if (players[i].nickname == players[turn].nickname)
									{
										packet << first << second;
										if (first == 1 || second == 1)
										{
											packet << true << static_cast<int32_t>(rand() % 3);
										}
										else packet << false;
									}
									sf::TcpSocket& client = **it;
									client.send(packet);
									packet.clear();
									i++;
								}
								*/
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
							//if(!accusationMode || clients.empty())matchStarted = false;
						}
						else
						{
							std::cout << "Error al recibir de " << client.getRemotePort() << std::endl;
						}
					}
					pIdx++;
				}

				packet.clear();
				/*if (!matchStarted && matchPlayers != 0) 
				{
					if (numPlayers == matchPlayers)
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
							for (auto it2 = players[i].playerCards.begin(); it2 != players[i].playerCards.end(); it2++)
							{
								packet << it2->name << static_cast<int32_t>(it2->type);
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
				}*/
			}
		}
	}

	return 0;
}