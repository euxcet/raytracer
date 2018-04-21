#ifndef API_H
#define API_H

#include "raytracer.h"

namespace Raytracer {
    Engine* LoadEngine(ifstream &fin, int type);
    Scene* LoadScene(ifstream &fin);
    void Init();
}

#endif
