#pragma once

#include "Position.h"
#include "network.h"
#include "geometry.h"

class Rocket : public Position
{
private:
	static double radius;
	static double force;
	static double resistance;
public:
	
	static network net;
	static void NetInit();

	Rocket();

	vec2 speed;
	vec2 target[2];

	void newGame();
	void Estimate();
	void FillNext();

	void save(std::ostream& fout);
	void read(std::istream& fin);
	void show();

	matrix vectorize();
};

