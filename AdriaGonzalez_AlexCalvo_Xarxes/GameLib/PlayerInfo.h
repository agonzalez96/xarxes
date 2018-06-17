#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <list>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>


#define NUM_PLAYERS 2

using namespace sf;
using namespace std;

int number = 0;

enum vote
{
	NO_VOTE, PAR, IMPAR
};

struct Player {
	string playername;
	TcpSocket* clientsocket;
	int points = 0;
	vote voted;
};
