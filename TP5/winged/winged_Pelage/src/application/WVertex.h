#ifndef WVERTEX_H
#define WVERTEX_H

class WVertex;

/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/

#include "Vector3.h"

class WEdge;
class Winged;


class WVertex
{
  public:
    virtual ~WVertex();

    WVertex(const p3d::Vector3 &p,Winged *owner);

    Winged *owner();

    WEdge *edge();
    void edge(WEdge *e);

    const p3d::Vector3 &normal() const;
    void normal(const p3d::Vector3 &n);

    const p3d::Vector3 &position() const;
    void position(const p3d::Vector3 &p);

    size_t index();
    void index(size_t i);

    WEdge *indirectBoundary();
    WEdge *directBoundary();

    void computeNormal();
    void computePointVertex();

    const p3d::Vector3 &pointVertex() const;

    void drawLineCatmull();

    void checkVertexBound();

  protected:
  private:
  p3d::Vector3 position_;
  p3d::Vector3 normal_;

  p3d::Vector3 catmullPoint_;

  WEdge *edge_;

  Winged *owner_;
  size_t index_;
};

#endif // WVERTEX_H

