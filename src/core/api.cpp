#include "api.h"
#include "engine.h"
#include "primitive.h"
#include "material.h"
#include "scene.h"
#include "shape.h"
#include "geometry.h"
#include "shape/sphere.h"
#include "shape/plane.h"
#include "shape/triangle.h"
#include "accelerator/bfaccel.h"
#include "accelerator/kdtreeaccel.h"

namespace Raytracer {

    void Init() {
        Material *material = new Material();
        vector<Primitive*> primitives;
        vector<Primitive*> triangles;
        //plane

        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 1, 0), Point3(0, -23, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));


        primitives.push_back(new GeometricPrimitive(CreatePlaneShape(Normal3(0, 0, 1), Point3(0, 0, 0)),
                             new Material(Color(1, 1, 1), Color(0, 0, 0), 0, 0.7, 1, 0, 1.7,
                                new Texture("../texture/floor.pic"))
                            ));



        ifstream fin;
        fin.open("../obj/bunny.obj");
        int v, f;
        fin >> v >> f;
        char s[4];
        float x[v * 2];
        float y[v * 2];
        float z[v * 2];
        for(int i = 1; i <= v; i++) {
            fin >> s >> x[i] >> y[i] >> z[i];
            x[i] *= 20;
            y[i] *= 20;
            z[i] *= -20;
            std::swap(y[i], z[i]);
            y[i] = - y[i];
            z[i] -= 0.666198;
            y[i] -= 1;
        }
        int u[3];
        for(int i = 1; i <= f; i++) {
            fin >> s >> u[0] >> u[1] >> u[2];
            Point3 v0 = Point3(x[u[0]], y[u[0]], z[u[0]]);
            Point3 v2 = Point3(x[u[1]], y[u[1]], z[u[1]]);
            Point3 v1 = Point3(x[u[2]], y[u[2]], z[u[2]]);
            Shape* p = CreateTriangleShape(v0, v1, v2);
            triangles.push_back(new GeometricPrimitive(p,
                new Material(Color(1, 0, 0), Color(1, 0, 0), 0, 0, 0.45, 0.25, 1.7 )));
        }
        /*
        Point3 v0 = Point3(0, 0, -1);
        Point3 v1 = Point3(3, 0, -1);
        Point3 v2 = Point3(0, 2, -1);
        Shape* p = CreateTriangleShape(v0, v2, v1);
        triangles.push_back(new GeometricPrimitive(p,
            new Material(Color(1, 0, 0), Color(1, 0, 0), 0, 0, 0.45, 0.25, 1.7 )));
            */


        primitives.push_back(new GeometricPrimitive(CreateSphereShape(0, -8, 3.3, 3.3),
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

        Scene *scene = new Scene(CreateKDTreeAccelerator(triangles, primitives), lights, Color(0.1, 0.1, 0.1));
        Engine *engine = new Engine(scene, 400, 400);
        engine -> Render();
    }
}
