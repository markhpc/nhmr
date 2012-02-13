/*
 * KDTree.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: nh
 */

#include "KDTree.h"

KDTree::KDTree(double _photonHitRadius) :
    PhotonMap(_photonHitRadius),
    size(Settings::instance()->width * Settings::instance()->height),
    root(new KDTreeNode()){
  compArray[0] = &common::xComp;
  compArray[1] = &common::yComp;
  compArray[2] = &common::zComp;
}

KDTree::~KDTree() {
  delete root;
  for (std::vector<common::PhotonHit*>::iterator it = hits.begin(); it != hits.end(); ++it) {
    delete *it;
  }
}

void KDTree::initDraw() {
  _buildTree(root, hits.begin(), hits.end(), 0);
}

void KDTree::_buildTree(KDTreeNode* node, std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth) {
  if (!node) {
    std::cout << "node not present.\n";
    return;
  }
  if (depth > end - begin) {
    node->data = new std::vector<common::PhotonHit*>(begin, end);
    return;
  }

  sort(begin, end, compArray[depth%3]);
  int splitIndex = _getSplitIndexMedian(begin, end, depth);
  node->splitPos = (*(begin + splitIndex))->location[depth%3];
  node->left = new KDTreeNode();
  node->right = new KDTreeNode();
  _buildTree(node->left, begin, begin + splitIndex, depth + 1);
  _buildTree(node->right, begin + splitIndex, end, depth + 1);
}

int KDTree::_getSplitIndexMedian(std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth) {
  return (end - begin) / 2;
}

int KDTree::_getSplitIndexSAH(std::vector<common::PhotonHit*>::iterator begin, std::vector<common::PhotonHit*>::iterator end, int depth) {
  double min = (*begin)->location[depth%3];
  double max = (*(end-1))->location[depth%3];
  double ratio = 10e6;
  int splitPos = 0;
  for (std::vector<common::PhotonHit*>::iterator iter = begin+1; iter != end; ++iter) {
    double current = (*iter)->location[depth%3];
    double left = (current - min) * (iter - begin);
    double right = (max - current) * (end - iter);
    double result = 1+3*(left + right) / (end - begin);
    if (result < ratio) {
      splitPos = iter - begin;
      ratio = result;
    }
  }
  return splitPos;
}

void KDTree::add(common::PhotonHit* photonHit) {
  hits.push_back(photonHit);
}

void KDTree::drawHit(HitPoint* hitPoint, Color3f& color) {
  _searchTree(root, hitPoint, color, 0);
}

void KDTree::_searchTree(KDTreeNode* node, HitPoint* hitPoint, Color3f& color, int depth) {
  if (node->data) {
    color += getColor(node->data->begin(), node->data->end(), hitPoint);
    return;
  }
  double position = hitPoint->location[depth%3];
  if (position - photonHitRadius < node->splitPos) {
    _searchTree(node->left, hitPoint, color, depth + 1);
  }
  if (position + photonHitRadius > node->splitPos) {
    _searchTree(node->right, hitPoint, color, depth + 1);
  }
}
