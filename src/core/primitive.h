#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytracer.h"
#include "common.h"
#include "material.h"

namespace Raytracer {

class Primitive {
public:
	virtual ~Primitive();
	virtual int Intersect(const Ray &ray, float &dist) const = 0;
	virtual const Material* GetMaterial() const = 0;
};

class GeometricPrimitive : public Primtive {
public:
	GeometricPrimitive(const shared_ptr<Shape> &_shape,
					   const shared_ptr<Material> &_material)
		: shape(_shape), material(_material) {}
	virtual bool Interect(const Ray &r, float &dist) const;
	virtual const Material* GetMaterial() const;

private:
	shared_ptr<Shape> shape;
	shared_ptr<Material> material;
};

}


#endif
