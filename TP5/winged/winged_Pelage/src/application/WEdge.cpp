
#include "WEdge.h"

#include <vector>

#include "glsupport.h"
#include "Tools.h"

#include "WFace.h"
#include "WVertex.h"
#include "GLDraw.h"


/**
@file
@author F. Aubert

@brief Winged-Edges : opérations sur les arêtes
*/

using namespace p3d;
using namespace std;



/** ************************************************************************************
** A COMPLETER : WEdge::computePointEdge => calcul des sommets d'arête
** c'est la moyenne entre les milieux d'aretes et des points de faces adjacents.
** les points de face adjacents sont accessibles par this->left()->pointFace(), et this->right()->pointFace() (de type Vector3).
** le calcul du milieu se fait avec e->begin()->point() et e->end()->point() (de type Vector3 qui permet les opérations +,/, etc).
** ************************************************************************************ */
void WEdge::computePointEdge() {
  pointEdge_=Vector3(0,0,0);
  unsigned int nb=0;
  if (left()) {pointEdge_+=left()->pointFace();nb++;}
  if (right()) {pointEdge_+=right()->pointFace();nb++;}
  pointEdge_+=begin()->position()+end()->position();
  pointEdge_=pointEdge_/(2.0+nb);

}


/** ************************************************************************************
** A COMPLETER : WEdge::insertVertexPointEdge => insertion d'un sommet correspondant au sommet d'arête.
** ************************************************************************************ */

void WEdge::insertVertexPointEdge() {
  WEdge *new_edge;
  WVertex *new_vertex;

  // construction du nouveau sommet
  new_vertex=owner_->createVertex(pointEdge());
  new_vertex->edge(this); // seul champ à mettre à jour

  // construction de la nouvelle arête (new_edge->begin(), et new_edge->end() déjà affecté par la construction).
  new_edge=owner_->createEdge(new_vertex,end());


  // il faut mettre à jour tous les champs de new_edge, mais également de tous les champs des entités adjacentes
  // (par exemple il faut changer le successeur, ou le prédécesseur selon le sens, de this->succRight()...).
  if(this->succLeft()->begin()==new_edge->end())
    this->succLeft()->predLeft(new_edge);
  else
    this->succLeft()->predRight(new_edge);
  if(this->predRight()->end()==new_edge->end())
    this->predRight()->succLeft(new_edge);
  else
    this->predRight()->succRight(new_edge);
//  new_edge->succLine(this->succLine());
  new_edge->left(this->left());
  new_edge->right(this->right());
  new_edge->succLeft(this->succLeft());
  new_edge->predLeft(this);
  new_edge->succRight(this);
  new_edge->predRight(this->predRight());
  this->succLine(new_edge);
  this->succLeft(new_edge);
  this->predRight(new_edge);
  this->end(new_edge->begin());

  new_edge->end()->edge(new_edge);


}


/** ************************************************************************************ */



// start should be a boundary edge
void WEdge::followBoundary() {
  WEdge *e;
  WEdge *next;
  e=this;
  unsigned int number=0;
  do {
      if (e->left()) next=e->end()->directBoundary();
      else if (e->right()) next=e->begin()->directBoundary();
      else throw ErrorD("WEdge::followBoundary : not a boundary edge");
      e->succLine(next);
      e->mark();
      e=next;
      number++;
      // if (e->isMark() && e!=start) {cout << "bouclage" << e <<endl;error("problème de link",__LINE__,__FILE__);}
  } while (e!=this);
}


WEdge::~WEdge() {
  //dtor
}

WEdge::WEdge(WVertex *b,WVertex *e,Winged *owner) {
  begin_=b;
  end_=e;
  owner_=owner;
  computeVector();
}

Winged *WEdge::owner() {
  return owner_;
}

WVertex *WEdge::begin() const {
  return begin_;
}

WVertex *WEdge::end() const {
  return end_;
}

void WEdge::begin(WVertex *b) {
  begin_=b;
  if (end_) computeVector();
}

void WEdge::end(WVertex *e) {
  end_=e;
  if (begin_) computeVector();
}

WFace *WEdge::left() const {
  return left_;
}

WFace *WEdge::right() const {
  return right_;
}

void WEdge::left(WFace *f) {
  left_=f;
}

void WEdge::right(WFace *f) {
  right_=f;
}

WEdge *WEdge::succLeft() const {
  return succLeft_;
}

WEdge *WEdge::succRight() const {
  return succRight_;
}

WEdge *WEdge::predLeft() const {
  return predLeft_;
}

WEdge *WEdge::predRight() const {
  return predRight_;
}

void WEdge::succLeft(WEdge *e) {
  succLeft_=e;
}

void WEdge::succRight(WEdge *e) {
  succRight_=e;
}
void WEdge::predLeft(WEdge *e) {
  predLeft_=e;
}
void WEdge::predRight(WEdge *e) {
  predRight_=e;
}

Vector3 WEdge::midPoint() {
  Vector3 res;
  res=(begin()->position()+end()->position())/2.0;
  return res;
}

void WEdge::draw() {
}

void WEdge::computeVector() {
  vec_=end()->position()-begin()->position();
//  _vector.normalize();
}

const Vector3 &WEdge::vec() const {
  return vec_;
}

Vector3 WEdge::direction() {
  Vector3 res;
  if (vec_.length2()>0.00000001) {
    res=vec_;
    res.normalize();
    return res;
  }
  else return Vector3(0,0,0);
}

size_t WEdge::index() {
  return index_;
}

void WEdge::index(size_t i) {
  index_=i;
}


const Vector3 &WEdge::pointEdge() const {
  return pointEdge_;
}


void WEdge::mark() {
  mark_=true;
}

void WEdge::unmark() {
  mark_=false;
}

bool WEdge::isMark() {
  return mark_;
}

void WEdge::drawLine() {
  vector<int> a;
  vector<Vector3> pts;
  pts.clear();
  unsigned int num=0;
  WEdge *e,*start;
  WVertex *v;
  start=this;
  e=start;
  if (start->succLine()) {
    if (begin()==succLine()->begin()) v=end(); else v=begin();
  }
  else{
    return;
  }
  do {
    pts.push_back(e->begin()->position());
    pts.push_back(e->end()->position());
    e=e->succLine();
    if (e) {if (v==e->begin()) v=e->end(); else v=e->begin();}
    num++;
  } while ((e!=start) && e);

  p3d::drawLines(pts);
}


void WEdge::succLine(WEdge *e) {
  succLine_=e;
}

WEdge *WEdge::succLine() {
  return succLine_;
}

ostream& operator <<(std::ostream &s,const WEdge &e) {
    s << "(begin = " << e.begin() << ", end= " << e.end() << "left,right = " <<  e.left() << "," << e.right() << " succLeft = " << e.succLeft() << " succRight= " << e.succRight() << " predLeft = " << e.predLeft() << " predRight=" << e.predRight();
    return s;
}




