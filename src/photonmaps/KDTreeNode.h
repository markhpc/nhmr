/*
 * KDTreeNode.h
 *
 *  Created on: Oct 31, 2011
 *      Author: nh
 */

#ifndef KDTREENODE_H_
#define KDTREENODE_H_
#include <vector>
#include "common/PhotonHit.h"

//#include <iostream>

class KDTreeNode {
  public:
  float splitPos;
  std::vector<common::PhotonHit*>* data;
  KDTreeNode* left;
  KDTreeNode* right;
  KDTreeNode();
  virtual ~KDTreeNode();
};

#endif /* KDTREENODE_H_ */
