/*
 * Color3f.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Color3f.h"

Color3f::Color3f() {
  r = 0;
  g = 0;
  b = 0;
}

Color3f::Color3f(float comp1, float comp2, float comp3) {
  r = comp1;
  g = comp2;
  b = comp3;
}

Color3f::~Color3f() {
  // TODO Auto-generated destructor stub
}

void Color3f::ceiling() {
  if (r > 1) r = 1;
  if (g > 1) g = 1;
  if (b > 1) b = 1;
}

bool Color3f::operator == (const Color3f& op1) const {
  if (r == op1.r && g == op1.g && b == op1.b) return true;
  return false;
}

Color3f& Color3f::operator *= (float op1) {
  r *= op1;
  g *= op1;
  b *= op1;
  return *this;
}

Color3f& Color3f::operator *= (const Color3f& op1) {
  r *= op1.r;
  g *= op1.g;
  b *= op1.b;
  return *this;
}

Color3f& Color3f::operator += (const Color3f& op1) {
  r += op1.r;
  g += op1.g;
  b += op1.b;
  return *this;
}

Color3f& Color3f::operator -= (const Color3f& op1) {
  r -= op1.r;
  g -= op1.g;
  b -= op1.b;
  return *this;
}

const Color3f Color3f::operator+(const Color3f& op1) const {
  return Color3f(*this) += op1;
}

const Color3f Color3f::operator-(const Color3f& op1) const {
  return Color3f(*this) -= op1;
}

const Color3f Color3f::operator*(const Color3f& op1) const {
  return Color3f(*this) *= op1;
}

const Color3f Color3f::operator*(float op1) const {
  return Color3f(*this) *= op1;
}

const Color3f operator*(float op1, const Color3f& op2) {
  return op2*op1;
}
