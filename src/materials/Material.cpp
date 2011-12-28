/*
 * Material.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Material.h"

Material::Material() {
  color = Color3f(0.2, 0.2, 0.2);
  reflection = 0;
  refraction = 0;
  rIndex = 1.5f;
  diffusion = 0.2;
}

float Material::specular() {
  return 1 - diffusion;
}
Material::~Material() {
  // TODO Auto-generated destructor stub
}

