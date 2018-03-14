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


void receiveData(TcpSocket* socket, vector<string>* aMensajes, bool start) {
	while (true) {
		sf::Packet infoPack;
		sf::Socket::Status status_r = socket->receive(infoPack);
		if (status_r == Socket::NotReady) {

			continue;
		}
	
		else if (status_r == sf::Socket::Disconnected) {
			break;
		}
		else if (status_r == Socket::Done)
		{
			string str;
			int type;
			infoPack >> type;
			switch (type)
			{
			case 0:
				infoPack >> start;
				break;
			case 1:
				infoPack >> str;
				aMensajes->push_back(str);
				break;

			default:
				break;
			}
			
		}
	}
}
int main()
{
	cout << "Enter your nickname: ";
	string nickname;
	cin >> nickname;
	cout << endl;

	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[100];
	std::size_t received;
	std::string text = "Connected to: ";
	bool start = false;


	if (!nickname.empty())
	{
		//CAS CLIENT
		sf::Socket::Status status_c = socket.connect("localhost", 50000, sf::milliseconds(5.f));
		if (status_c != sf::Socket::Done) {
			std::cout << "i couldn't connect to server" << std::endl;
		}
		else {
			std::cout << "connected to server" << std::endl;
		}

		if (status_c != Socket::Done)
		{
			cout << "No se ha podido conectar" << endl;
		}
		else
		{

			Packet nickPack;
			nickPack << nickname;

			Socket::Status nickInfo = socket.send(nickPack);
			//cout << "Me he conectado " << socket.getRemotePort() << endl;
		}
	}

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

	thread t1(&receiveData, &socket, &aMensajes, &start);


	while (window.isOpen())
	{
		if (start) {
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

						Socket::Status status2 = socket.send(packet);

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
	}
	if (!window.isOpen())
	{
		Packet sendLogout;
		sendLogout << "logOut_" + nickname;

		Socket::Status logOutStatus = socket.send(sendLogout);
		if (logOutStatus != Socket::Done) {

			cout << "No sha enviat el packet" << endl;
		}
		else
		{
			cout << "Si que sha enviat" << endl;
		}
	}
	socket.disconnect();

	t1.join();

	return 0;
}

