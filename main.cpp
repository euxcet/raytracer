#include "raytracer.h"

int main() {
	Raytracer::Engine* engine;
	engine = new Raytracer::Engine();
	engine -> SetTarget(800, 600);
	engine -> Render();
}
