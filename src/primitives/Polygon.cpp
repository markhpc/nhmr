/*
 * Polygon.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Polygon.h"

//Polygon::Polygon(Vector3d inNorm, double inDist) : plane(Plane(inNorm, inDist)) {
Polygon::Polygon(std::vector<Vector3d> _vertices) : vertices(_vertices) {
  Vector3d normal(0, 0, 0);
  for (unsigned int i = 0; i < vertices.size(); i++) {
    Vector3d current = vertices[i];
    Vector3d next = vertices[(i+1) % vertices.size()];
    normal.x() += (current.y() - next.y()) * (current.z() + next.z());
    normal.y() += (current.z() - next.z()) * (current.x() + next.x());
    normal.z() += (current.x() - next.x()) * (current.y() + next.y());
  }
  normal.normalize();
//  plane(normal, 0);
  type = PLANE;
  light = false;
}

Vector3d Polygon::getNormal(const Vector3d& position) {
  return plane.normal;
}

int Polygon::intersect(Ray& ray, double& distance) {
  double dot = plane.normal.dot(ray.direction);
  if (dot != 0) {
    double newDistance = -(plane.normal.dot(ray.origin) + plane.distance) / dot;
    if (newDistance > 0 && newDistance < distance) {
      // We are in the plane at least
      Vector3d location = ray.origin + newDistance*ray.direction;
    }
  }
// Need to modify this to make it work
/*
      typedef struct {
         double x,y,z;
      } XYZ;
      #define EPSILON  0.0000001
      #define MODULUS(p) (sqrt(p.x*p.x + p.y*p.y + p.z*p.z))
      #define TWOPI 6.283185307179586476925287
      #define RTOD 57.2957795

      double CalcAngleSum(XYZ q,XYZ *p,int n)
      {
         int i;
         double m1,m2;
         double anglesum=0,costheta;
         XYZ p1,p2;

         for (i=0;i<n;i++) {

            p1.x = p[i].x - q.x;
            p1.y = p[i].y - q.y;
            p1.z = p[i].z - q.z;
            p2.x = p[(i+1)%n].x - q.x;
            p2.y = p[(i+1)%n].y - q.y;
            p2.z = p[(i+1)%n].z - q.z;

            m1 = MODULUS(p1);
            m2 = MODULUS(p2);
            if (m1*m2 <= EPSILON)
               return(TWOPI); // We are on a node, consider this inside
            else
               costheta = (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z) / (m1*m2);

            anglesum += acos(costheta);
         }
         return(anglesum);
      }
*/

//      distance = newDistance;
//      return HIT;
//    }
//  }
//  return MISS;
}

Polygon::~Polygon() {
  // TODO Auto-generated destructor stub
}

