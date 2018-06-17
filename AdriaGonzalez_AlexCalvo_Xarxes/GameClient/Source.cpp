#include <PlayerInfo.h>

//Client

bool tienesTurno = true;



void receiveData(TcpSocket* socket, vector<string>* aMensajes) {
	while (true) {
		sf::Packet infoPack;
		string str;
		sf::Socket::Status status_r = socket->receive(infoPack);
		if (status_r == Socket::NotReady) {
			continue;
		}

		else if (status_r == sf::Socket::Disconnected) {
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

	cout << "Enter your nickname: ";
	string nickname;
	cin >> nickname;
	cout << endl;

	cout << nickname << ", welcome" << " to EntiPoly - The best Monopoly Edition ever made\n" << endl;
	cout << "Game instructions:\n\nThe game works with chat instructions, so when your turn starts type one of the commands below:\n" << endl;
	cout << "   throw dice --> This command allows you to throw a dice and move around the game\n" << endl;
	cout << "   exit --> This allows you to quit the game\n" << endl;
	cout << "The game will start when 4 people connect to the game, please wait...\n" << endl;;

	TcpSocket socket;
	char connectionType, mode;
	char buffer[100];
	std::size_t received;
	std::string text = "Connected to: ";
	int start = 0;



	if (!nickname.empty())
	{ 
		sf::Socket::Status status_c = socket.connect("localhost", 50000, sf::milliseconds(5.f));
		if (status_c != Socket::Done)
		{
			cout << "No se ha podido conectar" << endl;
		}
		else
		{
			Packet init, nickPack;
			char data[1];
			data[0] = 0;
			std::size_t received;

			nickPack << nickname;
			Socket::Status nickInfo = socket.send(nickPack);

			while (start != 1) {
				sf::Socket::Status status_r = socket.receive(data, 1, received);
				if (status_r == Socket::NotReady) {
					//cout << "not ready";
				}

				else if (status_r == sf::Socket::Disconnected) {
					//	cout << "disco";
				}
				else if (status_r == Socket::Done)
				{
					//cout << "suuh";
					start = data[0];
				}
				else if (status_r == Socket::Error) {
					//	cout << "error";
				}
			}
			cout << "Let's start!" << endl;
		}
	}

	char data[100];

	std::vector<string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window2;
	window2.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat_Client");

	sf::Font font;
	if (!font.loadFromFile("Champagne & Limousines.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	string mensaje = " " + nickname + " >";

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
	thread t1(&receiveData, &socket, &aMensajes);

	while (window2.isOpen())
	{
		//if (start) {
		sf::Event evento;
		while (window2.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window2.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape) {
					window2.close();
				}
				else if (evento.key.code == sf::Keyboard::Return)
				{
					aMensajes.push_back(mensaje);

					//SEND DEL MESSAGE

					Packet packet;
					packet << mensaje;

					if (mensaje == " " + nickname + ">exit" || mensaje == " " + nickname + " >exit") {
						std::cout << "Entra" << std::endl;
						window2.close();
						exit(0);
					}

					if (mensaje == " " + nickname + ">parell" || mensaje == " " + nickname + " >parell") {
						std::cout << "Entra" << std::endl;
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
					mensaje = " " + nickname + " >";
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
		window2.draw(separator);

		//RECEIVE
		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window2.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text1.setString(mensaje_);

		window2.draw(text1);
		window2.display();
		window2.clear();
		//}
	}
	if (!window2.isOpen())
	{
		Packet sendLogout;
		sendLogout << nickname + "has exited the game";

		Socket::Status logOutStatus = socket.send(sendLogout);
		if (logOutStatus != Socket::Done) {

			cout << "No sha enviat el packet" << endl;
		}
		else
		{
			cout << "Si que sha enviat" << endl;
		}
	}
	//socket.disconnect();
	t1.join();
	return 0;
}