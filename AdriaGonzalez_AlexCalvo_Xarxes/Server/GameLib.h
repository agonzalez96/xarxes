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

using namespace sf;
using namespace std;

int startMoney;
int startPos;

struct Property {
	int id;
	int price;
	vector<Vector2f> pos;
};

struct Player {
	int playerID;
	Vector2f pos;
	int money;
};
