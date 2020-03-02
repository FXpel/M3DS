#pragma once

/**
@file
@author F. Aubert
@brief Vector3 operations (x,y,z)

*/

#include <map>
#include "Mesh.h"
#include "Winged.h"

class VertexMap {
  size_t index_;
};



class WingedMap
{
  public:
    WingedMap();
    virtual ~WingedMap();

    void read(const std::string &filename,Winged *winged);

    WVertex *wvertex(unsigned int i);

    WEdge *findEdge(WVertex *v1,WVertex *v2);

  protected:
  private:
  std::vector<WVertex *> wvertex_;

  std::vector<std::vector<WEdge *> > vertexEdge_;

  p3d::Mesh *mesh_;
};


