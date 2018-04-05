#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytracer.h"
#include "geometry.h"
#include "material.h"
#include "intersection.h"
#include "shape.h"

namespace Raytracer {

class Primitive {
public:
	virtual ~Primitive() {}
	virtual bool Intersect(const Ray &ray, Intersection *isc) const = 0;
	virtual const Material* GetMaterial() const = 0;
	virtual const Shape* GetShape() const = 0;
};

class GeometricPrimitive : public Primitive {
public:
	GeometricPrimitive(Shape* shape, Material* material)
		: shape(shape), material(material) {}
	~GeometricPrimitive() {}
	virtual bool Intersect(const Ray &r, Intersection *isc) const;
	const Material* GetMaterial() const;
	const Shape* GetShape() const;

private:
	Shape* shape;
	Material* material;
};

class Aggregate : public Primitive {
public:
	const Material* GetMaterial() const {
		puts("Can't get material from class [Aggregate]!");
		return NULL;
	}
	const Shape* GetShape() const {
		puts("Can't get shape from class [Aggregate]!");
		return NULL;
	}
};

}


#endif
