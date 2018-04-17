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

Mesh::Mesh(const char* file, Material *material, int offset) : material(material) {
    ifstream fin;
    fin.open(file);
    char s[100];
	int v = 0, f = 0, vt = 0;
    float x[100000];
    float y[100000];
    float z[100000];
	float cx[100000];
	float cy[100000];
    int u[10], ut[10];
	fin >> s;
	int ftot = 0;
	int maxt = 0;
	float minx = 1000;
	float maxx = -1000;
	float miny = 1000;
	float maxy = -1000;
	while (true) {
		if (s[0] == 'f' && strlen(s) == 1) {
			ftot ++;
			int vertex = 0;
			while (fin >> s) {
				if (s[0] >= '0' && s[0] <= '9') {
					u[vertex] = atoi(s);
					fin >> s;
					ut[vertex] = atoi(s);
					maxt = max(maxt, ut[vertex]);
					fin >> s;
					vertex++;
				}
				else break;
			}
			if (ftot <= offset) continue;
			for(int i = 2; i < vertex; i++) {
		        Point3 v0 = Point3(x[u[0]], y[u[0]], z[u[0]]);
		        Point3 v1 = Point3(x[u[i - 1]], y[u[i - 1]], z[u[i - 1]]);
		        Point3 v2 = Point3(x[u[i]], y[u[i]], z[u[i]]);
				vector<Point3> vt;
				vt.push_back(Point3(cx[ut[0]], cy[ut[0]], 0));
				vt.push_back(Point3(cx[ut[i]], cy[ut[i]], 0));
				vt.push_back(Point3(cx[ut[i - 1]], cy[ut[i - 1]], 0));
		        Shape* p = CreateTriangleShape(v0, v2, v1, vt);
		        triangles.push_back(new GeometricPrimitive(p, material)); // WARNING : material not new
			}
		}
		else if (s[0] == 'v' && strlen(s) == 1) {
			++v;
			fin >> x[v] >> y[v] >> z[v];
			x[v] /= 40.;
			y[v] /= 40.;
			z[v] /= 40.;
			std::swap(y[v], z[v]);
			y[v] += 2;
			x[v] += 2;
			fin >> s;
		}
		else if (strlen(s) == 2 && s[0] == 'v' && s[1] == 't') {
			++vt;
			fin >> cx[vt] >> cy[vt];

			minx = min(minx, cx[vt]);
			maxx = max(maxx, cx[vt]);
			miny = min(miny, cy[vt]);
			maxy = max(maxy, cy[vt]);

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
