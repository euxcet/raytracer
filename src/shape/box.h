#ifndef SHAPE_BOX_H
#define SHAPE_BOX_H

#include "shape.h"
#include "geometry.h"

namespace Raytracer {

class Box : public Shape {
public:
	Box(const Point3& pos, const Vector3& size) : box(AABB(pos, size)) {}
	Box(const AABB& box) : box(box) {}

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const;
	
	Point3 GetPos() const { return box.GetPos(); }
	Vector3 GetSize() const { return box.GetSize(); }
	AABB GetAABB() const { return box; }

protected:
	AABB box;
};

}

#endif
