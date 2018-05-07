#ifndef SHAPE_TRIANGLE_H
#define SHAPE_TRIANGLE_H

#include "shape.h"

namespace Raytracer {

	static Point3 MIN = Point3(100, 100, 100);

class Triangle : public Shape {
public:
	Triangle(const Point3 &_v0, const Point3 &_v1, const Point3 &_v2, Transform transform) :
        Shape(transform) {
			v0 = transform(_v0);
			v1 = transform(_v1);
			v2 = transform(_v2);
            Vector3 v0v1 = Normalize(v1 - v0);
            Vector3 v0v2 = Normalize(v2 - v0);
            N = Normal3(Normalize(Cross(v0v1, v0v2)));
        }

	Triangle(const Point3 &_v0, const Point3 &_v1, const Point3 &_v2,
			 const vector<Normal3> &_vn, Transform transform) :
        Shape(transform), vn(_vn) {
			v0 = transform(_v0);
			v1 = transform(_v1);
			v2 = transform(_v2);
            Vector3 v0v1 = Normalize(v1 - v0);
            Vector3 v0v2 = Normalize(v2 - v0);
            N = Normal3(Normalize(Cross(v0v1, v0v2)));
			MIN = Min(MIN, v0);
			MIN = Min(MIN, v1);
			MIN = Min(MIN, v2);
			for(int i = 0; i < 3; i ++) {
				vn[i] = transform(vn[i]);
			}
        }

		/*
	Triangle(const Point3 &_v0, const Point3 &_v1, const Point3 &_v2,
			 const vector<Point3> &vt, Transform transform) :
        Shape(transform), vt(vt) {
			v0 = transform(_v0);
			v1 = transform(_v1);
			v2 = transform(_v2);
            Vector3 v0v1 = Normalize(v1 - v0);
            Vector3 v0v2 = Normalize(v2 - v0);
            N = Normal3(Normalize(Cross(v0v1, v0v2)));
        }
		*/

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return N; }
	Point3 GetCenter() const { return (v0 + v1 + v2) / 3.; }
    AABB GetAABB() const;
    pair<float, float> Coordinate(const Point3& pos) const; // TODO: modify pair<int,int> to Point2

private:
	vector<Point3> vt;
	vector<Normal3> vn;
    Point3 v0;
    Point3 v1;
    Point3 v2;
    Normal3 N;
};

Shape* CreateTriangleShape(const Point3 &v0, const Point3& v1, const Point3 &v2, Transform transform);
Shape* CreateTriangleShape(const Point3 &v0, const Point3 &v1, const Point3 &v2,
                           const vector<Normal3> &vn, Transform transform);


}

#endif
