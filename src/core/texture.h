#ifndef TEXTURE_H
#define TEXTURE_H

#include "raytracer.h"

namespace Raytracer {

class Texture {
public:
    Texture() {}
    Texture(const char* file) {
        fin.open(file);
        fin >> width >> height;
        data = new Color*[width];
        for(int i = 0; i < width; i++)
            data[i] = new Color[height];
        for(int i = 0; i < width; i++)
            for(int j = 0; j < height; j++) {
                fin >> data[i][j].r >> data[i][j].g >> data[i][j].b;
                data[i][j].r /= 255.;
                data[i][j].g /= 255.;
                data[i][j].b /= 255.;
            }
    }

    ~Texture() {
    }

    Color GetColor(const pair<float, float> &p) const {
        int x = max(min(int(p.first * width), width - 1), 1);
        int y = max(min(int(p.second * height), height - 1), 1);
        return data[x][y];
    }

private:
    int width;
    int height;
    Color** data;
    ifstream fin;
};

};

#endif
