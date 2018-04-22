#include "shape/bezier.h"

namespace Raytracer {
    #define ITERDEPTH (5)

    Vector3f S(float u,float v,Vector3f* Patch){
        float u2 = u * u;
        float u3 = u2 * u;
        float f = 1.f;
        Vector3f s(0,0,0);
        int i = 15;
        while(i >= 0){
            Vector3f t = Patch[i--];
            t += u * Patch[i--];
            t += u2 * Patch[i--];
            t += u3 * Patch[i--];
            s += t * f;
            f *= v;
        }
        return s;
    }
    Vector3f dS_du(float u,float v,Vector3f* Patch){
        float u2 = 2 * u;
        float u3 = 3 * u * u;
        float f = 1.f;
        Vector3f s(0,0,0);
        int i = 15;
        while(i >= 0){
            i --;
            Vector3f t = Patch[i--];
            t += u2 * Patch[i--];
            t += u3 * Patch[i--];
            s += t * f;
            f *= v;
        }
        return s;
    }
    Vector3f dS_dv(float u, float v,Vector3f* Patch){
        float u2 = u * u;
        float u3 = u2 * u;
        float f = 1.f;
        Vector3f s(0,0,0);
        int i = 11;
        while(i >= 0){
            Vector3f t = Patch[i--];
            t += u * Patch[i--];
            t += u2 * Patch[i--];
            t += u3 * Patch[i--];
            s += t * f;
            f *= i == 7 ? 2 * v : 1.5 * v;
        }
        return s;
    }

    void NT(Vector2f& xn,const Vector3f& N1,const Vector3f& N2,float d1, float d2,Vector2f R,Vector3f* Patch){
        Vector3f pspu = dS_du(xn(0),xn(1),Patch);
        Vector3f pspv = dS_dv(xn(0),xn(1),Patch);
        Matrix2f J;
        J << N1.dot(pspu), N1.dot(pspv),
             N2.dot(pspu), N2.dot(pspv);
        xn -= J.inverse() * R;
    }

    Vector2f Initx0(const Vector3f& o,const Vector3f& dir, Vector3f* CP){
        float minD2 = INF;
        float t = 0.0f;
        int Patch_Num = -1;
        for(int i = 0 ; i < 16 ; i ++){
            Vector3f from_o = CP[i] - o;
            float temp_t = from_o.dot(dir);

            float dis2 = from_o.dot(from_o) - temp_t * temp_t;
            if(dis2 < minD2){
                minD2 = dis2;
                Patch_Num = i;
            }
        }
        return Vector2f(Patch_Num / 4 * (1 / 3.f) , Patch_Num % 4 * (1 / 3.f));
    }

    bool NTIteration(const Vector2f& x0, const Vector3f& N1, const Vector3f& N2, float d1, float d2, Vector3f* Patch, Vector2f& res){
        Vector2f x = x0;
        Vector3f S_ = S(x(0),x(1),Patch);
        Vector2f R(N1.dot(S_) + d1, N2.dot(S_) + d2);
        float last = R.dot(R);
        for(int i = 0; i < ITERDEPTH; i ++){
            if(last < 1e-6){
                res = x;
                if (res[0] >= -EPS && res[0] <= 1+EPS && res[1] >= -EPS && res[1] <= 1+EPS)
                    return true;
                else return false;
            }
            NT(x,N1,N2,d1,d2,R,Patch);
            S_ = S(x(0),x(1),Patch);
            R << N1.dot(S_) + d1, N2.dot(S_) + d2;

            if(R.dot(R) > last)
                return false;
            last = R.dot(R);
        }
        return false;
    }


    Bezier3::Bezier3( const char* FileName )
    :m_Patches(0),m_Patch(NULL),m_ControlPoints(NULL) { LoadModel(FileName); }

