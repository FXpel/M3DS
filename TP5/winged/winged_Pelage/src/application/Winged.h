#pragma once

/**
@file
@author F. Aubert
@brief  Winged-Edges : stockage et construteurs des entités d'un winged + opérations sur l'objet

*/

#include <vector>
#include <string>


class WVertex;
class WFace;
class WEdge;

namespace p3d {
class Vector3;
class Vector4;
}

class Winged
{
  public:
    Winged();
    virtual ~Winged();

    void read(std::string nom);

    size_t nbVertex();
    size_t nbFace();
    size_t nbEdge();

    void eraseFace(size_t i);
    void deleteFace(size_t i);

    WVertex *createVertex(const p3d::Vector3 &p);
    WFace *createFace();
    WEdge *createEdge(WVertex *v1,WVertex *v2);

    Winged *clone();

    WEdge *findEdge2(WVertex *v1,WVertex *v2);

    WVertex *vertex(size_t i);
    WFace *face(size_t i);
    WEdge *edge(size_t i);

    void index();

    void stats();


    void computeNormalFace();
    void computeNormalVertex();


    // methods for catmull-clark subdivision scheme
    void computePointFace();
    void computePointEdge();
    void computePointVertex();
    void catmullClark();

    void check();
    void checkVertexBound();
    void checkEdgeFree();

    void initLineAttrib();

    void draw(bool withNormal);
    void drawEdgeBoundary();
    void drawPointFace();
    void drawPointEdge();
    void drawPointVertex();
    void drawWire(const p3d::Vector4 &color);
    void drawEdgeBound(const p3d::Vector4 &color);
    void drawLineCatmull();
    void drawNormal();

    void markAll();
    void unmarkAll();

    std::vector<WEdge *> boundary();

  protected:
  private:
  std::vector<WEdge *> edge_;
  std::vector<WFace *> face_;
  std::vector<WVertex *> vertex_;

  std::vector<p3d::Vector3> lineAttrib_;
};


