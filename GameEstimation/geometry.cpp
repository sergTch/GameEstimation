#include "geometry.h"
#include <math.h>

vec2::vec2() { }

vec2::vec2(double x, double y): x(x), y(y) { }

double vec2::l()
{
	return sqrt(l2());
}

double vec2::l2()
{
	return x * x + y * y;
}

vec2& vec2::operator=(const vec2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

vec2& vec2::operator+=(const vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

vec2& vec2::operator-=(const vec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

vec2& vec2::operator*=(const double t)
{
	x *= t;
	y *= t;
	return *this;
}

vec2& vec2::operator/=(const double t)
{
	x /= t;
	y /= t;
	return *this;
}

vec2 vec2::operator-()
{
	return vec2(-x, -y);
}

vec2 operator+(const vec2& v1, const vec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator-(const vec2& v1, const vec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator*(double t, const vec2& v)
{
	return vec2(v.x * t, v.y * t);
}

vec2 operator*(const vec2& v, double t)
{
	return vec2(v.x * t, v.y * t);
}

vec2 operator/(const vec2& v, double t)
{
	return vec2(v.x / t, v.y / t);
}

vec2 vec2::UP		( 0,  1);
vec2 vec2::DOWN		( 0, -1);
vec2 vec2::LEFT		(-1,  0);
vec2 vec2::RIGHT	( 1,  0);
vec2 vec2::ZERO		( 0,  0);