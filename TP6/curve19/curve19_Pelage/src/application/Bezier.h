#pragma once

#include "Curve.h"

class Bezier : public Curve
{
public:
  Bezier();
  /// De Casteljau evaluation
  p3d::Vector3 eval(double t);
  /// Cubic evaluation (brut force; precond : nbPoint()==4)
  p3d::Vector3 evalCubic(double t);

  p3d::Vector3 evalCubicVelocity(double t);
  p3d::Vector3 evalCubicAcceleration(double t);

  p3d::Matrix4 tbn(double t);
  p3d::Vector3 rN(double t);

  void draw();
};


