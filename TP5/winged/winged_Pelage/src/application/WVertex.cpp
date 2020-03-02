#include "WVertex.h"
#include "Tools.h"
#include "GLDraw.h"

#include "WEdge.h"
#include "Winged.h"
#include "WFace.h"


/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/


using namespace p3d;
using namespace std;



/** ************************************************************************************
** A COMPLETER : WVertex::computeNormal() => calculer la moyenne des normales aux faces incidentes au sommet. Cette normale est affectée au sommet à la fin
** On parcourt toutes les faces du sommet dans l'ordre direct
** - this->edge() : donne l'arête affectée à la face (début du parcours)
** - si e de type WEdge * :
**    - e->begin(), e->end() : donne les sommets de l'arête (de type WVertex *)
**    - e->right(), e->left() : donne les faces gauche et droite (de type WFace *)
**    - e->succLeft(), e->succRight(), e->predLeft(), e->predRight() donnent les arêtes incidentes à droite et à gauche (de type WEdge *)
** - si f de type WFace * :
**    - f->normal() donne la normale à f (de type Vector3).
** - sur les Vector3 on peut faire les opérations v1+v2,v1/k, v.normalize(), ...
** *********************************************************************************** */


void WVertex::computeNormal() {
  Vector3 average(0,0,0); // à calculer

  // TODO : compléter



  // A LAISSER à la fin
  average.normalize();
  this->normal(average);
}


/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/


/** ************************************************************************************
** A COMPLETER : WVertex::computePointVertex() => calculer le sommet de vertex selon catmull :
** soit A la moyenne des milieux des arêtes incidentes au sommet
** soit B la moyenne des points de face (une_face->pointFace() de type Vector3).
** soit n le nombre d'arêtes incidentes au sommet this
** alors le résultat est (2A+B+(n-3)this->point())/n
** *********************************************************************************** */

void WVertex::computePointVertex() {
  Vector3 A(0,0,0); // iso des milieux des arêtes;
  Vector3 B(0,0,0); // iso des points de face
  WEdge *e=indirectBoundary();
  WEdge *end=directBoundary();
  bool boundaryCase=false;
  unsigned int nb=0;
  if (e!=end) {A+=e->midPoint(); boundaryCase=true;}
  do {
    nb++;
    if (this==e->begin()) {
      B+=e->left()->pointFace();
      e=e->predLeft();
    }
    else {
      B+=e->right()->pointFace();
      e=e->predRight();
    }
    A+=e->midPoint();
  } while (e!=end);

  B=B/(float)nb;
  if (boundaryCase) A=A/(float)(nb+1); else A=A/(float)(nb);

  if (nb<4) nb=4;
  catmullPoint_=(2.0*A+B+((float)nb-3.0)*position())/(float)nb;
}

/** ************************************************************************************
** A COMPLETER : WVertex::drawLineCatmull() => on trace tous les segments qui relie le point de sommet aux points d'arête selon Catmull-Clark
** - this->pointVertex() donne le Vector3 du point de sommet
** - il faut parcourir toutes les arêtes e autour de this, et e->pointEdge() donne le point d'arête (de type Vector3).
** - la procédure UtilGL::draw(p1,p2) trace le segment entre p1 et p2 (de type Vector3).
** *********************************************************************************** */

void WVertex::drawLineCatmull() {
  WEdge *e=this->edge();
  do {
    p3d::drawLines(vector<Vector3>{this->pointVertex(),e->pointEdge()});
    if (this==e->begin())
      e=e->predLeft();
    else
      e=e->predRight();
  } while (e!=this->edge());
}

/** *********************************************************************************** */


WVertex::~WVertex() {
  //dtor
}

WVertex::WVertex(const Vector3 &p,Winged *owner) {
  position_=p;
  owner_=owner;
  edge_=NULL;
}


WEdge *WVertex::indirectBoundary() {
  WEdge *e=edge();
  bool found=false;
  do {
    if (this==e->begin()) {
      if (e->right()==NULL) found=true;
      else e=e->succRight();
    }
    else {
      if (e->left()==NULL) found=true;
      else e=e->succLeft();
    }
  } while (!found && (e!=edge()));

  if (found) return e; else return edge();
}


Winged *WVertex::owner() {
  return owner_;
}

void WVertex::checkVertexBound() {

  unsigned int nbBound=0;
  Winged *w=owner_;
  WEdge *e;
  for(unsigned int i=0;i<w->nbEdge();i++) {
    e=w->edge(i);
    if (((e->begin()==this) || (e->end()==this)) && ((e->left()==NULL) || (e->right()==NULL))) {
      nbBound++;
    }
  }
  if (nbBound>2) {
    cout << "WARNING !!!! vertex check : nbBound =" << nbBound << endl;
  }

}

WEdge *WVertex::directBoundary() {
  WEdge *e=edge();
  bool found=false;
  do {
    if (this==e->begin()) {
      if (e->left()==NULL) found=true;
      else e=e->predLeft();
    }
    else {
      if (e->right()==NULL) found=true;
      else e=e->predRight();
    }
  } while (!found && (e!=edge()));

  if (found) return e; else return edge();
}


const Vector3 &WVertex::normal() const {
  return normal_;
}

void WVertex::normal(const Vector3 &n) {
  normal_=n;
}

WEdge *WVertex::edge() {
  return edge_;
}

void WVertex::edge(WEdge *e) {
  edge_=e;
}

const Vector3 &WVertex::position() const {
  return position_;
}

void WVertex::position(const Vector3 &p) {
  position_=p;
}

void WVertex::index(size_t i) {
  index_=i;
}

size_t WVertex::index() {
  return index_;
}


const Vector3 &WVertex::pointVertex() const {
  return catmullPoint_;
}


