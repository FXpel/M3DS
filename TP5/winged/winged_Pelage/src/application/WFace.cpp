#include "WFace.h"
#include "Tools.h"
#include "GLDraw.h"
#include "WVertex.h"

/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/


using namespace p3d;
using namespace std;

/** ************************************************************************************
** A COMPLETER : WFace::draw()
** On parcourt tous les sommets de la face en remplissant le tableau position (qui est tracé à la fin).
** - this->edge() : donne l'arête affectée à la face
** - si e de type WEdge * :
**    - e->begin(), e->end() : donne les sommets de l'arête (de type WVertex *)
**    - e->right(), e->left() : donne les faces gauche et droite (de type WFace *) (Remarque : e->left()==this est un test valide)
**    - e->succLeft(), e->succRight(), e->predLeft(), e->predRight() donnent les arêtes incidentes à droite et à gauche (de type WEdge *)
**    - v->position() donne les coordonnées du sommet v de type Vector3
**    - v->normal() donne les coordonnées de la normale calculée au sommet v de type Vector3
** *********************************************************************************** */

void WFace::draw(bool withNormal) {
  WEdge *e, *e_new, *start;
  WVertex *v;
  e=start=this->edge();

  vector<Vector3> position;
  position.clear();

  vector<Vector3> normal;
  normal.clear();
  //faire un do  whilewsxccv
  do {
      if(this == e->left()){
          v = e->begin();
          e_new = e ->succLeft();
      }
      else{
          v = e->end();

          e_new = e ->succRight();
      }
      position.push_back(v->position());
      e=e_new;
  } while (e != start);



  // TODO : compléter



  // A Laisser à la fin (effectue un affichage polygone par polygone : lent => uniquement pour vérification/TP)
  if (withNormal)
    p3d::drawPolygon(position,normal,true);
  else
    p3d::drawPolygon(position,false,0.8f);

}

/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/


/** ************************************************************************************
** A COMPLETER : WFace::computePointFace() => point de face de catmull clark (iso barycentre des sommets).
** rappel : les coordonnés d'un sommet WVertex *v s'obtiennent par v->point() (de type Vector3).
** ************************************************************************************ */
void WFace::computePointFace() {
  WEdge *e=edge();
  Vector3 barycentre;
  WVertex *v;
  int nbedge=0;
  barycentre=Vector3(0,0,0);
  do {
    if (e->left()==this) v=e->begin(); else v=e->end();
    barycentre+=v->position();
    nbedge++;
    if (e->left()==this) e=e->succLeft(); else e=e->succRight();
  } while (e!=edge());
  barycentre=barycentre/nbedge;

  // affectation point de face
  pointFace_=barycentre;
}


/** ************************************************************************************
** A COMPLETER : WFace::drawLineCatmull() => on trace tous les segments qui relie le point de face aux points d'arête selon Catmull-Clark
** - this->pointFace() donne le Vector3 du point de face
** - il faut parcourir toutes les arêtes e appartenant à this, et e->pointEdge() donne alors le point d'arête (de type Vector3).
** - la procédure UtilGL::draw(p1,p2) trace le segment entre p1 et p2 (de type Vector3).
** *********************************************************************************** */

void WFace::drawLineCatmull() {
  WEdge *e=this->edge();
  do {
    p3d::drawLines(vector<Vector3>{this->pointFace(),e->pointEdge()});
    if (this==e->left())
      e=e->succLeft();
    else
      e=e->succRight();
  } while (e!=this->edge());
}


/** *********************************************************************************** */




WFace::WFace(Winged *owner) {
  owner_=owner;
  edge_=nullptr;
  //ctor
}

WFace::~WFace() {
  //dtor
}

void WFace::edge(WEdge *e) {
  edge_=e;
}

WEdge *WFace::edge() {
  return edge_;
}

Winged *WFace::owner() {
  return owner_;
}

void WFace::index(size_t i) {
  index_=i;
}

size_t WFace::index() {
  return index_;
}

void WFace::normal(const Vector3 &n) {
  normal_=n;
}

const Vector3 &WFace::normal() const {
  return normal_;
}

const Vector3 &WFace::pointFace() const {
  return pointFace_;
}


void WFace::computeNormal() {
  WEdge *u,*v,*start;
  Vector3 uvect,vvect;
  Vector3 n;
  start=edge();
  u=edge();
  if (u->left()==this) uvect=u->direction(); else uvect=-u->direction();
  do {
    if (u->left()==this) {
      v=u->succLeft();
    }
    else {
      v=u->succRight();
    }
    vvect=v->direction();
    if (v->right()==this) vvect=-vvect;
    n=cross(uvect,vvect);
    u=v;
    uvect=vvect;
  } while (n.length2()<0.000000001 && (u!=start));
  if (u==start) {
    normal(Vector3(0,0,0)); // cas particulier : face aplatie
  }
  else {
    n.normalize();
    normal(n);
  }
}

// Catmull
void WFace::insertVertexPointFace() {
  WVertex *new_vertex=owner()->createVertex(pointFace());

  WEdge *start_new,*new_edge,*pred_edge;
  WEdge *start=edge();
  if (start->left()==this) start=start->succLeft();

  WFace *new_face;

  WEdge *e1=start,*e2,*succ_e2;
  if (e1->left()==this) {
    e2=e1->succLeft();
    new_edge=owner_->createEdge(new_vertex,e1->begin());
  }
  else {
    e2=e1->succRight();
    new_edge=owner_->createEdge(new_vertex,e1->end());
  }
  new_vertex->edge(new_edge);
  pred_edge=new_edge;
  start_new=new_edge;

  do {
    new_face=owner_->createFace();
    new_face->edge(e1);
    if (e2->left()==this) {
      succ_e2=e2->succLeft();
      if (succ_e2!=start) new_edge=owner_->createEdge(new_vertex,e2->end()); else new_edge=start_new;
      e2->left(new_face);
      e2->succLeft(new_edge);
    }
    else {
      succ_e2=e2->succRight();
      if (succ_e2!=start) new_edge=owner_->createEdge(new_vertex,e2->begin()); else new_edge=start_new;
      e2->succRight(new_edge);
      e2->right(new_face);
    }

    new_edge->right(new_face);
    new_edge->succRight(pred_edge);
    new_edge->predRight(e2);

    pred_edge->left(new_face);
    pred_edge->predLeft(new_edge);
    pred_edge->succLeft(e1);

    if (e1->left()==this) {
      e1->left(new_face);
      e1->predLeft(pred_edge);
    }
    else {
      e1->right(new_face);
      e1->predRight(pred_edge);
    }

    e1=succ_e2;
    if (e1->left()==this) e2=e1->succLeft(); else e2=e1->succRight();
    pred_edge=new_edge;
  } while (e1!=start);

}



