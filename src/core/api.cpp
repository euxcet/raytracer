#include "api.h"
#include "engine.h"
#include "primitive.h"
#include "camera.h"
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

Texture* LoadTexture(ifstream &fin) {
    string s;
    string file;
    while (fin >> s) {
        if (s == "texture:") fin >> file;
        if (s == "}") break;
    }
    return new Texture(file.c_str());
}

Material* LoadMaterial(ifstream &fin) {
    string s;
    Color color(1, 1, 1);
    Color absorb(1, 1, 1);
    float refr = 0;
    float refl = 0.6;
    float diff = 0.6;
    float spec = 0.4;
    float rindex = 1.5;
    Texture *texture = NULL;
    while (fin >> s) {
        if (s == "color:") fin >> color.x >> color.y >> color.z;
        if (s == "absorb:") fin >> absorb.x >> absorb.y >> absorb.z;
        if (s == "refr:") fin >> refr;
        if (s == "refl:") fin >> refl;
        if (s == "diff:") fin >> diff;
        if (s == "spec:") fin >> spec;
        if (s == "rindex:") fin >> rindex;
        if (s == "Texture") texture = LoadTexture(fin);
        if (s == "}") break;
    }
    if (texture == NULL)
        return new Material(color, absorb, refr, refl, diff, spec, rindex);
    return new Material(color, absorb, refr, refl, diff, spec, rindex, texture);
}

Transform LoadTransform(ifstream &fin, int type) {
    string s;
    Vector3 delta;
    Vector3 scale;
    float theta;
    while (fin >> s) {
        if (s == "delta:") fin >> delta.x >> delta.y >> delta.z;
        if (s == "scale:") fin >> scale.x >> scale.y >> scale.z;
        if (s == "theta:") fin >> theta;
        if (s == "}") break;
    }
    if (type == 0) return Translate(delta);
    if (type == 1) return Scale(scale);
    if (type == 2) return RotateX(theta);
    if (type == 3) return RotateY(theta);
    if (type == 4) return RotateZ(theta);
}


Primitive* LoadSphere(ifstream &fin) {
    string s;
    Point3 origin;
    float radius;
    Material *material = NULL;
    Transform transform;
    while (fin >> s) {
        if (s == "origin:") fin >> origin.x >> origin.y >> origin.z;
        if (s == "radius:") fin >> radius;
        if (s == "Material") material = LoadMaterial(fin);
        if (s == "}") break;
    }
    return new GeometricPrimitive(CreateSphereShape(origin, radius, transform), material);
}

Primitive* LoadPlane(ifstream &fin) {
    string s;
    Normal3 normal;
    float dist;
    Material *material = NULL;
    Transform transform;
    while (fin >> s) {
        if (s == "normal:") fin >> normal.x >> normal.y >> normal.z;
        if (s == "dist:") fin >> dist;
        if (s == "Material") material = LoadMaterial(fin);
        if (s == "}") break;
    }
    return new GeometricPrimitive(CreatePlaneShape(normal, dist, transform), material);
}

Mesh* LoadMesh(ifstream &fin) {
    string s;
    string obj;
    int offset = 0;
    Material *material = NULL;
    Transform transform;
    while (fin >> s) {
        if (s == "obj:") fin >> obj;
        if (s == "offset:") fin >> offset;
        if (s == "Material") material = LoadMaterial(fin);
        if (s == "Translate") transform *= LoadTransform(fin, 0);
        if (s == "Scale") transform *= LoadTransform(fin, 1);
        if (s == "RotateX") transform *= LoadTransform(fin, 2);
        if (s == "RotateY") transform *= LoadTransform(fin, 3);
        if (s == "RotateZ") transform *= LoadTransform(fin, 4);
        if (s == "}") break;
    }
    return new Mesh(obj.c_str(), material, transform, offset);
}

Primitive* LoadBezier(ifstream &fin) {
    string s;
    string obj;
    Material *material = NULL;
    Transform transform;
    while (fin >> s) {
        if (s == "obj:") fin >> obj;
        if (s == "Material") material = LoadMaterial(fin);
        if (s == "Translate") transform *= LoadTransform(fin, 0);
        if (s == "Scale") transform *= LoadTransform(fin, 1);
        if (s == "RotateX") transform *= LoadTransform(fin, 2);
        if (s == "RotateY") transform *= LoadTransform(fin, 3);
        if (s == "RotateZ") transform *= LoadTransform(fin, 4);
        if (s == "}") break;
    }
    return new GeometricPrimitive(CreateBezier3Shape(obj.c_str(), transform), material);
}

