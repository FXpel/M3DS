#pragma once

#include "Intersection.h"
#include "MeshObject3D.h"

class SceneIntersection {

public:
  SceneIntersection() = default;

  /// number of intersections
  size_t size() {return result_.size();}
  /// delete all current intersections : should be called before the intersection
  void clear();

  /// intersection with mesh
  void intersect(p3d::MeshObject3D *e);

  /// intersection with all meshes (
  void intersect(const std::vector<p3d::MeshObject3D *> &allObj, const p3d::Line &pickingRay);

  /// returns the i-th intersection
  Intersection operator[](size_t i) const {return result_[i];}

  /// inserts intersection i in the result (sorted)
  void insert(const Intersection &inter);

  /// intersect the triangle s0,s1,s2 : ray should be expressed in the same frame
  IntersectResult intersect(const p3d::Line &ray, const p3d::Vector3 &s0, const p3d::Vector3 &s1, const p3d::Vector3 &s2);

private:
  p3d::Line pickingRay_{};
  std::vector<Intersection> result_{};

};



