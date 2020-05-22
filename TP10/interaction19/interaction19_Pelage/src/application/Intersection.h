#pragma once

#include <vector>
#include "Line.h"

#include "MeshObject3D.h"

class IntersectResult {
public:
  bool isIntersect_{false};
  double lambda_{0.0};
};


class Intersection {
public:
  Intersection() = default;
  void mesh(p3d::MeshObject3D *e) {mesh_=e;}
  void lambda(double a) {lambda_=a;}
  void rayWorld(const p3d::Line &ray) {rayWorld_=ray;}
  const p3d::Line &rayWorld() const {return rayWorld_;}
  double lambda() const {return lambda_;}
  p3d::MeshObject3D *mesh() const {return mesh_;}
private:
  p3d::MeshObject3D *mesh_;
  double lambda_;
  p3d::Line rayWorld_;

};

