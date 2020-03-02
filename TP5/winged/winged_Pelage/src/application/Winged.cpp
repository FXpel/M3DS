#include "Winged.h"
#include <iostream>
#include "WingedMap.h"
#include "GLRender.h"
#include "GLDraw.h"
#include "Tools.h"

#include "WVertex.h"
#include "WEdge.h"
#include "WFace.h"

/**
@file
@author F. Aubert
@brief  Winged-Edges
*/

using namespace std;
using namespace p3d;



/** ************************************************************************************
**   Winged::catmullClark()
** *********************************************************************************** */

void Winged::catmullClark() {
  computePointFace();
  computePointEdge();
  computePointVertex();

  // les coordonnées des sommets actuels deviennent celles des points de sommets.
  auto nb_vertex=nbVertex();
  for(size_t i=0;i<nb_vertex;++i) {
    vertex(i)->position(vertex(i)->pointVertex());
  }

  // insertion des sommets d'arêtes
  auto nb_edge=nbEdge();
  for(unsigned int i=0;i<nb_edge;i++) {
    edge(i)->insertVertexPointEdge();
  }

// /*
  // insertion des faces
  auto nb_face=nbFace();
  for(unsigned int i=0;i<nb_face;i++) {
    face(i)->insertVertexPointFace();
  }

  for(unsigned int i=0;i<nb_face;i++) {
    eraseFace(0);
  }
// */
}




/** ************************************************************************************ */


Winged::Winged() {
  //ctor
}

Winged::~Winged() {
  //dtor
  for(unsigned int i=0;i<nbVertex();i++) {
    delete vertex_[i];
  }
  for(unsigned int i=0;i<nbEdge();i++) {
    delete edge_[i];
  }
  for(unsigned int i=0;i<nbFace();i++) {
    delete face_[i];
  }
}

void Winged::read(string nom) {
    WingedMap transfert;
    transfert.read(nom,this);
    computeNormalFace();
}

size_t Winged::nbVertex() {
  return vertex_.size();
}

// WARNING : no check up to delete all edges/vertices that are not attached to any face after deletion (call deleteFace to do this).
void Winged::eraseFace(size_t i) {
  WFace *f=face(i);
  face_.erase(face_.begin()+int(i));
  delete f;
}

WVertex *Winged::createVertex(const Vector3 &p) {
  WVertex *res;
  res=new WVertex(p,this);
  vertex_.push_back(res);
  res->index(vertex_.size()-1);
  return res;
}

WFace *Winged::createFace() {
  WFace *res;
  res=new WFace(this);
  face_.push_back(res);
  return res;
}

WEdge *Winged::createEdge(WVertex *b,WVertex *e) {
  WEdge *res;
  res=new WEdge(b,e,this);
  edge_.push_back(res);
  return res;
}


WEdge *Winged::findEdge2(WVertex *v1,WVertex *v2) {
  unsigned int i=0;
  bool found=false;
  while (i<edge_.size() && !found) {
    if (((edge_[i]->begin()==v1) && (edge_[i]->end()==v2)) || ((edge_[i]->end()==v1) && (edge_[i]->begin()==v2)))
        found=true;
    else i++;
  }
  if (found) return edge_[i];
  else return nullptr;
}

size_t Winged::nbFace() {
  return face_.size();
}

size_t Winged::nbEdge() {
  return edge_.size();
}

void Winged::drawPointFace() {
  for(unsigned int i=0;i<nbFace();i++) {
    p3d::drawPoints({face(i)->pointFace()});
  }
}

void Winged::drawPointVertex() {
  for(unsigned int i=0;i<nbVertex();i++) {
    p3d::drawPoints({vertex(i)->pointVertex()});
  }
}


WFace *Winged::face(size_t i) {
  return face_[i];
}

WVertex *Winged::vertex(size_t i) {
  return vertex_[i];
}

WEdge *Winged::edge(size_t i) {
  return edge_[i];
}

void Winged::computeNormalFace() {
  for(unsigned int i=0;i<nbFace();i++) {
    face(i)->computeNormal();
  }
}

void Winged::drawLineCatmull() {
  for(unsigned int i=0;i<nbVertex();i++) {
    vertex(i)->drawLineCatmull();
  }
  for(unsigned int i=0;i<nbFace();i++) {
    face(i)->drawLineCatmull();
  }
}

void Winged::computeNormalVertex() {
  for(unsigned int i=0;i<nbVertex();i++) {
    vertex(i)->computeNormal();
  }
}

void Winged::computePointFace() {
  for(unsigned int i=0;i<nbFace();i++) {
    face(i)->computePointFace();
  }
}

void Winged::computePointEdge() {
  for(unsigned int i=0;i<nbEdge();i++) {
    edge(i)->computePointEdge();
  }
}

void Winged::computePointVertex() {
  for(unsigned int i=0;i<nbVertex();i++) {
    vertex(i)->computePointVertex();
  }
}


