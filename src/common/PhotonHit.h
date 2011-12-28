/*
 * common::PhotonHit.h
 *
 *  Created on: Aug 23, 2011
 *      Author: nh
 */

#ifndef PHOTONHIT_H_
#define PHOTONHIT_H_
#include <Eigen/Dense>
#include "primitives/Primitive.h"
#include "common/Color3f.h"
namespace common {
  class PhotonHit {
    public:
      Vector3d location;
      Color3f color;
      Primitive& primitive;
      PhotonHit(Vector3d _location, Color3f _color, Primitive& _primitive);
      double x() const;
      double y() const;
      double z() const;
      virtual ~PhotonHit();
  };
  bool xComp(const common::PhotonHit* a, const common::PhotonHit* b);
  bool yComp(const common::PhotonHit* a, const common::PhotonHit* b);
  bool zComp(const common::PhotonHit* a, const common::PhotonHit* b);

}
#endif /* PHOTONHIT_H_ */
