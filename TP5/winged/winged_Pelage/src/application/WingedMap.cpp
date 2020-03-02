#include "WingedMap.h"
#include "Tools.h"

#include "WVertex.h"
#include "WEdge.h"
#include "WFace.h"


/**
@file
@author F. Aubert

*/


using namespace std;
using namespace p3d;


WingedMap::WingedMap() {
  //ctor
}

WingedMap::~WingedMap() {
  //dtor
}

WVertex *WingedMap::wvertex(unsigned int i) {
  return wvertex_[i];
}

WEdge *WingedMap::findEdge(WVertex *v1,WVertex *v2) {
  bool found=false;
  unsigned int i,j;
  i=0;
  while ((i<vertexEdge_[v1->index()].size()) && !found) {
    j=0;
    while ((j<vertexEdge_[v2->index()].size()) && !found) {
      if (vertexEdge_[v1->index()][i]==vertexEdge_[v2->index()][j])
        found=true;
      else
        j++;
    }

    if (!found) i++;
  }

  if (found) return vertexEdge_[v1->index()][i]; else return NULL;
}


void WingedMap::read(const string &filename, Winged *winged) {
  wvertex_.clear();
  vertexEdge_.clear();
  mesh_=new Mesh();
  mesh_->read(filename);
  mesh_->computeNormal();
  mesh_->scaleInBoxRatio(-2,2,-2,2,-2,2);
  WEdge *we;
  WFace *wf;
  WVertex *wv_begin,*wv_end;

  vertexEdge_.resize(mesh_->nbPosition());
  for(unsigned int i=0;i<mesh_->nbPosition();i++) {
    vertexEdge_[i].clear();
    wvertex_.push_back(winged->createVertex(mesh_->positionMesh(i)));
  }

  for(unsigned int i=0;i<mesh_->nbFace();i++) {
    wf=winged->createFace();
    wf->normal(mesh_->normalFace(i));
    for(unsigned int j=0;j<mesh_->nbVertexFace(i);j++) {
      // On cherche et on crée éventuellement un nouveau sommet
      unsigned int v=mesh_->indexPositionVertexFace(i,j);

      wv_begin=wvertex_[v];


      // On cherche et on crée éventuellement une nouvelle arête
      unsigned int next=mesh_->indexPositionVertexFace(i,j+1);
      wv_end=wvertex_[next];

      if (wv_begin!=wv_end) {


      we=this->findEdge(wv_begin,wv_end);
      if (we) {
        if (we->begin()==wv_begin) {
          if ((we->begin()!=wv_begin) || (we->end()!=wv_end)) throw ErrorD("Edge found incoherent");
          if (we->left()!=NULL) throw ErrorD("left already set");
          we->left(wf);
        }
        else {
          if ((we->end()!=wv_begin) || (we->begin()!=wv_end)) throw ErrorD("Edge found incoherent");
          if (we->right()!=NULL) throw ErrorD("right already set");
          we->right(wf);
        }
      }
      else {
        we=winged->createEdge(wv_begin,wv_end);
        vertexEdge_[wv_begin->index()].push_back(we);
        vertexEdge_[wv_end->index()].push_back(we);
        we->left(wf);
      }

      wv_begin->edge(we);
      wv_end->edge(we);
      }
      else
        throw ErrorD("bug"); // well, hope never happens cause I dont remember why
    }
    wf->edge(we);

    unsigned int pred,succ,current0,current1;
    current0=mesh_->indexPositionVertexFace(i,0);
    current1=mesh_->indexPositionVertexFace(i,1);
    pred=mesh_->indexPositionVertexFace(i,-1);
    succ=mesh_->indexPositionVertexFace(i,2);
    WEdge *currente,*prede,*succe;
    currente=this->findEdge(wvertex_[current0],wvertex_[current1]);
    prede=this->findEdge(wvertex_[pred],wvertex_[current0]);
    succe=this->findEdge(wvertex_[current1],wvertex_[succ]);

    for(unsigned int j=0;j<mesh_->nbVertexFace(i);j++) {
      if ((currente==succe) || (currente==prede))
        throw ErrorD("ouhla "); // means restart all the class from scratch
      if (currente->left()==wf) {
          currente->succLeft(succe);
          currente->predLeft(prede);
      }
      else {
        if (currente->right()!=wf) throw ErrorD("No face for the edge !"); // sure ?
        currente->succRight(succe);
        currente->predRight(prede);
      }
      pred=current0;
      current0=current1;
      current1=succ;
      succ=mesh_->indexPositionVertexFace(i,j+3);
      prede=currente;
      currente=succe;
      succe=this->findEdge(wvertex_[current1],wvertex_[succ]);
      if (!succe) throw ErrorD("No Succ"); // no luck
    }


  }
  delete mesh_;
}