    bool Bezier3::Intersect(const Ray& ray, Intersection* isc) const {
        Point3 o = ray.GetOrigin();
        Vector3 dir = ray.GetDirection();
        float a_Dist = ray.tmax;
        Vector3f D(dir.x,dir.y,dir.z);
        Vector3f N1(dir.y, -dir.x, 0);
        N1.normalize();
        Vector3f O(o.x, o.y, o.z);
        float d1 = -N1.dot(O);
        Vector3f N2 = N1.cross(D);
        float d2 = -N2.dot(O);


        Vector2f res;
        float u,v;
        int Patch_Hit = -1;
        bool hit = false;
        for(int i = 0 ; i < m_Patches ; i ++){

            float minx = INF, miny = INF, minz = INF;
            float maxx = -INF, maxy= -INF, maxz = -INF;
            for(int j = i * 16; j < i * 16 + 16; j++) {
                minx = min(minx, m_ControlPoints[j](0));
                miny = min(miny, m_ControlPoints[j](1));
                minz = min(minz, m_ControlPoints[j](2));
                maxx = max(maxx, m_ControlPoints[j](0));
                maxy = max(maxy, m_ControlPoints[j](1));
                maxz = max(maxz, m_ControlPoints[j](2));
            }
            float hit0, hit1;
            AABB aabb(Point3(minx, miny, minz), Point3(maxx, maxy, maxz) - Point3(minx, miny, minz));
            if (!aabb.Expand().Intersect(ray, hit0, hit1)) continue;


            for(int _ = 0; _ < 5; _++) {
                Vector2f x0(RAND(), RAND());
                if(NTIteration(x0,N1,N2,d1,d2,m_Patch + 16 * i,res)){
                    float t = (S(res(0), res(1), m_Patch + 16 * i) - O).dot(D);
                    if(a_Dist > t){
                        Patch_Hit = i;
                        a_Dist = t;
                        u = res(0);
                        v = res(1);
                        hit = true;
                        break;
                    }
                }
            }
        }
        if(hit){
            Vector3f* m_Patch_Hit = m_Patch + 16 * Patch_Hit;
            isc -> hit = HIT;
			isc -> dist = a_Dist;
            Vector3f ip = S(u, v, m_Patch_Hit);
			isc -> p = Point3(ip(0), ip(1), ip(2));
            Vector3f a_Normal = dS_du(u,v,m_Patch_Hit).cross(dS_dv(u,v,m_Patch_Hit)).normalized();
            Normal3 normal = Normal3(a_Normal(0), a_Normal(1), a_Normal(2));
			isc -> n = Normalize(normal);
        }
        return hit;
    }
    void Bezier3::LoadModel(const char* FileName){
        ifstream in;
        in.open(FileName);
        in >> m_Patches;
        m_Patch = new Vector3f[m_Patches * 16];
        m_ControlPoints = new Vector3f[m_Patches * 16];
        int m, n;
        float x, y, z;
        Matrix4f CPx,CPy,CPz;
        Matrix4f N;
        N << -1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0;
        for(int i = 0; i < m_Patches; i ++){
            in >> m >> n;
            for(int j = 0; j < 16; j ++){
                in >> x >> y >> z;
                m_ControlPoints[i * 16 + j] = Vector3f(x,y,z);
                CPx(j) = x, CPy(j) = y, CPz(j) = z;
            }
            CPx = (N * CPx * N.transpose()).transpose();
            CPy = (N * CPy * N.transpose()).transpose();
            CPz = (N * CPz * N.transpose()).transpose();
            for(int j = 0; j < 16; j ++){
                m_Patch[i * 16 + j] = Vector3f(CPx(j),CPy(j),CPz(j));
            }
        }
    }

    Shape* CreateBezier3Shape(const char* FileName) {
        return new Bezier3(FileName);
    }

}

