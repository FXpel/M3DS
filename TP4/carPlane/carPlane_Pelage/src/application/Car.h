#pragma once

/**
  @author F. Aubert
  **/

#include "GLTool.h"
#include "Vector3.h"
#include "Quaternion.h"

class Car {
public:
  virtual ~Car();
  Car();

  p3d::Vector3 position();
  p3d::Quaternion orientation();

  void drawWorld();

  void draw();

  void drawRim();
  void drawWheel();
  void drawAxle();
  void drawBody();

  void rotateWheel(double angle);

  void accelerate();
  void decelerate();
  void brake();
  void turnLeft();
  void turnRight();
  void move();

  void drawCube();
  void drawTorus();
  void drawSphere();
  void drawCone();
  void drawCylinder();

private:
  p3d::Vector3 position_{p3d::Vector3(0,0,0)};
  p3d::Quaternion orientation_{p3d::Quaternion::identity()};

  double velocity_{0.0};
  double acceleration_{0.0};

  double rotateWheel_{0.0}; // rotation roue
  double steering_{0.0}; // rotation/essieu

};


