#ifndef SHAPE_BOX_H
#define SHAPE_BOX_H


namespace Raytracer {

class Box : public Primitive {
public:
	Box() : box(Vector3(0, 0, 0), Vector3(0, 0, 0)) {}
	Box(const aabb& _box) : box(_box) {}

	int Intersect(const Ray& ray, float& dist) const;
	Normal3 GetNormal(const Normal3& pos) const;


	bool IntersectBox(const aabb& _box) const { return box.Intersect(_box); }
	bool Contain(const Point3& p) const { return box.Contain(p); }
	Point3 GetPos() const { return box.GetPos(); }
	Vector3 GetSize() const { return box.GetSize(); }

	aabb GetAABB() const { return box; }

protected:
	aabb box;
};

}

#endif
