#include "api.h"
#include "primitive/sphere.h"

namespace Raytracer {
    static unique_ptr<Scene> scene;
    void Init() {
        scene.reset(new Scene);
        shared_ptr<Primitive> shape = CreateSpherePrimitive(1, 2, 3, 3);
        unique_ptr<
    }
}
