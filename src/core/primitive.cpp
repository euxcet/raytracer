#include "primitive.h"

namespace Raytracer {

bool GeometricPrimitive::Intersect(const Ray &r, Intersection *isc) const {
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

Mesh::Mesh(const char* file, Material *material) : material(material) {
}

bool Mesh::Intersect(const Ray &r, Intersection *isc) const {
	float dist = r.tmax;
	bool res = false;
	for(auto shape : shapes) {
		Ray ray(r.GetOrigin(), r.GetDirection(), dist);
		if ((res = shape -> Intersect(r, isc))) {
			isc -> primitive = this;
			dist = isc -> dist;
			res = true;
		}
	}
	return res;
}

}
