#include "api.h"
#include "engine.h"
#include "primitive.h"
#include "material.h"
#include "scene.h"
#include "shape.h"
#include "geometry.h"
#include "shape/bezier.h"
#include "shape/sphere.h"
#include "shape/plane.h"
#include "shape/triangle.h"
#include "accelerator/bfaccel.h"
#include "accelerator/kdtreeaccel.h"
#include "engine/rt.h"
#include "engine/ppm.h"

namespace Raytracer {

    void Init() {
        srand(unsigned(time(0)));
        Material *material = new Material();
        vector<Primitive*> primitives;
        vector<Mesh*> meshs;
        //plane

        /*
        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 1, 0), Point3(0, -23, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7)
                            ));


        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, 1), Point3(0, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7)
                            ));
                            */

        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 1, 0), Point3(0, -17, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.2, 0.6, 0, 1.7,
                                new Texture("../texture/wall1.pic"))
                            ));

        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, 1), Point3(0, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.4, 0.8, 0, 1.7,
                                new Texture("../texture/wall2.pic"))
                            ));


                            /*
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(0, 2, 2, 2),
                             new Material(Color(0, 1, 0), Color(0, 1, 0), 0, 0.3, 0.8, 0.25, 1.7)
                            ));
                            */

                            /*
        Primitive* bezier = new GeometricPrimitive(CreateBezier3Shape("../obj/teapot.bpt"),
                             new Material(Color(0, 1, 0), Color(0, 0, 0), 0, 0.3, 0.5, 0.25, 1.7));
        primitives.push_back(bezier);
        */

        meshs.push_back(new Mesh("../obj/sven_all.obj",
                        new Material(Color(0.803, 0.496, 0.195), Color(0.803, 0.496, 0.195), 0, 0.4, 0.6, 0, 1.7,
                            new Texture("../texture/sven.pic"))
                    , 370));
        meshs.push_back(new Mesh("../obj/sven_blade.obj",
                        new Material(Color(0.803, 0.496, 0.195), Color(0.803, 0.496, 0.195), 0, 0.4, 0.6, 0, 1.7,
                            new Texture("../texture/sven_blade.pic"))
                    ));


            /*
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(0, 2, 2, 2),
                             new Material(Color(1, 0, 0), Color(1, 0, 0), 0, 0.3, 0.45, 0.25, 1.7,
                                new Texture("../texture/marble.pic"))
                            ));
                            */


                            /*
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(0, -7.5, 5, 5),
                             new Material(Color(1, 0, 0), Color(1, 0, 0), 0, 0.14, 0.65, 0.25, 1.7,
                                new Texture("../texture/stone.pic"))
                            ));
                            */


        /*
        float br = 0.4;
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(-4.1, 1.54, br, br),
                             new Material(Color(1, 0, 0), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(4, 1.47, br, br),
                             new Material(Color(0, 0, 1), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));

        primitives.push_back(new GeometricPrimitive(CreateSphereShape(-2, 4.2, br, br),
                             new Material(Color(1, 0, 0), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(1.3, 3.9, br, br),
                             new Material(Color(0, 0, 1), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));

        primitives.push_back(new GeometricPrimitive(CreateSphereShape(-2.9, -2.0, br, br),
                             new Material(Color(1, 0, 0), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));
        primitives.push_back(new GeometricPrimitive(CreateSphereShape(3.3, -1.9, br, br),
                             new Material(Color(0, 0, 1), Color(RAND(), RAND(), RAND()), 1, 0, 0, 0, 1.2 + RAND() * 0.6)));
                             */


        vector<Light*> lights;
//        lights.push_back(new AreaLight(Point3(2, 2, 9), Vector3(1.2, 0, 0), Vector3(0, 1.2, 0), Color(1, 0.8, 0.87)));
        lights.push_back(new AreaLight(Point3(3, 5, 6), Vector3(1.4, 0, 0), Vector3(0, 1.4, 0), Color(1, 1, 1)));
        lights.push_back(new AreaLight(Point3(-3, 5, 6), Vector3(1.4, 0, 0), Vector3(0, 1.4, 0), Color(1, 1, 1)));

        puts("!!!");
        Scene *scene = new Scene(CreateKDTreeAccelerator(meshs, primitives), lights, Color(0.1, 0.1, 0.1));
        puts("!!!");
//        Engine *engine = CreateRaytracerEngine(scene, 640, 400);
//      Engine *engine = CreatePPMEngine(scene, 300, 300);
        Engine *engine = CreatePPMEngine(scene, 2560, 1600);
        engine -> Render();
    }
}
