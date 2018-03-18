#include "GameLib.h"

#define MAX_MENSAJES 30

int main()
{	

	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	sf::TcpListener listener;
	int _start = 0;
	char data[1];
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
			while (i < 3) {
			if (selector.isReady(listener))
			{
				TcpSocket* client = new TcpSocket;
					if (listener.accept(*client) == Socket::Done)
					{
						cout << "new client from port: " << client->getRemotePort() << endl;
						aSockets.push_back(client);
						i++;
						cout << i;

						for (vector<TcpSocket*>::iterator it2 = aSockets.begin(); it2 != aSockets.end(); ++it2)
						{
							TcpSocket& client2 = **it2;
							Packet start;
							if (i < 3) {
								data[0] = 0;
								client2.send(data, 1);
							}
							else {
								data[0] = 1;
								client2.send(data, 1);
								Packet startInfo;
								
							}
						}
						selector.add(*client);
					}
					else
					{
						delete client;
					}
					Packet name, login;
					status = client->receive(name);
					if (status != Socket::Disconnected)
					{
						string clientName;
						name >> clientName;

						string newLogin;
						//newLogin = " > "  name " has joined to the chat";


						login << newLogin;
						for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
						{
							TcpSocket& client2 = **it;
							client2.send(login);
						}
				}
				}
			}
			if (i == 3)
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
							selector.remove(client);
						}
					}
				}
			}
		}
	}
	
	return 0;
}


