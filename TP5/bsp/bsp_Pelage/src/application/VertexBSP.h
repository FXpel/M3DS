#pragma once

#include "Vector3.h"

class ObjectBSP;

class VertexBSP {
public:
  VertexBSP(const p3d::Vector3 &p,const p3d::Vector3 &n);
  VertexBSP() : position_{},normal_{} {}

  /// useless getters/setters : only for convenience
  p3d::Vector3 position() const {return position_;}
  p3d::Vector3 normal() const {return normal_;}
  void position(const p3d::Vector3 &p) {position_=p;}
  void normal(const p3d::Vector3 &n) {normal_=n;}

  p3d::Vector3 position_;
  p3d::Vector3 normal_;
  void interpolateNormal(const VertexBSP &v1, const VertexBSP &v2);
};



