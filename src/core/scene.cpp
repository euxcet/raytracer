#include "scene.h"

namespace Raytracer {

void Scene::Intersect(const Ray &ray, float &dist) const {
    return aggregate -> Intersect(ray, dist);
}

};
