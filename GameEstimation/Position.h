#pragma once
#include <vector>
#include <fstream>

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

	void DeepEstimation(int depth, bool first);

	virtual void newGame();
	virtual void Estimate();
	virtual void FillNext();

	Position* makeMove(int t);
	Position* makeBestMove(int depth);
	void freeNext();

	virtual void show();
	virtual void read(std::istream& fin);
	virtual void save(std::ostream& fout);
};
