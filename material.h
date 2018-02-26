#ifndef MATERIAL_H
#define MATERIAL_H

namespace Raytracer {

class Material {
public:
	Material() : color(Color(0.2f, 0.2f, 0.2f)), refl(0), diff(0.2f), spec(0.8f), rindex(1.5f) {}

	void SetColor(const Color& _color) { color = _color; }
	void SetDiffuse(float _diff) { diff = _diff; }
	void SetReflection(float _refl) { refl = _refl; }
	void SetRefraction(float _refr) { refr = _refr; }
	void SetRefrIndex(float _rindex) { rindex = _rindex; }
	void SetSpecular(float _spec) { spec = _spec; }

	Color GetColor() { return color; }

	float GetDiffuse() { return diff; }
	float GetReflection() { return refl; }
	float GetRefraction() { return refr; }
	float GetRefrIndex() { return rindex; }
	float GetSpecular() { return spec; }

private:
	Color color;
	float diff, spec;
	float refl, refr;
	float rindex;
};

}

#endif
