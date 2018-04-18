#ifndef SHAPE_BEZIER_H
#define SHAPE_BEZIER_H

#include "shape.h"
#include <Eigen/Dense>

using namespace Eigen;

namespace Raytracer{

class Bezier3 : public Shape {
public:
    Bezier3(const char* FileName);
    bool Intersect(const Ray& Ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return Normal3(); }
    AABB GetAABB() const { return AABB(); }
    pair<float, float> Coordinate(const Point3& pos) const { return make_pair(0, 0); }
    void LoadModel(const char* FileName);

private:
    Vector3f* m_Patch;
    Vector3f* m_ControlPoints;
    int m_Patches;
};

Shape* CreateBezier3Shape( const char* FileName);

}


#endif
