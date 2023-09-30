#pragma once
#include <vector>
#include <fstream>
#include "matrix.h"


class Position
{
private:
	static bool compare(Position* p1, Position* p2);

public:
	~Position();

	bool filled = false;
	Position* previous = nullptr;
	std::vector<Position*> next;

	int turn = 1;
	double rating = 0;
	double reward = 0;

	void DeepEstimation(int depth, bool first = true);

	virtual void newGame() = 0;
	virtual void Estimate() = 0;
	virtual void FillNext() = 0;

	Position* makeMove(int t);
	Position* makeBestMove(int depth);
	void freeNext();
	void freePrevious();

	virtual void show();
	virtual void read(std::istream& fin);
	virtual void save(std::ostream& fout);

	virtual matrix vectorize();
};
