#include "Checkers.h"
#include <iostream>
#include <fstream>

Checkers::Checkers(uint32_t allies, uint32_t enemies, uint32_t kings, int turn, Position* previous) {
	this->allies = allies;
	this->enemies = enemies;
	this->kings = kings;
	this->turn = turn;
	this->previous = previous;
}

Checkers::Checkers() {
	allies = 0;
	enemies = 0;
	kings = 0;
	turn = 1;
}

//Checkers::~Checkers() {
//
//}

bool Checkers::enemy(int cell) {
	return (1 << cell) & enemies;
}

bool Checkers::allie(int cell) {
	return (1 << cell) & allies;
}

bool Checkers::free(int cell) {
	return !bool((1 << cell) & allies) && !bool((1 << cell) & enemies);
}

bool Checkers::king(int cell) {
	return (1 << cell) & kings;
}

uint32_t reverse(uint32_t k) {
	uint32_t a = 0;
	for (int i = 0; i < 32; i++) {
		if (k & (1 << i))
			a |= (1 << (31 - i));
	}
	return a;
}

void Checkers::Estimate() {
	rating = 0;
	uint32_t bit = 1;
	int k = 0;
	for (int cell = 0; cell < 32; cell++) {
		if (bit & enemies) {
			k++;
			if (bit & kings)
				rating -= 4;
			else rating--;
		} 
		else
			if (bit & allies) {
				k++;
				if (bit & kings)
					rating += 4;
				else rating++;
			}
		bit = bit << 1;
	}
	rating *= turn;
}

void Checkers::FillNext() {
	if (filled)
		return;
	freeNext();
	fillBeats();
	if (!beat)
		fillMoves();
	filled = true;
}

void Checkers::fillMoves() {
	for (int cell = 0; cell < 32; cell++) {
		if (allie(cell)) {
			if (!king(cell)) {
				int dir = 1 - turn;
				for (int i = 0; i < 2; i++) {
					if (connections[cell][dir].size() > 0 && free(connections[cell][dir][0]))
						if (connections[cell][dir][0] < 4 || connections[cell][dir][0] >= 28)
							next.push_back(new Checkers(enemies, (allies & ~(1 << cell)) | (1 << connections[cell][dir][0]), kings | (1 << connections[cell][dir][0]), -1 * turn, this));
						else next.push_back(new Checkers(enemies, (allies & ~(1 << cell)) | (1 << connections[cell][dir][0]), kings, -1 * turn, this));
					dir++;
				}
			}
			else {
				for (int dir = 0; dir < 4; dir++) {
					for (int t = 0; t < connections[cell][dir].size(); t++) {
						if (free(connections[cell][dir][t]))
							next.push_back(new Checkers(enemies, (allies & ~(1 << cell)) | (1 << connections[cell][dir][t]), (kings & ~(1 << cell)) | (1 << connections[cell][dir][t]), -1 * turn, this));
						else break;
					}
				}
			}
		}
	}
}

void Checkers::fillBeats() {
	Checkers buf;
	for (int cell = 0; cell < 32; cell++) {
		if (allie(cell)) {
			for (int dir = 0; dir < 4; dir++) {
				if (connections[cell][dir].size() > 1) {
					if (!king(cell)) {
						if (enemy(connections[cell][dir][0]) && free(connections[cell][dir][1])) {
							buf = Checkers(allies, enemies, kings, turn, this);
							buf.enemies &= ~(1 << connections[cell][dir][0]);
							buf.kings &= ~(1 << connections[cell][dir][0]);
							buf.allies &= ~(1 << cell);
							buf.allies |= (1 << connections[cell][dir][1]);
							if ((turn < 0 && connections[cell][dir][1] < 4) || (turn > 0 && connections[cell][dir][1] >= 28))
								buf.kings |= (1 << connections[cell][dir][1]);

							if (!buf.continueBeats(dir + 2, connections[cell][dir][1], next, this))
								next.push_back(new Checkers(buf.enemies, buf.allies, buf.kings, -buf.turn, this));
						}
					} else {
						int t = 0;
						int b = 0;
						bool finish = true;
						while (t < connections[cell][dir].size() && free(connections[cell][dir][t]))
							t++;
						b = t;
						if (t == connections[cell][dir].size() || !enemy(connections[cell][dir][t++]))
							continue;
						while (t < connections[cell][dir].size() && free(connections[cell][dir][t])) {
							buf = Checkers(allies, enemies, kings, turn, this);
							buf.enemies &= ~(1 << connections[cell][dir][b]);
							buf.kings &= ~(1 << connections[cell][dir][b]);
							buf.allies &= ~(1 << cell);
							buf.kings &= ~(1 << cell);
							buf.allies |= (1 << connections[cell][dir][t]);
							buf.kings |= (1 << connections[cell][dir][t]);
							finish &= !buf.continueBeats(dir + 2, connections[cell][dir][t], next, this);
							t++;
						}

						if (finish) {
							t = b + 1;
							while (t < connections[cell][dir].size() && free(connections[cell][dir][t])) {
								buf = Checkers(allies, enemies, kings, turn, this);
								buf.enemies &= ~(1 << connections[cell][dir][b]);
								buf.kings &= ~(1 << connections[cell][dir][b]);
								buf.allies &= ~(1 << cell);
								buf.kings &= ~(1 << cell);
								buf.allies |= (1 << connections[cell][dir][t]);
								buf.kings |= (1 << connections[cell][dir][t]);
								next.push_back(new Checkers(buf.enemies, buf.allies, buf.kings, -buf.turn, this));
								t++;
							}
						}
					}
				}
			}
		}
	}
	beat = next.size();
}

