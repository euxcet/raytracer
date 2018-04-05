#include "primitive.h"

namespace Raytracer {

bool GeometricPrimitive::Intersect(const Ray &r, Intersection *isc) const {
	float dist = r.tmax;
	bool res;
	if ((res = shape -> Intersect(r, isc)))
		isc -> primitive = this;
	return res;
}

const Material* GeometricPrimitive::GetMaterial() const {
	return material;
}

const Shape* GeometricPrimitive::GetShape() const {
	return shape;
}


}
