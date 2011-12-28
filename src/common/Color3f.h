/*
 * Color3f.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef COLOR3F_H_
#define COLOR3F_H_

class Color3f {
  public:
  float r, g, b;
  Color3f();
  Color3f(float comp1, float comp2, float comp3);
  virtual ~Color3f();
  void ceiling();
  bool operator == (const Color3f& op1) const;
  Color3f& operator- ();
  Color3f& operator += (const Color3f& op1);
  Color3f& operator -= (const Color3f& op1);
  Color3f& operator *= (const Color3f& op1);
  Color3f& operator *= (float op1);
  const Color3f operator+(const Color3f& op1) const;
  const Color3f operator-(const Color3f& op1) const;
  const Color3f operator*(const Color3f& op1) const;
  const Color3f operator*(float op1) const;
  friend const Color3f operator*(float op1, const Color3f& op2);
};

#endif /* COLOR3F_H_ */