void Winged::initLineAttrib() {
  lineAttrib_.clear();
  for(WEdge *e:edge_) {
    lineAttrib_.push_back(e->begin()->position());
    lineAttrib_.push_back(e->end()->position());
  }
}

void Winged::draw(bool withNormal) {
  for(unsigned int i=0;i<nbFace();i++) {
    face(i)->draw(withNormal);
  }
}


void Winged::markAll() {
  for(unsigned int i=0;i<nbEdge();i++) {
    edge(i)->mark();
  }
}

vector<WEdge*> Winged::boundary() {
  vector<WEdge*> res;
  res.clear();
  unmarkAll();
  for(unsigned int i=0;i<nbEdge();i++) {
    if (!edge(i)->isMark()) {
      if ((!edge(i)->left()) || (!edge(i)->right())) {
        edge(i)->followBoundary();
        res.push_back(edge(i));
      }
  }
  }
  cout << "Boundaries found = " << res.size() << endl;
  return res;
}

void Winged::checkVertexBound() {
  for(unsigned int i=0;i<nbVertex();i++) {
    vertex(i)->checkVertexBound();
  }
}

void Winged::checkEdgeFree() {
  for(unsigned int i=0;i<nbEdge();i++) {
    if ((!edge(i)->left()) && (!edge(i)->right()))
        cout << "Arete Libre" << endl;
  }
}




void Winged::unmarkAll() {
  for(unsigned int i=0;i<nbEdge();i++) {
    edge(i)->unmark();
  }
}


void Winged::drawWire(const Vector4 &color) {
  p3d::ambientColor=color;
  p3d::shaderVertexAmbient();
  p3d::drawLines(lineAttrib_);
}

void Winged::drawEdgeBoundary() {
  for(unsigned int i=0;i<nbEdge();i++) {
    WEdge *e=edge(i);
    if ((!e->left()) || (!e->right())) {
      e->draw();
    }
  }
}

void Winged::drawPointEdge() {
  for(unsigned int i=0;i<nbEdge();i++) {
    p3d::drawPoints({edge(i)->pointEdge()});
  }
}


void Winged::index() {
  for(unsigned int i=0;i<nbVertex();i++) {
    vertex(i)->index(i);
  }
  for(unsigned int i=0;i<nbFace();i++) {
    face(i)->index(i);
  }
  for(unsigned int i=0;i<nbEdge();i++) {
    edge(i)->index(i);
  }
}

Winged *Winged::clone() {
  Winged *res;
  res=new Winged();
  for(unsigned int i=0;i<nbVertex();i++) {
    res->createVertex(vertex(i)->position());
  }
  for(unsigned int i=0;i<nbFace();i++) {
    res->createFace();
  }
  for(unsigned int i=0;i<nbEdge();i++) {
    res->createEdge(res->vertex(edge(i)->begin()->index()),res->vertex(edge(i)->end()->index()));
  }

  for(unsigned int i=0;i<nbVertex();i++) {
    res->vertex(i)->edge(res->edge(vertex(i)->edge()->index()));
  }

  for(unsigned int i=0;i<nbFace();i++) {
    res->face(i)->edge(res->edge(face(i)->edge()->index()));
  }

  for(unsigned int i=0;i<nbEdge();i++) {
    res->edge(i)->left(res->face(edge(i)->left()->index()));
    res->edge(i)->right(res->face(edge(i)->right()->index()));
    res->edge(i)->succRight(res->edge(edge(i)->succRight()->index()));
    res->edge(i)->succLeft(res->edge(edge(i)->succLeft()->index()));
    res->edge(i)->predRight(res->edge(edge(i)->predRight()->index()));
    res->edge(i)->predLeft(res->edge(edge(i)->predLeft()->index()));
  }

  return res;
}

void Winged::check() {
  cout << "checking..." << endl;
  for(unsigned int i=0;i<nbFace();i++) {
    cout << "Face =" << i << endl;
    if (face(i)->edge()==NULL) ErrorD("edge null dans face");
    WEdge *start=face(i)->edge();
    WEdge *e=start;
    do {
      if (e==NULL) throw ErrorD("Succession NULL");
      cout << "Edge = " << e << endl;
      if (e->left()==face(i)) {
        e=e->succLeft();
      }
      else {
        e=e->succRight();
      }
    } while (e!=start);
  }
  cout << "checked" << endl;
}


void Winged::drawNormal() {
    vector<Vector3> lines;
    for(unsigned int i=0;i<vertex_.size();++i) {
        lines.push_back(vertex_[i]->position());
        lines.push_back(vertex_[i]->normal()*0.2+vertex_[i]->position());
    }
    p3d::ambientColor=Vector4(1,0,0,1);
    p3d::shaderVertexAmbient();
    p3d::drawLines(lines);
}






void Winged::stats() {
  cout << "*******************" << endl;
  cout << "nbVertex = " << nbVertex() << endl;
  cout << "nbEdge = " << nbEdge() << endl;
  cout << "nbFace = " << nbFace() << endl;
  cout << "*******************" << endl;
}


