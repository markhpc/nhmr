/*
 * Scene.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Scene.h"

Scene::Scene() {
  Primitive* primitive;
/*
  // Ground Plane
  primitive = new PlanePrim(Vector3d(0, 1, 0), 4.4);
  primitive->name = "plane";
  primitive->material.reflection = 0;
  primitive->material.refraction = 0;
  primitive->material.diffusion = 1;
  primitive->material.color = Color3f(0.4, 0.3, 0.3);
  primitives.push_back(primitive);

  // Big Sphere
  primitive = new Sphere(Vector3d(2, 0.8, 3), 2.5);
  primitive->name = "big sphere";
  primitive->material.reflection = 0.2;
  primitive->material.refraction = 0.8;
  primitive->material.rIndex = 1.3;
  primitive->material.color = Color3f(0.7, 0.7, 1.0);
  primitives.push_back(primitive);

  // Small Sphere
  primitive = new Sphere(Vector3d(-5.5, -0.5, 7), 2);
  primitive->name = "small sphere";
  primitive->material.reflection = 0.5;
  primitive->material.refraction = 0.0;
  primitive->material.rIndex = 1.3;
  primitive->material.diffusion = 0.1;
  primitive->material.color = Color3f(0.7, 0.7, 1.0);
  primitives.push_back(primitive);

  // light source 1
  primitive = new Sphere(Vector3d(0, 5, 5), 0.1);
  primitive->name = "light 1";
  primitive->light = true;
  primitive->material.color = Color3f(0.4, 0.4, 0.4);
//  primitive->material.color = Color3f(1.0, 1.0, 1.0);
  primitives.push_back(primitive);
  lights.push_back(primitive);

  // light source 2
  primitive = new Sphere(Vector3d(-3, 5, 1), 0.1);
  primitive->light = true;
  primitive->material.color = Color3f(0.6, 0.6, 0.8);
  primitives.push_back(primitive);
  lights.push_back(primitive);

  // extra Sphere
  primitive = new Sphere(Vector3d(-1.5, -3.8, 1), 1.5);
  primitive->name = "extra sphere";
  primitive->material.reflection = 0.0;
  primitive->material.refraction = 0.8;
  primitive->material.color = Color3f(0.7, 0.7, 1.0);
  primitives.push_back(primitive);

  // back plane
  primitive = new PlanePrim(Vector3d(0.4, 0, -1), 12);
  primitive->name = "back plane";
  primitive->material.reflection = 0.0;
  primitive->material.refraction = 0.0;
  primitive->material.diffusion = 1.0;
  primitive->material.color = Color3f(0.5f, 0.3f, 0.5f);
  primitives.push_back(primitive);

  // ceiling plane
  primitive = new PlanePrim(Vector3d(0, -1, 0), 7.4f);
  primitive->name = "ceiling plane";
  primitive->material.reflection = 0.0;
  primitive->material.refraction = 0.0;
  primitive->material.diffusion = 1.0;
  primitive->material.color = Color3f(0.4f, 0.7f, 0.7f);
  primitives.push_back(primitive);

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 7; y++) {
      primitive = new Sphere(Vector3d(-4.5 + x * 1.5, -4.3 + y*1.5, 10), 0.3f);
      primitive->name = "grid sphere";
      primitive->material.reflection = 0;
      primitive->material.refraction = 0;
      primitive->material.diffusion = 0.6;
      primitive->material.color = Color3f(0.3, 1.0, 0.4);
      primitives.push_back(primitive);
    }
  }
*/

  primitive = new Sphere(Vector3d(-1e5, 40.8, 81.6), 1e5);
  primitive->name = "left wall";
  primitive->material.color = Color3f(.75, .25, .25);
//  primitive->material.color = Color3f(0, 0, 0);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(1e5+100, 40.8, 81.6), 1e5);
  primitive->name = "right wall";
