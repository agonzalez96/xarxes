#include "GameLib.h"

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

void ThrowDice(Player player, int dice) {
	dice = rand() % 6 + 1;
	player.pos += dice;
	if (player.pos >= 15) {
		player.pos = player.pos - 15;
	}
}

int main()
{	
	int dice;
	srand(time(NULL));
	Player player1{ 0,0,0 };
	Player player2{ 0,0,0 };
	Player player3{ 0,0,0 };
	Player player4{ 0,0,0 };

	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	sf::TcpListener listener;
	int _start = 0;
	char data[1];
	bool setUp = false;
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
						i++;
						cout << i;

						status = client->receive(name);
						if (status != Socket::Disconnected)
						{
							string clientName;
							name >> clientName;

							string newLogin;
							newLogin = " > " + clientName + " has joined to the chat\n";

							cout << newLogin;

							login << newLogin;

						for (vector<TcpSocket*>::iterator it2 = aSockets.begin(); it2 != aSockets.end(); ++it2)
						{
							TcpSocket& client2 = **it2;
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
						else{
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

					for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
					{
						TcpSocket& client2 = **it;
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
				

				for (vector<TcpSocket*>::iterator it1 = aSockets.begin(); it1 != aSockets.end(); ++it1)
				{
					TcpSocket& client = **it1;
	
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
	
							int player = 0;
							int action = 0;
							//Turnos (Not finished)
							/*switch (player)
							{
							case 1:
								switch (action)
								{
								case 1:
									ThrowDice(player1, dice);
									break;

								case 2:

									break;

								default:
									break;
								}
								break;
							case 2:
								switch (action)
								{
								case 1:
									ThrowDice(player1, dice);
									break;

								case 2:

									break;

								default:
									break;
								}
								break;
							case 3:
								switch (action)
								{
								case 1:
									ThrowDice(player1, dice);
									break;

								case 2:

									break;

								default:
									break;
								}
								break;
							case 4:
								switch (action)
								{
								case 1:
									ThrowDice(player1, dice);
									break;

								case 2:

									break;

								default:
									break;
								}
								break;
							default:
								break;
							}*/

							bool key = false;
							cout << logout;

							if (logout == " > throwdice") {
								Packet action;
								ThrowDice(player1, dice);
								action << "7" + 1;
							}
		
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
		
							for (vector<TcpSocket*>::iterator it2 = aSockets.begin(); it2 != aSockets.end(); ++it2)
							{
								TcpSocket& client2 = **it2;
	
								if (it1 != it2) {
									client2.send(packet);
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


