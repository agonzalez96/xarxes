#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <list>
#define MAX_MENSAJES 30
using namespace sf;
using namespace std;

struct Direction {
	string IP;
	int port;
};

int main()
{	
	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	std::vector<Direction> directionList;
	sf::TcpListener listener;
	//5000 es el puerto por el que escucha el servidor.
	//El cliente debe conocer la ip del servidor y el puerto por el que escucha.
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Done)
	{
		//No se puede vincular al puerto 50000
		std::cout << "nope" << std::endl;
	}

	Direction direction;
	Packet _direction;
	TcpSocket* client = new TcpSocket;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < directionList.size(); j++) {
			direction.IP = directionList[j].IP;
			direction.port = directionList[j].port;
			_direction << direction.IP << direction.port;
			client->send(_direction);
		}
		direction.IP = client->getRemoteAddress().toString();
		direction.port = client->getRemotePort();
		directionList.push_back(direction);
	}	
	
	client->disconnect();
	exit(0);

	/*SocketSelector selector;
	selector.add(listener);
	
	while (true)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					cout << "new client from port: " << client->getRemotePort() << endl;
					aSockets.push_back(client);
	
					Packet name, login;
					status = client->receive(name);
					if (status != Socket::Disconnected)
					{
						string clientName;
						name >> clientName;
	
						string newLogin;
						newLogin = " >" + clientName + " has joined to the chat";
	
	
						login << newLogin;
	
						for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
						{
							TcpSocket& client2 = **it;
	
							client2.send(login);
						}
					}
	
					selector.add(*client);
				}
				else
				{
					delete client;
				}
			}
			else
			{
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
	
							bool key = false;
		
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
							Packet  sendDisconnect;
	
							string newLogout;
							newLogout = "Another user has exit the chat";
	
							sendDisconnect << newLogout;
	
							for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
							{
								TcpSocket& client2 = **it;
	
								client2.send(sendDisconnect);
							}
							selector.remove(client);
						}
					}
				}
			}
		}
	}
	*/
	return 0;
}


