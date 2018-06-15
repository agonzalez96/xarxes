#include <PlayerInfo.h>

//Client

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


//Si vale true --> nos permite marcar casilla con el mouse
//Si vale false --> No podemos interactuar con el tablero y aparece un letrero de "esperando"

bool tienesTurno = true;


//Ahora mismo no tiene efecto, pero luego lo necesitarás para validar los movimientos
//en función de si eres el gato o el ratón.

TipoProceso quienSoy = TipoProceso::RATON;




//Cuando el jugador clica en la pantalla, se nos da una coordenada del 0 al 512.
//Esta función la transforma a una posición entre el 0 y el 7

sf::Vector2f TransformaCoordenadaACasilla(int _x, int _y)
{
	float xCasilla = _x / LADO_CASILLA;
	float yCasilla = _y / LADO_CASILLA;
	sf::Vector2f casilla(xCasilla, yCasilla);
	return casilla;
}


//Si guardamos las posiciones de las piezas con valores del 0 al 7,
//esta función las transforma a posición de ventana (pixel), que va del 0 al 512

sf::Vector2f BoardToWindows(sf::Vector2f _position)
{
	return sf::Vector2f(_position.x*LADO_CASILLA + OFFSET_AVATAR, _position.y*LADO_CASILLA + OFFSET_AVATAR);
}

//Contiene el código SFML que captura el evento del clic del mouse y el código que pinta por pantalla
/*
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
posicionPlayer3 = BoardToWindows(posicionPlayer3);
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
*/


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

	sf::TcpSocket socket;
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

	string mensaje = " " + nickname + " > ";

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
					mensaje = " " + nickname + " > ";
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