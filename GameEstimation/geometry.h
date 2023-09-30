#pragma once

class vec2 {
public:
	double x = 0;
	double y = 0;

	vec2();
	vec2(double x, double y);

	double l();
	double l2();

	vec2& operator =(const vec2& v);
	vec2& operator +=(const vec2& v);
	vec2& operator -=(const vec2& v);
	vec2& operator *=(const double t);
	vec2& operator /=(const double t);

	vec2 operator -();

	static vec2 UP;
	static vec2 DOWN;
	static vec2 LEFT;
	static vec2 RIGHT;
	static vec2 ZERO;
};

vec2 operator +(const vec2& v1, const vec2& v2);
vec2 operator -(const vec2& v1, const vec2& v2);
vec2 operator *(const double t, const vec2& v);
vec2 operator *(const vec2& v, const double t);
vec2 operator /(const vec2& v, const double t);
