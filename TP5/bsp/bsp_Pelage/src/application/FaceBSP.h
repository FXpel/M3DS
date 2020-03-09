#pragma once

#include <vector>
#include "VertexBSP.h"
#include "Vector3.h"

/**
@file
@author Fabrice Aubert
*/

/**
@class Représentation d'une face dans un bsp
*/

class ObjectBSP;

typedef enum {Sign_Plus,Sign_Minus,Sign_None} ESign;


class FaceBSP {

public:
  ~FaceBSP();
  FaceBSP();

  /// normale :
  p3d::Vector3 normal() const;
  void normal(const p3d::Vector3 &n);

	/// donne le signe du point p par rapport à la facette
	ESign sign(const p3d::Vector3 &p) const;



  /// donne le point d'intersection entre la droite (v1v2) et le plan de la face this
  VertexBSP intersection(const VertexBSP &v1,const VertexBSP &v2) const;
	/// separe this selon f
  void separe(const FaceBSP &f,FaceBSP &fPositive,FaceBSP &fNegative) const;

  size_t size() const {return vertex_.size();}
  bool empty() const {return size()==0;}

  void add(const VertexBSP &v);

	void draw();

  void addDrawWire();
  void addDrawFill();
  void addDraw();
  void clear();

  p3d::Vector3 position(size_t i) const {return vertex_[i].position_;}
  p3d::Vector3 normal(size_t i) const {return vertex_[i].normal_;}
  const VertexBSP &vertex(size_t i) const {return vertex_[i];}
  size_t nbVertex() const {return vertex_.size();}

private:
  std::vector<VertexBSP> vertex_;
  p3d::Vector3 normal_;
};


