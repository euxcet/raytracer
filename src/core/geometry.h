#ifndef COMMON_H
#define COMMON_H

#include "raytracer.h"

namespace Raytracer {
class Vector3 {
public:
	Vector3() { x = y = z = 0; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	explicit Vector3(const Normal3 &n);
	explicit Vector3(const Point3 &p);

	float operator[](int i) const {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	float &operator[](int i) {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	Vector3& operator=(const Vector3 &v) { x = v.x; y = v.y; z = v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
 	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
	Vector3& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }
	Vector3 operator-() const { return Vector3(-x, -y, -z); return *this; }
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }
	Vector3 operator/(float f) const { return Vector3(x / f, y / f, z / f); }
	bool operator==(const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator!=(const Vector3& v) const { return x != v.x || y != v.y || z != v.z; }


	float Length() const { return sqrt(x*x + y*y + z*z); }
	float SqrLength() const { return x*x + y*y + z*z; }

	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float c[3]; };
	};
};

inline std::ostream& operator<<(std::ostream &os, const Vector3 &v) {
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

inline Vector3 operator*(float f, const Vector3 &v) {
	return v * f;
}

class Point3 {
public:
	Point3() { x = y = z = 0; }
	Point3(float x, float y, float z) : x(x), y(y), z(z) {}

	float operator[](int i) const {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	float &operator[](int i) {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	Point3& operator=(const Point3 &p) { x = p.x; y = p.y; z = p.z; return *this; }
	Point3 operator+(const Vector3& v) const { return Point3(x + v.x, y + v.y, z + v.z); }
	Point3 operator+(const Point3& p) const { return Point3(x + p.x, y + p.y, z + p.z); }
	Vector3 operator-(const Point3& p) const { return Vector3(x - p.x, y - p.y, z - p.z); }
	Point3 operator-(const Vector3& v) const { return Point3(x - v.x, y - v.y, z - v.z); }
	Point3 operator*(float f) const { return Point3(x * f, y * f, z * f); }
	Point3 operator/(float f) const { return Point3(x / f, y / f, z / f); }
	Point3 operator-() { return Point3(-x, -y, -z); }

	Point3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Point3& operator+=(const Point3& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Point3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Point3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
	Point3& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }

	Point3 operator-() const {return Point3(-x, -y, -z); }

	bool operator==(const Point3& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator!=(const Point3& p) const { return x != p.x || y != p.y || z != p.z; }

	float x, y, z;
};

inline std::ostream& operator<<(std::ostream &os, const Point3 &p) {
	os << "[ " << p.x << ", " << p.y << ", " << p.z << " ]";
	return os;
}

class Normal3 {
public:
	Normal3() {}
	Normal3(float x, float y, float z) : x(x), y(y), z(z) {}
	explicit Normal3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}

	float operator[](int i) const {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	float &operator[](int i) {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	Normal3& operator=(const Normal3 &n) { x = n.x; y = n.y; z = n.z; return *this; }

	Normal3 operator+(const Normal3 &n) const { return Normal3(x + n.x, y + n.y, z + n.z); }
	Normal3 operator-(const Normal3 &n) const { return Normal3(x - n.x, y - n.y, z - n.z); }
	Normal3 operator*(float f) const { return Normal3(x * f, y * f, z * f); }
	Normal3 operator/(float f) const { return Normal3(x / f, y / f, z / f); }

	Normal3& operator+(const Normal3 &n) { x += n.x; y += n.y; z += n.z; return *this; }
	Normal3& operator-(const Normal3 &n) { x -= n.x; y -= n.y; z -= n.z; return *this; }
	Normal3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
	Normal3& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }


	float Length() const { return sqrt(x*x + y*y + z*z); }
	float SqrLength() const { return x*x + y*y + z*z; }

	float x, y, z;
};

inline std::ostream& operator<<(std::ostream &os, const Normal3 &n) {
	os << "[ " << n.x << ", " << n.y << ", " << n.z << " ]";
	return os;
}

class Ray {
public:
	Ray() : tmax(INF) {}
	Ray(const Point3& origin, const Vector3& direction,
		float tmax = INF)
		 : origin(origin), direction(direction), tmax(tmax) {}
	Point3 operator()(float t) const { return origin + t * direction; }
	//TODO Remove  SetOrigin and SetDirection
	void SetOrigin(const Point3& _origin) { origin = _origin; }
	void SetDirection(const Vector3& _direction) { direction = _direction; }
	Point3 GetOrigin() const { return origin; }
	Vector3 GetDirection() const { return direction; }

	float tmax;
	Point3 origin;
	Vector3 direction;
};

class aabb {
public:
	aabb() : pos(Vector3(0, 0, 0)), size(Vector3(0, 0, 0)) {}
	aabb(const Vector3 &_pos, const Vector3& _size) : pos(_pos), size(_size) {}
	Vector3 GetPos() const { return pos; }
	Vector3 GetSize() const { return size; }

	bool Intersect(const aabb& ab) {
		Vector3 v1 = ab.GetPos();
		Vector3 v2 = ab.GetPos() + ab.GetSize();
		Vector3 v3 = pos;
		Vector3 v4 = pos + size;
		return (v2.x > v3.x && v4.x > v1.x &&
				v2.y > v3.y && v4.y > v1.y &&
				v2.z > v3.z && v4.z > v1.z);
	}

	bool Contain(const Vector3& pos) {
		Vector3 v1 = pos;
		Vector3 v2 = pos + size;
		return (v1.x - EPS < pos.x && pos.x < v2.x + EPS &&
				v1.y - EPS < pos.y && pos.y < v2.y + EPS &&
				v1.z - EPS < pos.z && pos.z < v2.z + EPS);
	}
private:
	Vector3 pos;
	Vector3 size;
};
typedef Vector3 Color;


inline float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline float Dot(const Vector3& v1, const Normal3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline float Dot(const Normal3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline float Dot(const Normal3& v1, const Normal3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
                      (v1.x * v2.y) - (v1.y * v2.x));
}
inline Vector3 Cross(const Vector3& v1, const Normal3& v2) {
    return Vector3((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
                      (v1.x * v2.y) - (v1.y * v2.x));
}
inline Vector3 Cross(const Normal3& v1, const Vector3& v2) {
    return Vector3((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
                      (v1.x * v2.y) - (v1.y * v2.x));
}

inline Vector3 operator+(const Vector3& v, const Normal3& n) {
	return Vector3(v.x + n.x, v.y + n.y, v.z + n.z);
}

inline Vector3 operator+(const Normal3& n, const Vector3& v) {
	return Vector3(n.x + v.x, n.y + v.y, n.z + v.z);
}

inline Vector3 operator-(const Normal3& n, const Vector3& v) {
	return Vector3(n.x - v.x, n.y - v.y, n.z - v.z);
}
inline Vector3 operator-(const Vector3& v, const Normal3& n) {
	return Vector3(v.x - n.x, v.y - n.y, v.z - n.z);
}


inline Vector3::Vector3(const Point3& p) : x(p.x), y(p.y), z(p.z) {}
inline Vector3::Vector3(const Normal3 &n) : x(n.x), y(n.y), z(n.z) {}
inline Vector3 operator*(float f, Vector3& v) { return v * f; }
inline Vector3 Normalize(const Vector3& v) { return v / v.Length(); }

inline Point3 operator*(float f, const Point3& p) { return p * f; }
inline float Distance(const Point3& p1, const Point3& p2) { return (p1 - p2).Length(); }
inline Point3 Lerp(float t, const Point3& p1, const Point3& p2) {
	return (1 - t) * p1 + t * p2;
}

inline Normal3 operator*(float f, const Normal3& n) { return n * f; }
inline Normal3 Normalize(const Normal3& n) { return n / n.Length(); }

};

#endif
