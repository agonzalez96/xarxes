#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#define MAX_MENSAJES 30

#define MAX 100
#define SIZE_TABLERO 64
#define SIZE_FILA_TABLERO 8
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5

#define SIZE_TABLERO 64
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5

using namespace sf;
using namespace std;

enum TipoProceso { RATON, GATO, PADRE };
char tablero[SIZE_TABLERO];

/**
* Si vale true --> nos permite marcar casilla con el mouse
* Si vale false --> No podemos interactuar con el tablero y aparece un letrero de "esperando"
*/
bool tienesTurno = true;

/**
* Ahora mismo no tiene efecto, pero luego lo necesitarás para validar los movimientos
* en función de si eres el gato o el ratón.
*/
TipoProceso quienSoy = TipoProceso::RATON;



/**
* Cuando el jugador clica en la pantalla, se nos da una coordenada del 0 al 512.
* Esta función la transforma a una posición entre el 0 y el 7
*/
sf::Vector2f TransformaCoordenadaACasilla(int _x, int _y)
{
	float xCasilla = _x / LADO_CASILLA;
	float yCasilla = _y / LADO_CASILLA;
	sf::Vector2f casilla(xCasilla, yCasilla);
	return casilla;
}

/**
* Si guardamos las posiciones de las piezas con valores del 0 al 7,
* esta función las transforma a posición de ventana (pixel), que va del 0 al 512
*/
sf::Vector2f BoardToWindows(sf::Vector2f _position)
{
	return sf::Vector2f(_position.x*LADO_CASILLA + OFFSET_AVATAR, _position.y*LADO_CASILLA + OFFSET_AVATAR);
}




/**
* Contiene el código SFML que captura el evento del clic del mouse y el código que pinta por pantalla
*/
void DibujaSFML()
{
	sf::Vector2f casillaOrigen, casillaDestino;
	bool casillaMarcada = false;

	sf::RenderWindow window(sf::VideoMode(640, 640), "Entipoly");

	Texture background;
	if (!background.loadFromFile("../res/Entipoly.png"))
	{
		cout << "Error" << endl;
	}
	Sprite backgroundSprite;
	backgroundSprite.setTexture(background);
	while (window.isOpen())
	{
		sf::Event event;

		//Este primer WHILE es para controlar los eventos del mouse
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left && tienesTurno)
				{
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;
					if (!casillaMarcada)
					{
						casillaOrigen = TransformaCoordenadaACasilla(x, y);
						casillaMarcada = true;
						//TODO: Comprobar que la casilla marcada coincide con las posición del raton (si le toca al ratón)
						//o con la posicion de alguna de las piezas del gato (si le toca al gato)

					}
					else
					{
						casillaDestino = TransformaCoordenadaACasilla(x, y);
						if (casillaOrigen.x == casillaDestino.x && casillaOrigen.y == casillaDestino.y)
						{
							casillaMarcada = false;
							//Si me vuelven a picar sobre la misma casilla, la desmarco
						}
						else
						{
							if (quienSoy == TipoProceso::RATON)
							{
								//TODO: Validar que el destino del ratón es correcto

								//TODO: Si es correcto, modificar la posición del ratón y enviar las posiciones al padre

							}
							else if (quienSoy == TipoProceso::GATO)
							{
								//TODO: Validar que el destino del gato es correcto

								//TODO: Si es correcto, modificar la posición de la pieza correspondiente del gato y enviar las posiciones al padre
							}
						}
					}
				}
				break;
			default:
				break;

			}
		}

		window.clear();
		window.draw(backgroundSprite);
		//A partir de aquí es para pintar por pantalla
		//Este FOR es para el tablero
		/*
		for (int i = 0; i<10; i++)
		{
		for (int j = 0; j<10; j++)
		{
		sf::RectangleShape rectBlanco(sf::Vector2f(LADO_CASILLA, LADO_CASILLA));
		rectBlanco.setFillColor(sf::Color::White);
		if (i % 2 == 0)
		{
		//Empieza por el blanco
		if (j % 2 == 0)
		{
		rectBlanco.setPosition(sf::Vector2f(i*LADO_CASILLA, j*LADO_CASILLA));
		window.draw(rectBlanco);
		}
		}
		else
		{
		//Empieza por el negro
		if (j % 2 == 1)
		{
		rectBlanco.setPosition(sf::Vector2f(i*LADO_CASILLA, j*LADO_CASILLA));
		window.draw(rectBlanco);
		}
		}
		}

		}
		*/
		Color red(255, 0, 0, 129);
		Color blue(255, 0, 0, 129);
		Color green(255, 0, 0, 129);
		Color yellow(255, 0, 0, 129);

		//TODO: Para pintar el circulito del ratón
		//Player1
		sf::CircleShape Player1(RADIO_AVATAR);
		Player1.setFillColor(red);
		sf::Vector2f posicionPlayer1(8.f, 8.f);
		posicionPlayer1 = BoardToWindows(posicionPlayer1);
		Player1.setPosition(posicionPlayer1);
		window.draw(Player1);
		//Player2
		sf::CircleShape Player2(RADIO_AVATAR);
		Player2.setFillColor(blue);
		sf::Vector2f posicionPlayer2(9.f, 8.f);
		posicionPlayer2 = BoardToWindows(posicionPlayer2);
		Player2.setPosition(posicionPlayer2);
		window.draw(Player2);
		//Player3
		sf::CircleShape Player3(RADIO_AVATAR);
		Player3.setFillColor(green);
		sf::Vector2f posicionPlayer3(8.f, 9.f);
		posicionPlayer2 = BoardToWindows(posicionPlayer3);
		Player3.setPosition(posicionPlayer3);
		window.draw(Player3);
		//Player4
		sf::CircleShape Player4(RADIO_AVATAR);
		Player4.setFillColor(yellow);
		sf::Vector2f posicionPlayer4(9.f, 9.f);
		posicionPlayer4 = BoardToWindows(posicionPlayer4);
		Player4.setPosition(posicionPlayer4);
		window.draw(Player4);



		if (!tienesTurno)
		{
			//Si no tengo el turno, pinto un letrerito de "Esperando..."
			sf::Font font;
			std::string pathFont = "liberation_sans/LiberationSans-Regular.ttf";
			if (!font.loadFromFile(pathFont))
			{
				std::cout << "No se pudo cargar la fuente" << std::endl;
			}


			sf::Text textEsperando("Esperando...", font);
			textEsperando.setPosition(sf::Vector2f(180, 200));
			textEsperando.setCharacterSize(30);
			textEsperando.setStyle(sf::Text::Bold);
			textEsperando.setFillColor(sf::Color::Green);
			window.draw(textEsperando);
		}
		else
		{
			//Si tengo el turno y hay una casilla marcada, la marco con un recuadro amarillo.
			if (casillaMarcada)
			{
				sf::RectangleShape rect(sf::Vector2f(LADO_CASILLA, LADO_CASILLA));
				rect.setPosition(sf::Vector2f(casillaOrigen.x*LADO_CASILLA, casillaOrigen.y*LADO_CASILLA));
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineThickness(5);
				rect.setOutlineColor(sf::Color::Yellow);
				window.draw(rect);
			}
		}

		window.display();
	}

}

