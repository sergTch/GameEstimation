#include "Rocket.h"
#include <random>
#include <vector>
#include <math.h>
#include <iostream>

inline double randd(double a, double b) {
	return double(rand()) / RAND_MAX * (b - a) + b;
}

double Rocket::radius = 0.1;
double Rocket::resistance = 0.1;
double Rocket::force = 0.01;

network Rocket::net;

void Rocket::NetInit() {
	net.addLayer(6, 1, linearType);
	//net.addLayer(6, sigmType);
	//net.addLayer(1, linearType);
}

Rocket::Rocket()
{
	newGame();
}

void Rocket::newGame() {
	target[0] = vec2(randd(-1, 1), randd(1, 2));
	target[1] = target[0] + vec2(randd(-1, 1), randd(1,2));
	speed = vec2(0, 0);
}

std::vector<vec2> directions{ vec2::LEFT, vec2::UP, vec2::RIGHT, vec2::ZERO };
//std::vector<vec2> directions{ vec2(-1, 0.1), vec2::UP, vec2(1, 0.1) };

void Rocket::FillNext() {
	if (filled)
		return;
	freeNext();
	if (target[0].y < 0)
		return;

	Rocket* buf;

	for (int i = 0; i < directions.size(); i++) {
		buf = new Rocket();

		for (int j = 0; j < 2; j++)
			buf->target[j] = target[j];
		buf->previous = this;
		buf->speed = speed;
		buf->target[0] -= speed;
		buf->target[1] -= speed;
		buf->speed *= (1 - resistance);
		buf->speed = directions[i] * force;

		if (buf->target[0].l() < radius) {
			buf->target[0] = buf->target[1];
			reward += 200;
		}
		if (buf->target[0].y < 0) {
			buf->reward -= (target[0].y == target[1].y ? 100 : 1) * buf->target[0].l2();
		}
		if (abs(buf->target[0].x) > 5)
			buf->reward -= 100;
		//buf->reward -= 0.01 * target[1].y;
		next.push_back(buf);
	}
}

void Rocket::Estimate() {
	rating = net.feed(vectorize()).get(0, 0) + reward;
}

void Rocket::save(std::ostream& fout)
{
	fout << speed.x << ",";
	fout << speed.y << ",";
	fout << target[0].x << ",";
	fout << target[0].y << ",";
	fout << target[1].x << ",";
	fout << target[1].y << "\n";
}

void Rocket::read(std::istream& fin){}

void Rocket::show(){}

inline int sgn(double t) {
	return t >= 0 ? 1 : -1;
}

matrix Rocket::vectorize()
{
	int s = sgn(target[0].x);
	return std::vector<double>{s* target[0].x, target[0].y, 0 * s * target[1].x, 0 * target[1].y, 0, 0};// s* speed.x, speed.y};
}
