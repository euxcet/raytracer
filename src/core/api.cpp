#include "api.h"
#include "engine.h"
#include "primitive.h"
#include "material.h"
#include "scene.h"
#include "shape.h"
#include "geometry.h"
#include "shape/sphere.h"
#include "shape/plane.h"
#include "accelerator/bfaccel.h"

namespace Raytracer {
    void Init() {
        /*
	Color color;
	Color absorb;
	float refr;
	float refl;
	float diff;
	float spec;
	float rindex;
    */
//    (-2,2,0.5)
        Material *material = new Material();
        vector<Primitive*> primitives;
        //plane
        /*
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, 1), 0),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));
                            */


        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, -1, 0), Point3(0, 10, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));

        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 1, 0), Point3(0, -10, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(1, 0, 0), Point3(-10, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(-1, 0, 0), Point3(10, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, 1), Point3(0, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, -1), Point3(0, 0, 10)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));



        primitives.push_back(new GeometricPrimitive(CreateSphereShape(0, -2, 2.4, 2.4),
                             new Material(Color(1, 0, 0), Color(1, 0, 0), 0, 0.3, 0.45, 0.25, 1.7,
                                new Texture("../texture/marble.pic"))
                            ));

        primitives.push_back(new GeometricPrimitive(CreateSphereShape(-2.2, 2, 0.7, 0.7),
                             new Material(Color(1, 0, 0), Color(1, 0, 0), 1, 0, 0, 0, 1.7)));
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(2.2, 2, 0.7, 0.7),
                             new Material(Color(0, 0, 1), Color(0, 0, 1), 1, 0, 0, 0, 1.7)));


        vector<Light*> lights;
        //lights.push_back(new PointLight(Point3(-3, 3, 5), Color(1, 1, 1)));
        lights.push_back(new AreaLight(Point3(-3, 3, 5), Vector3(1.5, 0, 0), Vector3(0, 1.5, 0),
                                        Color(1, 1, 1)));

        Scene *scene = new Scene(CreateBFAccelerator(primitives), lights, Color(0.1, 0.1, 0.1));
        Engine *engine = new Engine(scene, 800, 800);
        engine -> Render();
    }
}
