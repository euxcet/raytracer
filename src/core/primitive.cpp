#include "primitive.h"
#include "shape/triangle.h"

namespace Raytracer {

bool GeometricPrimitive::Intersect(const Ray &r, Intersection *isc) const {
	bool res;
	if ((res = shape -> Intersect(r, isc)))
		isc -> primitive = this;
	return res;
}

const Material* GeometricPrimitive::GetMaterial() const {
	return material;
}

const Shape* GeometricPrimitive::GetShape() const {
	return shape;
}

Mesh::Mesh(const char* file, Material *material, const Transform &transform, int offset)
	: material(material), transform(transform) {
    ifstream fin;
    fin.open(file);
    char s[100];
    int u[10], ut[10];
	int f = 0;
	vector<float> x, y, z;
	vector<float> cx, cy;
	x.push_back(0);y.push_back(0);z.push_back(0);
	cx.push_back(0);cy.push_back(0);
	fin >> s;
	while (true) {
		if (s[0] == 'f' && strlen(s) == 1) {
			f ++;
			int vertex = 0;
			while (fin >> s) {
				if (s[0] >= '0' && s[0] <= '9') {
					u[vertex] = atoi(s);
					fin >> s;
					ut[vertex] = atoi(s);
					fin >> s;
					vertex++;
				}
				else break;
			}
			if (f <= offset) continue;
			for(int i = 2; i < vertex; i++) {
		        Point3 v0 = Point3(x[u[0]], y[u[0]], z[u[0]]);
		        Point3 v1 = Point3(x[u[i - 1]], y[u[i - 1]], z[u[i - 1]]);
		        Point3 v2 = Point3(x[u[i]], y[u[i]], z[u[i]]);
				vector<Point3> vt;
				vt.push_back(Point3(cx[ut[0]], cy[ut[0]], 0));
				vt.push_back(Point3(cx[ut[i - 1]], cy[ut[i - 1]], 0));
				vt.push_back(Point3(cx[ut[i]], cy[ut[i]], 0));
		        Shape* p = CreateTriangleShape(v0, v1, v2, vt, transform);
		        triangles.push_back(new GeometricPrimitive(p, material)); // WARNING : material not new
			}
		}
		else if (s[0] == 'v' && strlen(s) == 1) {
			float tx, ty, tz;
			fin >> tx >> ty >> tz;
			x.push_back(tx);
			y.push_back(ty);
			z.push_back(tz);
			fin >> s;
		}
		else if (strlen(s) == 2 && s[0] == 'v' && s[1] == 't') {
			float tx, ty;
			fin >> tx >> ty;
			cx.push_back(tx);
			cy.push_back(ty);
			fin >> s;
		}
		else if (s[0] == '%') break;
		else fin >> s;
	}
	puts("OBJ LOADED");
}

bool Mesh::Intersect(const Ray &r, Intersection *isc) const {
	float dist = r.tmax;
	bool res = false;
	for(auto triangle : triangles) {
		Ray ray(r.GetOrigin(), r.GetDirection(), dist);
		if ((res = triangle -> Intersect(r, isc))) {
			isc -> primitive = this;
			dist = isc -> dist;
			res = true;
		}
	}
	return res;
}

}
