#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <ctime>
#include <time.h>
#include <queue>
#include <string>
#include "Checkers.h"
#include "network.h"
#include "Rocket.h"
#include "RLAgent.h"

void runBoard(int depth) {
	int t;
	Checkers checkers;
	checkers.newGame();
	Position* game = new Checkers(checkers);
	Position* start = game;
	while (true) {
		game->show();
		std::cin >> t;

		if (t == -2) {
			game = game->makeBestMove(depth);
			continue;
		}
		game->filled = false;
		game->FillNext();
		game = game->makeMove(t);
	}
	delete start;
}

void readAndMove(int depth) {
	Checkers game;
	game.newGame();
	game.readBoard(std::cin);
	game = *dynamic_cast<Checkers *>(game.makeBestMove(depth));
	game.show();
}

int main()
{
	std::cout << std::fixed << std::setprecision(2);
	std::string file = "positions.txt";
	int depth = 100000000;
	Checkers::ConnectionsInit();
	Rocket::NetInit();

	clock_t tStart = clock();
	std::srand(unsigned(std::time(0)));

	//runBoard(depth);
	////readAndMove(depth);

	Qlearn qagent;

	qagent.nn = &Rocket::net;
	qagent.pos = new Rocket();

	//Rocket::net.load("rocket");
	qagent.train(10000000000, 1);
	//Rocket::net.save("rocket");

	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	return 0;
}