#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer.h"
#include "texture.h"

namespace Raytracer {

class Material {
public:
	Material() : color(Color(0.7f, 0.2f, 0.2f)), refr(0.8f), refl(0.6f),
				 diff(0.2f), spec(0.8f), rindex(1.5f) { texture = NULL; }
	Material(const Color &color, const Color &absorb, float refr, float refl,
			 float diff, float spec, float rindex, Texture *texture = NULL)
			 : color(color), absorb(absorb), refr(refr), refl(refl),
			   diff(diff), spec(spec), rindex(rindex), texture(texture) {
			   }

	void SetColor(const Color& _color) { color = _color; }
	void SetAbsorb(const Color& _absorb) { absorb = _absorb; }
	void SetDiffuse(float _diff) { diff = _diff; }
	void SetReflection(float _refl) { refl = _refl; }
	void SetRefraction(float _refr) { refr = _refr; }
	void SetRefrIndex(float _rindex) { rindex = _rindex; }
	void SetSpecular(float _spec) { spec = _spec; }

	Color GetColor() const { return color; } //TODO : and warning
	Color GetColor(const pair<float, float> &p) const {  // TODO: modify pair<int,int> to Point2
		if (texture == NULL) return color;
		return texture -> GetColor(p);
	}
	Color GetAbsorb() const { return absorb; }
	float GetDiffuse() const { return diff; }
	float GetReflection() const { return refl; }
	float GetRefraction() const { return refr; }
	float GetRefrIndex() const { return rindex; }
	float GetSpecular() const { return spec; }

private:
	Color color;
	Color absorb;
	float refl;
	float refr;
	float diff;
	float spec;
	float rindex;
	Texture *texture;
};

}

#endif
