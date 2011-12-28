/*
 * KDTreeNode.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: nh
 */

#include "KDTreeNode.h"

KDTreeNode::KDTreeNode() : data(0), left(0), right(0) {
  // TODO Auto-generated constructor stub
}

KDTreeNode::~KDTreeNode() {
  delete right;
  delete left;
  delete data;
}

