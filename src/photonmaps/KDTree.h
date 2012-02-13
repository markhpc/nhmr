/*
 * KDTree.h
 *
 *  Created on: Sep 12, 2011
 *      Author: nh
 */

#ifndef KDTREE_H_
#define KDTREE_H_
#include <vector>
#include <iostream>
#include <cmath>
#include <tr1/functional>
#include "common/Settings.h"
#include "common/PhotonHit.h"
#include "PhotonMap.h"
#include "KDTreeNode.h"

class KDTree: public PhotonMap {
  public:
  bool(*compArray[3])(const common::PhotonHit* a, const common::PhotonHit* b);

  int size;
  std::vector<common::PhotonHit*> hits;
  KDTreeNode* root;

  KDTree(double _photonHitRadius);
  virtual ~KDTree();
  void initDraw();
  void add(common::PhotonHit* photonHit);
  void drawHit(HitPoint* hitPoint, Color3f& color);

  private:
  void _buildTree(KDTreeNode* node, std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth);
  void _searchTree(KDTreeNode* node, HitPoint* hitPoint, Color3f& color, int depth);
  int _getSplitIndexMedian(std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth);
  int _getSplitIndexSAH(std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth);
};

#endif /* KDTREE_H_ */
