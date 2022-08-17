#pragma once
#include "Position.h"

class Checkers : public Position
{
private:
	static std::vector<uint32_t> connections[32][4];

	static int field[8][8];
	static std::pair<int, int> cells[];
	uint32_t allies;
	uint32_t enemies;
	uint32_t kings;

public:
	bool beat = false;
	Checkers();
	Checkers(uint32_t allies, uint32_t enemies, uint32_t kings, int turn, Position* previous);

	//~Checkers();

	static void ConnectionsInit();

	bool enemy(int cell);
	bool allie(int cell);
	bool free(int cell);
	bool king(int cell);

	void fillBeats();
	bool continueBeats(int dir, uint32_t cell, std::vector<Position*>& next, Position* first);
	void fillMoves();

	void readBoard(std::istream& fin);

	void newGame()override;
	void FillNext() override;
	void Estimate() override;

	void read(std::istream& fin) override;
	void save(std::ostream& fout) override;
	void show() override;
};