Light* LoadLight(ifstream &fin, int type) {
    string s;
    Point3 origin;
    Vector3 dx, dy;
    Color color;
    float power = 60;
    float scope = 0.1;
    while (fin >> s) {
        if (s == "origin:") fin >> origin.x >> origin.y >> origin.z;
        if (s == "dx:") fin >> dx.x >> dx.y >> dx.z;
        if (s == "dy:") fin >> dy.x >> dy.y >> dy.z;
        if (s == "color:") fin >> color.x >> color.y >> color.z;
        if (s == "power:") fin >> power;
        if (s == "scope:") fin >> scope;
        if (s == "}") break;
    }
    if (type == 0) return new PointLight(origin, color, power, scope);
    return new AreaLight(origin, dx, dy, color, power, scope);
}

Camera* LoadCamera(ifstream &fin, int width, int height, int type) {
    string s;
    Point3 eye;
    Point3 des;
    while (fin >> s) {
        if (s == "width:") fin >> width;
        if (s == "height:") fin >> height;
        if (s == "eye:") fin >> eye.x >> eye.y >> eye.z;
        if (s == "des:") fin >> des.x >> des.y >> des.z;
        if (s == "}") break;
    }
    Vector3 dir = Normalize(des - eye);
    Vector3 up = Normalize(Vector3(0, 1, -dir.y / dir.z));
    if (type == 0) return new Camera(eye, dir, up, des, width, height);
    return new FocusCamera(eye, dir, up, des, width, height);
}

Scene* LoadScene(ifstream &fin) {
    string s;
    string accelerator;
    Color bg;
    vector<Mesh*> meshs;
    vector<Primitive*> primitives;
    vector<Light*> lights;
    while (fin >> s) {
        if (s == "accelerator:") fin >> accelerator;
        if (s == "background:") fin >> bg.x >> bg.y >> bg.z;
        if (s == "Plane") primitives.push_back(LoadPlane(fin));
        if (s == "Sphere") primitives.push_back(LoadSphere(fin));
        if (s == "Mesh") meshs.push_back(LoadMesh(fin));
        if (s == "Bezier") primitives.push_back(LoadBezier(fin));
        if (s == "PointLight") lights.push_back(LoadLight(fin, 0));
        if (s == "AreaLight") lights.push_back(LoadLight(fin, 1));
        if (s == "}") break;
    }
    if (accelerator == "kdtree") {
        return new Scene(CreateKDTreeAccelerator(meshs, primitives), lights, bg);
    }
    else { // bruteforce
        return new Scene(CreateBFAccelerator(primitives), lights, bg);
    }

}

Engine* LoadEngine(ifstream &fin, int type) {
    string s;
    Camera *camera = NULL;
    Scene *scene;
    int width, height;
    int photons = 100000;
    while (fin >> s) {
        if (s == "width:") fin >> width;
        if (s == "height:") fin >> height;
        if (s == "photons:") fin >> photons;
        if (s == "Scene") scene = LoadScene(fin);
        if (s == "Camera") camera = LoadCamera(fin, width, height, 0);
        if (s == "FocusCamera") camera = LoadCamera(fin, width, height, 1);
        if (s == "}") break;
    }
    if (scene == NULL) {
        puts("Load scene fail: No scene");
        return NULL;
    }
    if (camera == NULL) {
        puts("Load scene fail: No camera");
        return NULL;
    }
    if (type == 0) return CreateRaytracerEngine(scene, camera, width, height);
    return CreatePPMEngine(scene, camera, width, height, photons);
}

void Init() {
    srand(unsigned(time(0)));
    ifstream fin("../scene/scene.txt");
    string s;
    Engine *engine = NULL;
    while (fin >> s) {
        if (s == "Raytracer") engine = LoadEngine(fin, 0);
        if (s == "PPM") engine = LoadEngine(fin, 1);
    }
    if (engine == NULL) {
        puts("Load scene fail: No engine");
        return;
    }
    engine -> Render();
    delete engine;
}

}
