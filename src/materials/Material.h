/*
 * Material.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "common/Color3f.h"

class Material {
  public:
  Color3f color;
  float reflection, specular, refraction, diffusion, rIndex;

  Material();
  Material(Color3f _color, float _reflection, float _specular, float _refraction, float _rIndex, float _diffusion);
  virtual ~Material();
};

#endif /* MATERIAL_H_ */
