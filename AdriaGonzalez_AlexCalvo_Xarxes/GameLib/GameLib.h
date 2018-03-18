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

int startMoney;
int startPos;

struct Property {
	int id;
	int price;
	float posx;
	float posy;
};

struct Player {
	int playerID;
	int pos;
	int money;
};

vector<Property> caselles = {
	{ 1,100,8.0f,8.0f },
{ 2,100,6.0f,8.0f },
{ 3,100,4.0f,8.0f },
{ 4,100,2.0f,8.0f },
{ 5,100,0.0f,8.0f },
{ 6,100,0.0f,6.0f },
{ 7,100,0.0f,4.0f },
{ 8,100,0.0f,2.0f },
{ 9,100,0.0f,0.0f },
{ 10,100,2.0f,0.0f },
{ 11,100,4.0f,0.0f },
{ 12,100,6.0f,0.0f },
{ 13,100,8.0f,0.0f },
{ 14,100,8.0f,2.0f },
{ 15,100,8.0f,4.0f },
{ 16,100,8.0f,6.0f }
};