/*
#include "shape/bezier.h"

namespace Raytracer {

    #define ITERTIMES 4
    #define ITERDEPTH 5
    float B(int i,float t){
        if(i == 0)
            return (1.0f - t) * (1.0f - t) * (1.0f - t);
        if(i == 1)
            return 3.0f * (1.0f - t) * (1.0f - t) * t;
        if(i == 2)
            return 3.0f * (1.0f - t) * t * t;
        if(i == 3)
            return t * t * t;
        return 0.0f;
    }
    float dB(int i,float t){
        if(i == 0)
            return -3.0f * (1.0f - t) * (1.0f - t);
        if(i == 1)
            return 3.0f * (1.0f - t) * ( 1.0f - 3.0f * t);
        if(i == 2)
            return 3.0f * t * (2.0f - 3.0f * t);
        if(i == 3)
            return 3.0f * t * t;
        return 0.0f;
    }
    float Rand(float range){
        return rand() * range / RAND_MAX;
    }
    Vector3f Initx0(const Vector3f& p, const Vector3f& w, Vector3f* Patch){
        float minD2 = INF;
        float t = 0.0f;
        int Patch_Num = -1;
        for(int i = 0 ; i < 16 ; i ++){
            Vector3f from_o = Patch[i] - p;
            float temp_t = from_o.dot(w);
            float dis2 = from_o.dot(from_o) - temp_t * temp_t;
            if(dis2 < minD2){
                minD2 = dis2;
                t = temp_t;
                Patch_Num = i;
            }
        }
        return Vector3f(t, Patch_Num / 4 * (1./3) , Patch_Num % 4 * (1./3));
    }

Vector3f F(Vector3f& xi, Vector3f* Patch, Vector3f& p, Vector3f& w){        Vector3f s = -p - w * xi(0);        float B_i[] = {B(0,xi(1)), B(1,xi(1)), B(2,xi(1)), B(3,xi(1))};        float B_j[] = {B(0,xi(2)), B(1,xi(2)), B(2,xi(2)), B(3,xi(2))};        for(int i = 0; i < 4; i ++)            for(int j = 0; j < 4; j ++)                s += B_i[i] * B_j[j] * Patch[i * 4 + j];        return s;    }    Vector3f aF_au(Vector3f& xi, Vector3f* Patch){        Vector3f s(0.0f,0.0f,0.0f);        float dB_i[] = {dB(0,xi(1)), dB(1,xi(1)), dB(2,xi(1)), dB(3,xi(1))};        float B_j[] = {B(0,xi(2)), B(1,xi(2)), B(2,xi(2)), B(3,xi(2))};        for(int i = 0; i < 4;i ++)            for(int j = 0 ; j < 4 ; j ++)                s += dB_i[i] * B_j[j] * Patch[i * 4 + j];        return s;    }    Vector3f aF_av(Vector3f& xi, Vector3f* Patch){        Vector3f s(0.0f,0.0f,0.0f);        float B_i[] = {B(0,xi(1)), B(1,xi(1)), B(2,xi(1)), B(3,xi(1))};        float dB_j[] = {dB(0,xi(2)), dB(1,xi(2)), dB(2,xi(2)), dB(3,xi(2))};        for(int i = 0; i < 4;i ++)            for(int j = 0 ; j < 4 ; j ++)                s += B_i[i] * dB_j[j] * Patch[i * 4 + j];        return s;    }
    Vector3f NT(Vector3f& xi, Vector3f* Patch, Vector3f& p, Vector3f& w){
        Matrix3f F_;
        F_.col(0) = -w;
        F_.col(1) = aF_au(xi, Patch);
        F_.col(2) = aF_av(xi, Patch);
        return xi - F_.inverse() * F(xi, Patch, p, w);
    }
    bool NTIteration(Vector3f &x0 , Vector3f* Patch, Vector3f& p, Vector3f& w, Vector3f& res){
        Vector3f last = x0;
        for(int i = 0; i < ITERDEPTH; i ++){
            x0 = NT(x0, Patch,p ,w);

            if((last - x0).dot(last - x0) < 0.0001f){
                res = x0;
                return (res(0) > 0 && !(x0(1) < -0 || x0(1) > 1.0f || x0(2) < -0 || x0(2) > 1.f)) ? true : false;
            }
            last = x0;
        }
        return false;
    }

    Point3 P(float u, float v, Vector3f* Patch){
        Vector3f s(0.0f,0.0f,0.0f);
        for(int i = 0; i < 4; i ++)
        for(int j = 0; j < 4; j ++)
        s += B(i, u) * B(j,v) * Patch[i * 4 + j];
        return Point3(s(0), s(1), s(2));
    }

    Vector3 aP_au(float u, float v, Vector3f* Patch){
        Vector3f s(0.0f,0.0f,0.0f);
        for(int i = 0; i < 4;i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += dB(i, u) * B(j, v) * Patch[i * 4 + j];
        return Vector3(s(0), s(1), s(2));
    }
    Vector3 aP_av(float u, float v, Vector3f* Patch){
        Vector3f s(0.0f,0.0f,0.0f);
        for(int i = 0; i < 4;i ++)
            for(int j = 0 ; j < 4 ; j ++)
                s += B(i, u) * dB(j, v) * Patch[i * 4 + j];
        return Vector3(s(0), s(1), s(2));
    }

    Bezier3::Bezier3( const char* FileName )
    :m_Patches(0),m_Patch(NULL){ LoadModel(FileName); }

    bool Bezier3::Intersect(const Ray& ray, Intersection* isc) const {
        Vector3 dir = ray.GetDirection();
        Point3 o = ray.GetOrigin();
        float a_Dist = ray.tmax;
        Vector3f p(o.x, o.y, o.z);
        Vector3f w(dir.x, dir.y, dir.z);
        Vector3f res;
        float u, v;
        int Patch_Hit = -1;
        bool hit = false;
        for(int i = 0; i < m_Patches; i ++){

            float minx = INF, miny = INF, minz = INF;
            float maxx = -INF, maxy= -INF, maxz = -INF;
            for(int j = i * 16; j < i * 16 + 16; j++) {
                minx = min(minx, m_Patch[j](0));
                miny = min(miny, m_Patch[j](1));
                minz = min(minz, m_Patch[j](2));
                maxx = max(maxx, m_Patch[j](0));
                maxy = max(maxy, m_Patch[j](1));
                maxz = max(maxz, m_Patch[j](2));
            }
            float hit0, hit1;
            AABB aabb(Point3(minx, miny, minz), Point3(maxx, maxy, maxz) - Point3(minx, miny, minz));
            if (!aabb.Expand().Intersect(ray, hit0, hit1)) continue;


            for(int j = 0; j < ITERTIMES ; j ++){
                Vector3f x0 = Initx0(p, w, m_Patch + 16 * i);//(t,u,v)
                if(NTIteration(x0, m_Patch + 16 * i, p, w, res)){
                    if(a_Dist > res(0)){
                        Patch_Hit = i;
                        a_Dist = res(0);
                        u = res(1);
                        v = res(2);
                        hit = true;
                        break;
                    }
                }
            }
        }
        if(hit){
            Vector3f* m_Patch_Hit = m_Patch + 16 * Patch_Hit;
            isc -> hit = HIT;
			isc -> dist = a_Dist;
			isc -> p = P(u, v, m_Patch_Hit);
            Normal3 a_Normal = Normal3(Cross(aP_au(u, v, m_Patch_Hit), aP_av(u, v, m_Patch_Hit)));
			isc -> n = Normalize(a_Normal);
        }
        return hit;
    }
    void Bezier3::LoadModel(const char* FileName){
        ifstream in;
        in.open(FileName,std::ios::in);
        in >> m_Patches;
        m_Patch = new Vector3f[m_Patches * 16];
        int m, n;
        float x, y, z;
        float minz = 1000;
        for(int i = 0; i < m_Patches; i ++){
            in >> m >> n;
            for(int j = 0; j < 16; j ++){
                in >> x >> y >> z;
                minz = min(minz, z);

                m_Patch[i * 16 + j] = Vector3f(x, y, z);
            }
        }
    }

    Shape* CreateBezier3Shape(const char* FileName) {
        return new Bezier3(FileName);
    }

}
*/
