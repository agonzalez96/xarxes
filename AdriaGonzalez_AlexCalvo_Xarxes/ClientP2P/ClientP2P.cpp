#include <PlayerInfo.h>

#define MAX_MENSAJES 30

using namespace sf;
using namespace std;


struct Direction {
	string IP;
	int port;
};

bool tienesTurno = true;

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

	while (window2.isOpen())
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
				if (evento.key.code == sf::Keyboard::Escape)
					window2.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					aMensajes.push_back(mensaje);

					//SEND DEL MESSAGE

					Packet packet;
					packet << mensaje;

					if (mensaje == ">exit" || mensaje == " >exit") {
						window2.close();
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
		//////////////////////

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
			window2.draw(textEsperando);
		}
		else
		{
			//Si tengo el turno y hay una casilla marcada, la marco con un recuadro amarillo.
			/*if (casillaMarcada)
			{
			sf::RectangleShape rect(sf::Vector2f(LADO_CASILLA, LADO_CASILLA));
			rect.setPosition(sf::Vector2f(casillaOrigen.x*LADO_CASILLA, casillaOrigen.y*LADO_CASILLA));
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineThickness(5);
			rect.setOutlineColor(sf::Color::Yellow);
			window.draw(rect);
			}*/
		}

		window2.draw(text1);
		window2.display();
		window2.clear();
		//}
	}
	if (!window2.isOpen())
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
	////////////////////////////////////////////
	//char data[100];

	//std::vector<string> aMensajes;

	//sf::Vector2i screenDimensions(800, 600);

	//sf::RenderWindow window;
	//window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat_Client");

	//sf::Font font;
	//if (!font.loadFromFile("Champagne & Limousines.ttf"))
	//{
	//	std::cout << "Can't load the font file" << std::endl;
	//}

	//string mensaje = " >";

	//sf::Text chattingText(mensaje, font, 14);
	//chattingText.setFillColor(sf::Color(0, 160, 0));
	//chattingText.setStyle(sf::Text::Bold);


	//sf::Text text1(mensaje, font, 14);
	//text1.setFillColor(sf::Color(0, 160, 0));
	//text1.setStyle(sf::Text::Bold);
	//text1.setPosition(0, 560);

	//sf::RectangleShape separator(sf::Vector2f(800, 5));
	//separator.setFillColor(sf::Color(200, 200, 200, 255));
	//separator.setPosition(0, 550);


	////THREAD RECEIVE seguir per aqui
	//
	//thread t1(&receiveData, socket, &aMensajes);

	////SocketSelector selector;
	////selector.add(listener);

	//while (window.isOpen())
	//{
	//	//if (selector.wait())
	//	//{
	//	//	if (selector.isReady(listener))
	//	//	{
	//	//		TcpSocket* client = new TcpSocket;
	//	//		if (listener.accept(*client) == Socket::Done)
	//	//		{
	//	//			cout << "new client from port: " << client->getRemotePort() << endl;
	//	//			aSockets.push_back(client);

	//	//			Packet name, login;
	//	//			status = client->receive(name);
	//	//			if (status != Socket::Disconnected)
	//	//			{
	//	//				string clientName;
	//	//				name >> clientName;

	//	//				string newLogin;
	//	//				newLogin = " >" + clientName + " has joined to the chat";


	//	//				login << newLogin;

	//	//				for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
	//	//				{
	//	//					TcpSocket& client2 = **it;

	//	//					client2.send(login);
	//	//				}
	//	//			}

	//	//			selector.add(*client);
	//	//		}
	//	//		else
	//	//		{
	//	//			delete client;
	//	//		}
	//	//	}
	//	//	else
	//	//	{
	//	//		for (vector<TcpSocket*>::iterator it1 = aSockets.begin(); it1 != aSockets.end(); ++it1)
	//	//		{
	//	//			TcpSocket& client = **it1;

	//	//			if (selector.isReady(client))
	//	//			{
	//	//				Packet packet;
	//	//				status = client.receive(packet);

	//	//				string messageComplete;
	//	//				string logOutKey = "logOut_";
	//	//				string userNameLogOut;

	//	//				if (status != Socket::Disconnected)
	//	//				{
	//	//					string logout;
	//	//					packet >> logout;

	//	//					bool key = false;

	//	//					for (int i = 0; i < logout.length(); i++)
	//	//					{

	//	//						messageComplete.push_back(logout[i]);
	//	//						if (logOutKey == messageComplete) {
	//	//							key = true;
	//	//						}

	//	//						if (key)
	//	//						{
	//	//							userNameLogOut.push_back(logout[i]);
	//	//						}
	//	//					}

	//	//					for (vector<TcpSocket*>::iterator it2 = aSockets.begin(); it2 != aSockets.end(); ++it2)
	//	//					{
	//	//						TcpSocket& client2 = **it2;

	//	//						if (it1 != it2) {
	//	//							client2.send(packet);
	//	//						}

	//	//					}
	//	//				}
	//	//				else if (status == Socket::Disconnected)
	//	//				{
	//	//					Packet  sendDisconnect;

	//	//					string newLogout;
	//	//					newLogout = "Another user has exit the chat";

	//	//					sendDisconnect << newLogout;

	//	//					for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
	//	//					{
	//	//						TcpSocket& client2 = **it;

	//	//						client2.send(sendDisconnect);
	//	//					}
	//	//					selector.remove(client);
	//	//				}
	//	//			}
	//	//		}
	//	//	}
	//	//}

	//	sf::Event evento;
	//	while (window.pollEvent(evento))
	//	{
	//		switch (evento.type)
	//		{
	//		case sf::Event::Closed:
	//			window.close();
	//			break;
	//		case sf::Event::KeyPressed:
	//			if (evento.key.code == sf::Keyboard::Escape)
	//				window.close();
	//			else if (evento.key.code == sf::Keyboard::Return)
	//			{
	//				aMensajes.push_back(mensaje);

	//				//SEND DEL MESSAGE

	//				Packet packet;
	//				packet << mensaje;

	//				if (mensaje == ">exit" || mensaje == " >exit") {
	//					window.close();
	//					exit(0);
	//				}
	//				for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
	//				{
	//					TcpSocket& client2 = **it;

	//					Socket::Status status2 = client2.send(packet);

	//					if (status2 != Socket::Done)
	//					{
	//						cout << "Ha fallado el envio de datos" << endl;

	//					}

	//					if (aMensajes.size() > 25)
	//					{
	//						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
	//					}
	//					mensaje = ">";
	//				}
	//				
	//			}
	//			break;
	//		case sf::Event::TextEntered:
	//			if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
	//				mensaje += (char)evento.text.unicode;
	//			else if (evento.text.unicode == 8 && mensaje.length() > 0)
	//				mensaje.erase(mensaje.length() - 1, mensaje.length());
	//			break;
	//		}
	//	}
	//	window.draw(separator);


	//	//RECEIVE
	//	for (size_t i = 0; i < aMensajes.size(); i++)
	//	{
	//		std::string chatting = aMensajes[i];
	//		chattingText.setPosition(sf::Vector2f(0, 20 * i));
	//		chattingText.setString(chatting);
	//		window.draw(chattingText);
	//	}
	//	std::string mensaje_ = mensaje + "_";
	//	text1.setString(mensaje_);
	//	window.draw(text1);
	//	window.display();
	//	window.clear();
	//}
	//if (!window.isOpen())
	//{
	//	Packet sendLogout;
	//	sendLogout << "logOut_" + nickname;

	//	for (vector<TcpSocket*>::iterator it = aSockets.begin(); it != aSockets.end(); ++it)
	//	{
	//		TcpSocket& client2 = **it;

	//		Socket::Status logOutStatus = client2.send(sendLogout);
	//		if (logOutStatus != Socket::Done) {

	//			cout << "No sha enviat el packet" << endl;
	//		}
	//		else
	//		{
	//			cout << "Si que sha enviat" << endl;
	//		}
	//	}
	//}
	//socket->disconnect();

	//t1.join(); 
	//
	return 0;
}