#pragma once

#include "FaceBSP.h"
#include <string>

/**
@file
@author F. Aubert
@brief
*/

/**
@class NodeBSP
@brief
*/
class NodeBSP {
public:
  ~NodeBSP();
  NodeBSP();
  NodeBSP(const FaceBSP &f,NodeBSP *neg,NodeBSP *pos);

  void set(FaceBSP *f,NodeBSP *g,NodeBSP *d);
  void face(const FaceBSP &f);
  const FaceBSP &face() {return face_;}

  void negative(NodeBSP *g);
  void positive(NodeBSP *d);

  const FaceBSP &face() const;
  NodeBSP *negative() const;
  NodeBSP *positive() const;

  /// trace le BSP selon technique du peintre
  void draw(const p3d::Vector3 &obs);

  size_t nbFace();
private:
  /// les sous arbres
  NodeBSP *negative_{nullptr},*positive_{nullptr};
  /// la face pivot
  FaceBSP face_{};
};




