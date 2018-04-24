#ifndef GEOMETRY_H
#define GEOMETRY_H

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


	float Power() const { return max(max(x, y), z); }
	float Length() const { return sqrt(x*x + y*y + z*z); }
	float SqrLength() const { return x*x + y*y + z*z; }

	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float c[3]; };
	};
};

typedef Vector3 Color;

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

	Normal3 operator-() const { return Normal3(-x, -y, -z); return *this; }
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

class Photon {
public:
	Photon() {}
	Photon(const Point3& position, const Vector3& direction,
		   const Color& color)
		   : position(position), direction(direction), color(color) {}

	Color color;
	Point3 position;
	Vector3 direction;
};

class AABB {
public:
	AABB() : pos(Point3(0, 0, 0)), size(Vector3(0, 0, 0)) {}
	AABB(const Point3 &pos, const Vector3& size) : pos(pos), size(size) {}
	Point3 GetPos() const { return pos; }
	Vector3 GetSize() const { return size; }

	bool Intersect(const AABB& ab) const {
		Point3 v1 = ab.GetPos();
		Point3 v2 = ab.GetPos() + ab.GetSize();
		Point3 v3 = pos;
		Point3 v4 = pos + size;
		return (v2.x > v3.x && v4.x > v1.x &&
				v2.y > v3.y && v4.y > v1.y &&
				v2.z > v3.z && v4.z > v1.z);
	}

	bool Intersect(const Ray& ray, float &hit0, float &hit1) {
		float t0 = 0, t1 = ray.tmax;
		Point3 pMin = pos;
		Point3 pMax = pos + size;
	    for (int i = 0; i < 3; ++i) {
	        float invRayDir = 1.f / ray.direction[i];
	        float tNear = (pMin[i] - ray.origin[i]) * invRayDir;
	        float tFar  = (pMax[i] - ray.origin[i]) * invRayDir;

	        if (tNear > tFar) std::swap(tNear, tFar);
	        t0 = tNear > t0 ? tNear : t0;
	        t1 = tFar  < t1 ? tFar  : t1;
	        if (t0 > t1) return false;
	    }
		hit0 = t0; hit1 = t1;
	    return true;
	}

	AABB Expand() const {
		return AABB(pos - Vector3(EPS, EPS, EPS),
					size + Vector3(EPS * 2, EPS * 2, EPS * 2));
	}

	AABB Combine(const AABB& ab) const {
		Point3 p = Point3( min(pos.x, ab.GetPos().x),
						   min(pos.y, ab.GetPos().y),
						   min(pos.z, ab.GetPos().z));
		Point3 pb = Point3(max(pos.x + size.x, ab.GetPos().x + ab.GetSize().x),
						   max(pos.y + size.y, ab.GetPos().y + ab.GetSize().y),
						   max(pos.z + size.z, ab.GetPos().z + ab.GetSize().z));
		return AABB(p, pb - p);
	}

	AABB CutLeft(int dim) const {
		if (dim == 0) return AABB(pos, Vector3(size.x / 2, size.y, size.z));
		if (dim == 1) return AABB(pos, Vector3(size.x, size.y / 2, size.z));
		if (dim == 2) return AABB(pos, Vector3(size.x, size.y, size.z / 2));
		puts("dim can only be less than 3 in functino AABB::CutLeft(int dim)");
		return AABB();
	}

	AABB CutRight(int dim) const {
		if (dim == 0) return AABB(pos + Vector3(size.x / 2, 0, 0),
								  Vector3(size.x / 2, size.y, size.z));
		if (dim == 1) return AABB(pos + Vector3(0, size.y / 2, 0),
								  Vector3(size.x, size.y / 2, size.z));
		if (dim == 2) return AABB(pos + Vector3(0, 0, size.z / 2),
								  Vector3(size.x, size.y, size.z / 2));
		puts("dim can only be less than 3 in functino AABB::CutRight(int dim)");
		return AABB();
	}

	bool Contain(const Point3& pos) const {
		Point3 v1 = pos;
	 	Point3 v2 = pos + size;
		return (v1.x - EPS < pos.x && pos.x < v2.x + EPS &&
				v1.y - EPS < pos.y && pos.y < v2.y + EPS &&
				v1.z - EPS < pos.z && pos.z < v2.z + EPS);
	}
	Point3 pos;
	Vector3 size;
};

inline std::ostream& operator<<(std::ostream &os, const AABB &ab) {
	cout << ab.pos << "  " << ab.pos + ab.size << endl;
	return os;
}

struct Matrix3 {
    Matrix3() {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) m[i][j] = 0;
        for(int i = 0; i < 3; i++) m[i][i] = 1;
    }
    Matrix3(const float mat[3][3]) {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) m[i][j] = mat[i][j];
    }
    Matrix3(float m00, float m01, float m02,
           float m10, float m11, float m12,
           float m20, float m21, float m22) {
               m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
               m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
               m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
           }
    bool operator==(const Matrix3 &_m) const {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if (m[i][j] != _m.m[i][j]) return false;
        return true;
    }
    bool operator!=(const Matrix3 &_m) const {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if (m[i][j] != _m.m[i][j]) return true;
        return false;
    }
	Vector3 operator*(const Vector3 &v) const;
    friend Matrix3 Transpose(const Matrix3 &m);
    friend Matrix3 Inverse(const Matrix3 &m);
    float m[3][3];
};

inline Point3 Min(const Point3& p1, const Point3& p2) {
	return Point3(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
}

inline Point3 Max(const Point3& p1, const Point3& p2) {
	return Point3(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}

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
