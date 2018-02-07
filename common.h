#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string;

#include <iostream>
#include <fstream>
#include <cassert>
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;


namespace Raytracer {

#define EPS (1e-4)
#define BIGFLOAT (1e6)
#define TRACEDEPTH (6)

class vec3 {
public:
	vec3() : x(0.f), y(0.f), z(0.f) {}
	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	float Length() { return (float)sqrt(x*x + y*y + z*z); }
	float SqrLength() { return x*x + y*y + z*z; }
	float Dot(vec3 v) { return x * v.x + y * v.y + z * v.z; }
	vec3 Cross(vec3 v) {
		return vec3(y * v.z - z * v.y,
					z * v.x - x * v.z,
					x * v.y - y * v.x);
	}
	void Normalize() {
		float k = 1. / Length();
		x *= k; y *= k; z *= k;
	}
	void operator +=(const vec3& v) { x += v.x; y += v.y; z += v.z; }
	void operator +=(vec3* v) { x += v -> x; y += v -> y; z += v -> z; }
	void operator -=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
	void operator *=(float f) { x *= f; y *= f; z *= f; }
	void operator *=(const vec3& v) { x *= v.x; y *= v.y; z *= v.z; }
	void operator /=(float f) { x /= f; y /= f; z /= f; } // TODO : divide 0
	vec3 operator-() const { return vec3(-x, -y, -z); }
	friend vec3 operator + (const vec3& v1, const vec3& v2) {
		return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	friend vec3 operator - (const vec3& v1, const vec3& v2) {
		return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}
	friend vec3 operator * (const vec3& v, float f) {
		return vec3(v.x * f, v.y * f, v.z * f);
	}
	friend vec3 operator * (float f, const vec3& v) {
		return vec3(v.x * f, v.y * f, v.z * f);
	}
	friend vec3 operator * (const vec3& v1, const vec3& v2) {
		return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}
	friend vec3 operator / (const vec3& v, float f) {
		return vec3(v.x / f, v.y / f, v.z / f);
	}

	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float c[3]; };
	};
};

class Plane {
public:
	Plane() : N(0, 0, 0), D(0) {}
	Plane(const vec3 &_n, float _d) : N(_n), D(_d) {}

	/*
	union {
		struct {
			vec3 N;
			float D;
		};
		float c[4];
	};
	*/
	vec3 N;
	float D;

};

typedef vec3 Color;

};

#endif
