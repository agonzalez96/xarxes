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

void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	while (true) {

		char buffer[100];
		std::size_t bytesReceived;
		sf::Socket::Status status_r = socket->receive(buffer, sizeof(buffer), bytesReceived);

		if (status_r == Socket::NotReady) {

			continue;
		}

		else if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			//FUNCIONA
			buffer[bytesReceived] = '\0';
			aMensajes->push_back(buffer);
		}
	}
}

int main()
{
	cout << "-------------- SERVER CHAT -------------" << endl;
	std::cout << "Waiting for users to connect the chat..." << endl;

	char data[100];
	std::string textoAEnviar;
	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	sf::TcpListener listener;

	//5000 es el puerto por el que escucha el servidor.
	//El cliente debe conocer la ip del servidor y el puerto por el que escucha.
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Done)
	{
		//No se puede vincular al puerto 50000
		std::cout << "nope" << std::endl;
	}
	else {
		listener.accept(socket);
		aSockets.push_back(&socket);
		std::string texto = "Conexion con ... " + (socket.getRemoteAddress()).toString() + ":" + std::to_string(socket.getRemotePort()) + "\n";
		std::cout << texto;
		//listener.close();
	}

	
	while (true)
	{
		std::vector<string> aMensajes;

		//THREAD RECEIVE seguir per aqui

		thread t1(&receiveData, &socket, &aMensajes);

	}
	socket.disconnect();

	return 0;
}

void ControlServidor() {

	bool running = true;

	TcpListener listener;
	Socket::Status status = listener.listen(50000);
	if (status != Socket::Done)
	{
		cout << "Error al abrir listener" << endl;
		exit(0);
	}

	list<TcpSocket*> clients;

	SocketSelector selector;
	selector.add(listener);

	while (running)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					cout << "Llega el cliente por el puerto: " << client->getRemotePort() << endl;
					clients.push_back(client);

					Packet nick, sendLogin;
					status = client->receive(nick);
					if (status != Socket::Disconnected)
					{
						//Algu s'ha connectat al xat
						string nickName;
						nick >> nickName;

						string newLogin;
						newLogin = " " + nickName + " has joined to the chat";


						sendLogin << newLogin;
						//Socket::Status loginStatus = client->send(sendLogin);

						for (list<TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
						{
							TcpSocket& client2 = **it;

							client2.send(sendLogin);
						}
						/*if (loginStatus != Socket::Done) {
						cout << "no s'esta enviant amics" << endl;
						}*/
						cout << "Sha conectat " << nickName << " al xat" << endl;
					}

					selector.add(*client);
				}
				else
				{
					cout << "Error al recoger conexion nueva " << endl;
					delete client;
				}
			}
			else
			{
				std::cout << "Waiting for users to connect the chat..." << endl;
				for (list<TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					TcpSocket& client = **it;

					if (selector.isReady(client))
					{
						Packet packet;
						status = client.receive(packet);

						string messageComplete;
						string logOutKey = "logOut_";
						string userNameLogOut;

						if (status != Socket::Disconnected)
						{
							//Chat packet
							string strRec;
							packet >> strRec;
							cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << endl;


							bool keyWord = false;



							for (int i = 0; i < strRec.length(); i++)
							{

								messageComplete.push_back(strRec[i]);
								if (logOutKey == messageComplete) {
									keyWord = true;
									//cout << "Algu s'ha desconectat" << endl;
								}

								if (keyWord)
								{
									userNameLogOut.push_back(strRec[i]);
								}
							}
							cout << endl;
							cout << userNameLogOut << endl;



							for (list<TcpSocket*>::iterator it2 = clients.begin(); it2 != clients.end(); ++it2)
							{
								TcpSocket& client2 = **it2;

								if (it != it2) {

									client2.send(packet);

								}

							}
						}
						else if (status == Socket::Disconnected)
						{
							Packet nickLogOut, sendLogout;

							Socket::Status statusLogOut = client.receive(nickLogOut);

							nickLogOut << userNameLogOut;
							if (statusLogOut != Socket::Done)
							{
								//cout << " No he rebut un puto packet" << endl;
							}
							//cout << " " + userNameLogOut + " has exit the chat" << endl;

							string newLogout;
							newLogout = " " + userNameLogOut + " Someone has exit the chat";


							sendLogout << newLogout;


							for (list<TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
							{
								TcpSocket& client2 = **it;

								client2.send(sendLogout);
							}
							selector.remove(client);

							//cout << "Elimino el socket que se ha desconectado" << endl;
							//cout << newLogout << endl;
						}
						else
						{
							cout << "Error al recibir de " << client.getRemotePort() << endl;

						}
					}
				}
			}
		}
	}
}

int main()
{

	cout << "-------------- SERVER CHAT -------------" << endl;
	std::cout << "Waiting for users to connect the chat..." << endl;

	ControlServidor();


	return 0;
}




