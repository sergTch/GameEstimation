#include "Position.h"
#include <algorithm>
#include <iostream>

Position::~Position() {
	for (int i = 0; i < next.size(); i++)
		delete next[i];
}

bool Position::compare(Position* p1, Position *p2) {
	return p1->rating * p1->turn > p2->rating * p2->turn;
}

void Position::DeepEstimation(int depth, bool first) {
	rating = turn * -1000;
	FillNext();

	if (depth < next.size()) {
		for (int i = 0; i < next.size(); i++) {
			next[i]->Estimate();
			rating = turn * std::max(turn * rating, turn * next[i]->rating);
		}
		return;
	} 
	
	for (int i = 0; i < next.size(); i++) {
		next[i]->FillNext();
		next[i]->Estimate();
	}

	std::random_shuffle(next.begin(), next.end());
	std::sort(next.begin(), next.end(), compare);

	for (int i = 0; i < next.size(); i++) {
		next[i]->DeepEstimation(depth/next.size(), false);
		rating = turn * std::max(turn * rating, turn * next[i]->rating);

		if (turn * previous-> turn == -1 && !first && rating * turn >= previous->rating * turn)
			return;
	}
}

Position* Position::makeMove(int t) {
	if (t < 0 || t >= next.size()) {
		filled = false;
		for (int i = 0; i < next.size(); i++)
			delete next[i];
		next = std::vector<Position*>(0);
		return previous;
	}
	else {
		for (int i = 0; i < next.size(); i++)
			if (i != t)
				delete next[i];

		filled = false;

		next = std::vector<Position*>(1, next[t]);
		return next[0];
	}
}

Position* Position::makeBestMove(int depth) {
	DeepEstimation(depth, true);
	for (int i = 0; i < next.size(); i++)
		if (rating == next[i]->rating)
			return makeMove(i);
}

void Position::show() { }

void Position::save(std::ostream& fout) { }

void Position::read(std::istream& fin) { }

void Position::freeNext() {
	if (next.size() != 0) {
		for (int i = 0; i < next.size(); i++)
			delete next[i];
		next = std::vector<Position*>(0);
	}
}

void Position::freePrevious()
{
	if (previous) {
		previous->next = std::vector<Position*>(0);
		Position* root = this;

		while (root->previous)
			root = root->previous;
		delete root;
		previous = nullptr;
	}
}

matrix Position::vectorize() {
	return 0;
}
