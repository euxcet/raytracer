#include "primitive.h"

namespace Raytracer {

bool GeometricPrimitive::Intersect(const Ray &r, float &dist) const {
	return shape -> Intersect(r, dist);
}

const Material* GeometricPrimitive::GetMaterial() const {
	return material.get();
}

}
