#pragma once

#include "NodeBSP.h"

namespace p3d {
class Mesh;
}

class ObjectBSP {
public:
  ObjectBSP();
  virtual ~ObjectBSP();

  void set(const std::vector<FaceBSP> &face);
  void consBSP();
  static NodeBSP *consTree(const std::vector<FaceBSP> &face);
  void drawBSP(NodeBSP *tree,const p3d::Vector3 &p);

  void drawBSP(const p3d::Vector3 &eye);

  void drawDepth();
  void drawBrut();
  void drawGrid();
  void drawArrayFill();
  void drawArrayGrid();
  void drawNormal();
  void clearDraw();

  size_t nbFaceInit() const;
  size_t nbFaceBSP() const;

  void read(const std::string &name);

  NodeBSP *bsp() {return bsp_;}

  void addDrawFill(NodeBSP *n);
  void addDrawWire(NodeBSP *n);
  void addDraw(NodeBSP *n);
  void addDrawFill(const FaceBSP &f);
  void addDrawWire(const FaceBSP &f);
  void addDraw(const FaceBSP &f);
  void addDrawAllInit();
  void addDrawAllTree();

  const FaceBSP &face(size_t i) const {return allFace_[i];}


  std::vector<p3d::Vector3> drawPtsFill_{},drawPtsWire_{},drawNormalFill_{};

private:
  NodeBSP *consBSP(const std::vector<FaceBSP> &face);
  NodeBSP *bsp_{nullptr};

  std::vector<FaceBSP> allFace_;


};


