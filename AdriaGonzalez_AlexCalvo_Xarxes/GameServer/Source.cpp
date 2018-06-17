#include "PlayerInfo.h"

//Server

void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	while (true) {
		sf::Packet infoPack;
		string str;
		sf::Socket::Status status_r = socket->receive(infoPack);

		if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			for (int i = 0; i < NUM_PLAYERS; i++) {
				infoPack >> str;
				aMensajes->push_back(str);
			}

		}
	}
}


int main()
{
	srand(time(NULL));


	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	vector <Player> aPlayers;
	sf::TcpListener listener;
	int _start = 0;
	char data[1];
	bool setUp = false;
	bool num = false;
	Packet name, login;

	//getPort
	//get IP
	//5000 es el puerto por el que escucha el servidor.
	//El cliente debe conocer la ip del servidor y el puerto por el que escucha.
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Done)
	{
		//No se puede vincular al puerto 50000
		std::cout << "nope" << std::endl;

	}


	SocketSelector selector;
	selector.add(listener);
	int i = 0;

	while (true)
	{
		if (selector.wait())
		{
			while (i < NUM_PLAYERS) {
				if (selector.isReady(listener))
				{
					TcpSocket* client = new TcpSocket;

					if (listener.accept(*client) == Socket::Done)
					{
						cout << "new client from port: " << client->getRemotePort() << endl;
						aSockets.push_back(client);
						
						Player player;
						player.clientsocket = client;

						i++;
						cout << i;

						status = client->receive(name);
						if (status != Socket::Disconnected)
						{
							string clientName;
							name >> clientName;

							string newLogin;
							newLogin = " > " + clientName + " has joined to the chat\n";

							player.playername = clientName;
							aPlayers.push_back(player);
						

							cout << newLogin;

							login << newLogin;
							
							for (vector<Player>::iterator it2 = aPlayers.begin(); it2 != aPlayers.end(); ++it2)
							{
								TcpSocket& client2 = *it2->clientsocket;
								Packet start;
								if (i < NUM_PLAYERS) {
									data[0] = 0;
									client2.send(data, 1);
								}
								else {
									data[0] = 1;
									client2.send(data, 1);
								}
							}
							selector.add(*client);
						}
						else {
							delete client;
						}

					}
				}

			}
			if (i == NUM_PLAYERS)
			{
				if (!setUp) {
					string init, init2;
					Packet temp;

					for (vector<Player>::iterator it = aPlayers.begin(); it != aPlayers.end(); ++it)
					{
						TcpSocket& client2 = *it->clientsocket;
						//temp << 0;
						for (int j = 0; j < NUM_PLAYERS; j++) {
							login >> init;
							init2 += init;
							//	cout << init2;
							temp << init;
						}
						client2.send(temp);
					}
					setUp = true;
				}


				for (vector<Player>::iterator it1 = aPlayers.begin(); it1 != aPlayers.end(); ++it1)
				{
					TcpSocket& client = *it1->clientsocket;

					if (selector.isReady(client))
					{
						Packet packet;
						status = client.receive(packet);

						string messageComplete;
						string logOutKey = "logOut_";
						string userNameLogOut;

						if (status != Socket::Disconnected)
						{
							string logout;
							packet >> logout;
							string nickname;
							nickname = it1->playername;
							//Turnos (Not finished)


							bool key = false;
							cout << logout;
							//per cada jugador escull parell o imparell//
							if (it1->voted = NO_VOTE) {
								if (logout == " " + nickname + ">parell") {
									it1->voted = PAR;

								}

								if (logout == " " + nickname + " >imparell") {
									it1->voted = IMPAR;
								}
							}
							else {
								//un cop ha votat(fer que sigui que han votat tots);

								// per cada jugador diu el numero que vol apostar de 1 a 5//

								if (logout == " " + nickname + " >1") {
									number += 1;
									num = true;
								}

								if (logout == " " + nickname + " >2") {
									number += 2;
									num = true;
								}

								if (logout == " " + nickname + " >3") {
									number += 3;
									num = true;
								}

								if (logout == " " + nickname + " >4") {
									number += 4;
									num = true;
								}

								if (logout == " " + nickname + " >5") {
									number += 5;
									num = true;
								}
							}
							//quan tots hagin votat i dit numero s'hauria de comprovar i sumar un punt, al arribar a 3 s'acaba la partida 
							for (int i = 0; i < logout.length(); i++)
							{

								messageComplete.push_back(logout[i]);
								if (logOutKey == messageComplete) {
									key = true;
								}

								if (key)
								{
									userNameLogOut.push_back(logout[i]);
								}

							}

							for (vector<Player>::iterator it2 = aPlayers.begin(); it2 != aPlayers.end(); ++it2)
							{
								TcpSocket& client2 = *it2->clientsocket;
								//si num es true envia a la resta unicament que ha votat
								if (it1 != it2) {
									if (!num) {
										client2.send(packet);
									}
									else {
										Packet votedpack;
										votedpack << it1->playername + " is done ";
										client2.send(votedpack);
										num = false;
									}
								}

							}
						}
						else if (status == Socket::Disconnected)
						{
							selector.remove(client);
							client.disconnect();
						}
					}
				}
			}
		}
	}

	return 0;
}
