#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#define MAX_MENSAJES 30
using namespace sf;
using namespace std;

void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	while (true) {
		char buffer[100];
		std::size_t bytesReceived;
		sf::Socket::Status status_r = socket->receive(buffer, sizeof(buffer), bytesReceived);

		if (status_r == Socket::NotReady) {

			//continue;
		}
		else if (status_r != sf::Socket::Done)
		{
		//	std::cout << "failed to receive data" << std::endl;
		}
		else if (status_r == sf::Socket::Disconnected) {
			//break;
		}
		else if (status_r == Socket::Done)
		{
			//FUNCIONA
			buffer[bytesReceived] = '\0';
			if (buffer == " >exit") {
				cout << "yolo";
			}
			aMensajes->push_back(buffer);
		}
	}
}

int main()
{
	char data[100];
	std::string textoAEnviar;
	sf::TcpSocket socket;
	std::vector<sf::TcpSocket*> aSockets;
	sf::TcpListener listener;
	//5000 es el puerto por el que escucha el servidor.
	//El cliente debe conocer la ip del servidor y el puerto por el que escucha.
	
	while (true)
	{
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


		std::vector<string> aMensajes;

		//THREAD RECEIVE seguir per aqui

		thread t1(&receiveData, &socket, &aMensajes);

	}
	//socket.disconnect();

	return 0;
}