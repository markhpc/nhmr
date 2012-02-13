/*
 * Material.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Material.h"

Material::Material() {
  color = Color3f(0, 0, 0);
  reflection = 0;
  specular = 0;
  refraction = 0;
  rIndex = 1.0;
  diffusion = 0;
}

Material::Material(Color3f _color, float _reflection, float _specular, float _refraction, float _rIndex, float _diffusion) :
    color(_color),
    reflection(_reflection),
    specular(_specular),
    refraction(_refraction),
    rIndex(_rIndex),
    diffusion(_diffusion) {
}

Material::~Material() {
  // TODO Auto-generated destructor stub
}

