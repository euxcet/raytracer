#include "api.h"
#include "engine.h"
#include "primitive.h"
#include "material.h"
#include "scene.h"
#include "shape.h"
#include "geometry.h"
#include "shape/sphere.h"

namespace Raytracer {
    void Init() {
        Material *material = new Material();
        Primitive *primitive = new GeometricPrimitive(CreateSphereShape(2, 2, 3, 1.5), material);
        vector<Light*> lights;
        lights.push_back(new Light(Point3(0, 0, 0), Color(1, 1, 1)));
        lights.push_back(new Light(Point3(2, 5, 1), Color(0.6, 0.6, 0.6)));
        Scene *scene = new Scene(primitive, lights);
        Engine *engine = new Engine(scene, 800, 600);
        engine -> Render();
    }
}
