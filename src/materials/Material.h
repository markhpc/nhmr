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
  float reflection, refraction, diffusion, rIndex;

  Material();
  float specular();
  virtual ~Material();
};

#endif /* MATERIAL_H_ */