struct Direction {
	string IP;
	int port;
};

void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	//TcpSocket* client = new TcpSocket;
	while (true) {

		sf::Packet infoPack;
		std::size_t bytesReceived;
		// Aqui pot rebre un packet amb la ip i el port
		// Cada packet crea un socket
		// Packet ip, port;
		sf::Socket::Status status_r = socket->receive(infoPack);
		if (status_r == Socket::NotReady) {
			continue;
		}
		else if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			// socket per cada nou client rebut

			//status = client.recive(ip)
			
			//client.connect(ip,50000,sf::milliseconds(5.f));


			string str;
			infoPack >> str;
			aMensajes->push_back(str);
			
		}
	}
} 


int main()
{
	cout << "Enter your nickname: ";
	string nickname;
	cin >> nickname;
	cout << endl;

	char connectionType, mode;
	char buffer[100];
	bool start = false;
	int socketNumber;
	std::size_t received;
	std::string text = "Connected to: ";
	std::vector<Direction> directionList;
	std::vector<sf::TcpSocket*> aSockets;
	vector <thread> Athreads;
	sf::TcpListener listener;
	Direction newDirection;
	Packet connectionInfo;
	//TcpSocket* clients = new TcpSocket;

	sf::Socket::Status status = listener.listen(51000);
	if (status != sf::Socket::Done)
	{
		//Aprovans per l'originalitat no?
		std::cout << "nope" << std::endl;
		int x = 1;
		status = listener.listen(51000 + x);
		x++;
	}


	if (!nickname.empty())
	{
		//CAS CLIENT
		// socket nou per client rebut
		sf::TcpSocket socketConnect;

		sf::Socket::Status status_c = socketConnect.connect("localhost", 50000, sf::milliseconds(5.f));
		if (status_c != sf::Socket::Done) {
			std::cout << "i couldn't connect to server" << std::endl;
		}
		else
		{

			//Packet nickPack;
			//nickPack << nickname;

			//Socket::Status nickInfo = socket.send(nickPack);
			//cout << "Me he conectado " << socket.getRemotePort() << endl;
		}
	}
	sf::TcpSocket* socket = new TcpSocket;
	socket->receive(connectionInfo);
	connectionInfo >> socketNumber;
	int a = 0;
	while (a < socketNumber) {
		socket = new TcpSocket;
		connectionInfo >> newDirection.IP >> newDirection.port;
		socket->connect(newDirection.IP, newDirection.port, sf::milliseconds(5.f));
		directionList.push_back(newDirection);
		listener.accept(*socket);
		aSockets.push_back(socket);
		a++;
	}

	for (int i = 0; i < directionList.size(); i++) {
		cout << "connection " << i << "\nip " << directionList[i].IP << "\nport " << directionList[i].port << endl;
	}

	DibujaSFML();

	char data[100];

	std::vector<string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat_Client");

	sf::Font font;
	if (!font.loadFromFile("Champagne & Limousines.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	string mensaje = " >";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text1(mensaje, font, 14);
	text1.setFillColor(sf::Color(0, 160, 0));
	text1.setStyle(sf::Text::Bold);
	text1.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);


	//THREAD RECEIVE seguir per aqui
	
	thread t1(&receiveData, socket, &aMensajes);

	//SocketSelector selector;
	//selector.add(listener);

	while (window.isOpen())
	{
		//if (selector.wait())
		//{
		//	if (selector.isReady(listener))
		//	{
		//		TcpSocket* client = new TcpSocket;
		//		if (listener.accept(*client) == Socket::Done)
		//		{
		//			cout << "new client from port: " << client->getRemotePort() << endl;
		//			aSockets.push_back(client);

		//			Packet name, login;
		//			status = client->receive(name);
		//			if (status != Socket::Disconnected)
		//			{
		//				string clientName;
		//				name >> clientName;

		//				string newLogin;
		//				newLogin = " >" + clientName + " has joined to the chat";


		//				login << newLogin;

		//				for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
		//				{
		//					TcpSocket& client2 = **it;

		//					client2.send(login);
		//				}
		//			}

		//			selector.add(*client);
		//		}
		//		else
		//		{
		//			delete client;
		//		}
		//	}
		//	else
		//	{
		//		for (vector<TcpSocket*>::iterator it1 = aSockets.begin(); it1 != aSockets.end(); ++it1)
		//		{
		//			TcpSocket& client = **it1;

		//			if (selector.isReady(client))
		//			{
		//				Packet packet;
		//				status = client.receive(packet);

		//				string messageComplete;
		//				string logOutKey = "logOut_";
		//				string userNameLogOut;

		//				if (status != Socket::Disconnected)
		//				{
		//					string logout;
		//					packet >> logout;

		//					bool key = false;

		//					for (int i = 0; i < logout.length(); i++)
		//					{

		//						messageComplete.push_back(logout[i]);
		//						if (logOutKey == messageComplete) {
		//							key = true;
		//						}

		//						if (key)
		//						{
		//							userNameLogOut.push_back(logout[i]);
		//						}
		//					}

		//					for (vector<TcpSocket*>::iterator it2 = aSockets.begin(); it2 != aSockets.end(); ++it2)
		//					{
		//						TcpSocket& client2 = **it2;

		//						if (it1 != it2) {
		//							client2.send(packet);
		//						}

		//					}
		//				}
		//				else if (status == Socket::Disconnected)
		//				{
		//					Packet  sendDisconnect;

		//					string newLogout;
		//					newLogout = "Another user has exit the chat";

		//					sendDisconnect << newLogout;

		//					for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
		//					{
		//						TcpSocket& client2 = **it;

		//						client2.send(sendDisconnect);
		//					}
		//					selector.remove(client);
		//				}
		//			}
		//		}
		//	}
		//}

		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					aMensajes.push_back(mensaje);

					//SEND DEL MESSAGE

					Packet packet;
					packet << mensaje;

					if (mensaje == ">exit" || mensaje == " >exit") {
						window.close();
						exit(0);
					}
					for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
					{
						TcpSocket& client2 = **it;

						Socket::Status status2 = client2.send(packet);

						if (status2 != Socket::Done)
						{
							cout << "Ha fallado el envio de datos" << endl;

						}

						if (aMensajes.size() > 25)
						{
							aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
						}
						mensaje = ">";
					}
					
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.length() > 0)
					mensaje.erase(mensaje.length() - 1, mensaje.length());
				break;
			}
		}
		window.draw(separator);


		//RECEIVE
		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text1.setString(mensaje_);
		window.draw(text1);
		window.display();
		window.clear();
	}
	if (!window.isOpen())
	{
		Packet sendLogout;
		sendLogout << "logOut_" + nickname;

		for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
		{
			TcpSocket& client2 = **it;

			Socket::Status logOutStatus = client2.send(sendLogout);
			if (logOutStatus != Socket::Done) {

				cout << "No sha enviat el packet" << endl;
			}
			else
			{
				cout << "Si que sha enviat" << endl;
			}
		}
	}
	socket->disconnect();

	t1.join(); 
	return 0;
}