//  primitive->material.color = Color3f(0, 0, 0);
  primitive->material.color = Color3f(.25, .25, .75);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(50, 40.8, 1e5+170), 1e5);
  primitive->name = "back wall";
  primitive->material.color = Color3f(1, .95, .9);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(50, 40.8, -1e5-10), 1e5);
  primitive->name = "front wall";
  primitive->material.color = Color3f(0, 0, 0);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(50, -1e5, 81.6), 1e5);
  primitive->name = "floor";
  primitive->material.color = Color3f(1, .95, .9);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(50, 1e5+81.6, 81.6), 1e5);
  primitive->name = "ceiling";
  primitive->material.color = Color3f(1, .95, .9);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

//  primitive = new Sphere(Vector3d(50, 681.6-.27, 81.6), 600);
//  primitive->name = "light";
//  primitive->light = true;
//  primitive->material.color = Color3f(12, 12, 12);
//  primitives.push_back(primitive);
//  lights.push_back(primitive);

  primitive = new Sphere(Vector3d(50, 83.6, 81.6), 6);
  primitive->name = "light";
  primitive->light = true;
  primitive->material.color = Color3f(5000, 5000, 5000);
//  primitives.push_back(primitive);
  lights.push_back(primitive);

  primitive = new Sphere(Vector3d(26, 12, 100.6), 12);
  primitive->name = "ball1";
  primitive->material.color = Color3f(.75, .75, .75);
  primitive->material.diffusion = 0.0;
  primitive->material.reflection = 1.0;
  primitives.push_back(primitive);
  primitive = new Sphere(Vector3d(50, 12, 100.6), 12);
  primitive->name = "ball2";
  primitive->material.color = Color3f(.25, .75, .25);
  primitive->material.diffusion = 1.0;
  primitives.push_back(primitive);

  primitive = new Sphere(Vector3d(74, 12, 100.6), 12);
  primitive->name = "ball3";
  primitive->material.color = Color3f(.75, .75, .75);
  primitive->material.diffusion = 0.0;
  primitive->material.refraction = 1.0;
  primitives.push_back(primitive);

//  primitive = new Sphere(Vector3d(0, 0, 0), 5);
//  primitive->name = "light";
//  primitive->light = true;
//  primitive->material.color = Color3f(12, 12, 12);
//  primitives.push_back(primitive);
/*
  for (int i = 0; i < 500; i++) {
    Vector3d position = getHaltonPointOnHalfSphere(i, 12);
    position = rotateVector(position, Vector3d(0, 0, 1), Vector3d(1,0,0));
    primitive = new Sphere(position + Vector3d(74, 50, 81.6), .5);
    primitive->name = "Sphere";
    primitive->material.color = Color3f(.25, .25, .75);
    primitive->material.diffusion = 1.0;
    primitives.push_back(primitive);
  }
  */
}

Vector3d Scene::rotateVector(Vector3d vector, Vector3d normal1, Vector3d normal2) {
  Quaterniond q;
  q.setFromTwoVectors(normal1, normal2);
  Vector3d newVector = q * vector;
  return newVector;
}

Vector3d Scene::getRandomPointOnHalfSphere(double radius) {
  double mult = 2.0 / RAND_MAX;
  double theta = rand()*mult*3.1415;
  double z = rand() / (float) RAND_MAX;
  return getPointOnSphere(radius, theta, z);
}

Vector3d Scene::getHaltonPointOnHalfSphere(int id, double radius) {
  double theta = 2.0 * 3.1415 * halton(id, 2);
  double z = 1.0 * halton(id, 3);
  return getPointOnSphere(radius, theta, z);
}

Vector3d Scene::getPointOnSphere(double radius, double theta, double z) {
  double y = sqrt(1 - z*z) * sin(theta);
  double x = sqrt(1 - z*z) * cos(theta);
  Vector3d direction = Vector3d(x, y, z);
  direction.normalize();
  return direction * radius;
}

float Scene::halton(int id, int prime) {
  float h = 0.0;
  float f = 1.0/prime;
  float fct = 1.0;
  while(id>0)
  {
    fct *= f;
    h += (id%prime)*fct;
    id /= prime;
  }
  return h;
}

Scene::~Scene() {
  // TODO Auto-generated destructor stub
}