bool Checkers::continueBeats(int dir, uint32_t cell, std::vector<Position*>& next, Position* first) {
	Checkers buf;
	bool response = false;
	for (int k = 0; k < 3; k++) {
		dir = (dir + 1) % 4;
		
		if (connections[cell][dir].size() > 1) {
			if (!king(cell)) {
				if (enemy(connections[cell][dir][0]) && free(connections[cell][dir][1])) {
					response = true;
					buf = Checkers(allies, enemies, kings, turn, this);
					buf.enemies &= ~(1 << connections[cell][dir][0]);
					buf.kings &= ~(1 << connections[cell][dir][0]);
					buf.allies &= ~(1 << cell);
					buf.allies |= (1 << connections[cell][dir][1]);
					if ((turn < 0 && connections[cell][dir][1] < 4) || (turn > 0 && connections[cell][dir][1] >= 28))
						buf.kings |= (1 << connections[cell][dir][1]);

					if (!buf.continueBeats(dir + 2, connections[cell][dir][1], next, first))
						next.push_back(new Checkers(buf.enemies, buf.allies, buf.kings, -buf.turn, first));
				}
			}
			else {
				int t = 0;
				int b = 0;
				bool finish = true;
				while (t < connections[cell][dir].size() && free(connections[cell][dir][t]))
					t++;
				b = t;
				if (t == connections[cell][dir].size() || !enemy(connections[cell][dir][t++]))
					continue;
				while (t < connections[cell][dir].size() && free(connections[cell][dir][t])) {
					response = true;
					buf = Checkers(allies, enemies, kings, turn, this);
					buf.enemies &= ~(1 << connections[cell][dir][b]);
					buf.kings &= ~(1 << connections[cell][dir][b]);
					buf.allies &= ~(1 << cell);
					buf.kings &= ~(1 << cell);
					buf.allies |= (1 << connections[cell][dir][t]);
					buf.kings |= (1 << connections[cell][dir][t]);
					finish &= !buf.continueBeats(dir + 2, connections[cell][dir][t], next, first);
					t++;
				}

				if (finish) {
					t = b + 1;
					while (t < connections[cell][dir].size() && free(connections[cell][dir][t])) {
						response = true;
						buf = Checkers(allies, enemies, kings, turn, this);
						buf.enemies &= ~(1 << connections[cell][dir][b]);
						buf.kings &= ~(1 << connections[cell][dir][b]);
						buf.allies &= ~(1 << cell);
						buf.kings &= ~(1 << cell);
						buf.allies |= (1 << connections[cell][dir][t]);
						buf.kings |= (1 << connections[cell][dir][t]);
						next.push_back(new Checkers(buf.enemies, buf.allies, buf.kings, -buf.turn, first));
						t++;
					}
				}
			}
		}
	}
	return response;
}

void Checkers::newGame() {
	freeNext();
	kings = 0;
	uint32_t checker = 1;
	for (int i = 0; i < 32; i++) {
		if (i < 12)
			allies |= checker;
		if (i > 19)
			enemies |= checker;
		checker = checker << 1;
	}
}

void Checkers::read(std::istream& fin) {
	fin >> allies;
	fin >> enemies;
	fin >> kings;
	fin >> turn;
}

void Checkers::save(std::ostream& fout) {
	fout << allies << std::endl;
	fout << enemies << std::endl;
	fout << kings << std::endl;
	fout << turn << std::endl;
}

void Checkers::readBoard(std::istream& fin) {
	fin >> turn;

	uint32_t &white = turn > 0 ? allies : enemies;
	uint32_t &black = turn < 0 ? allies : enemies;
	char ch;
	std::string s = "";
	white = 0;
	black = 0;
	
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0){
				fin >> ch;
				s += ch;
				if (ch == 'X') {
					white |= (1 << field[i][j]);
					kings |= (1 << field[i][j]);
				}
				if (ch == 'x') {
					white |= (1 << field[i][j]);
				}
				if (ch == 'O') {
					black |= (1 << field[i][j]);
					kings |= (1 << field[i][j]);
				}
				if (ch == 'o') {
					black |= (1 << field[i][j]);
				}
			}
		}
	}
}

void Checkers::show() {
	system("cls");
	std::cout << turn << std::endl;
	uint32_t white = turn > 0 ? allies : enemies;
	uint32_t black = turn < 0 ? allies : enemies;
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			std::cout << " ";
			if ((i + j) % 2)
				std::cout << " ";
			else {
				if ((1 << field[i][j]) & white)
					if ((1 << field[i][j]) & kings)
						std::cout << "X";
					else std::cout << "x";
				else if ((1 << field[i][j]) & black)
					if ((1 << field[i][j]) & kings)
						std::cout << "O";
					else std::cout << "o";
				else std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}