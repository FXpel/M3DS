#pragma once

#include "Quaternion.h"
#include "Vector3.h"
#include "Mesh.h"
#include "MeshGL.h"
#include <string>


class Airplane {
public:
  Airplane();
  virtual ~Airplane();
  void initDraw();
  void drawWorld();
  void draw();
  void read(const std::string &filename);

  inline const p3d::Vector3 &position() const {return position_;}
  inline const p3d::Quaternion &orientation() const {return orientation_;}

  void move();
  void pitchUp();
  void pitchDown();
  void rollLeft();
  void rollRight();
  void yawLeft();
  void yawRight();
  void accelerate();
  void decelerate();


  void position(const p3d::Vector3 &position) {position_=position;}
  void position(double x,double y, double z) {position(p3d::Vector3(x,y,z));}

private:
  double angleX_{0},angleY_{0},angleZ_{0};
  p3d::Quaternion orientation_{p3d::Quaternion::identity()};

  p3d::Vector3 position_{p3d::Vector3(0,0,0)};

  double increment_{0},accelerate_{0},velocity_{0};

  p3d::Mesh obj_{};
  p3d::MeshGL *objGL_{nullptr};


};


