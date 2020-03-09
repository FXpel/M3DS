#include "FaceBSP.h"
#include <math.h>

using namespace p3d;
using namespace std;

/**
* @author Fabrice Aubert
*/

/** ********************************************************
  Trois méthodes à compléter pour le TP : sign, intersection et separe

*/


/// détermine le coté du point p par rapport à la face this (le résultat de type ESign peut être Sign_Minus ou Sign_Plus)
ESign FaceBSP::sign(const Vector3 &p) const {
  /// méthodes utiles :
  /// - vertex_[i].position() : les coordonnées (de type Vector3) du i-ième sommet de la face (i commence à 0)
  /// - normal_ : la normale (de type Vector3) de la face
  /// - dot(v1,v2) : produit scalaire entre 2 Vector3
  /// - Vector3 v=p2-p1 : pour obtenir le vecteur p1p2


  ESign res=Sign_None;
  Vector3 p2 = vertex_[0].position_;
  Vector3 P1P2 = p2 - p;

  double prodScalaire = dot(P1P2,normal_);

  if(prodScalaire >=0 ){
      res = Sign_Plus;
  }
  else{
      res = Sign_Minus;
  }


  return res;
}


/// Donne le point d'intersection de la face this avec le segment [v1v2] (précondition v1 et v2 de signes distincts par rapport à this)
VertexBSP FaceBSP::intersection(const VertexBSP &v1,const VertexBSP &v2) const {
  /// il suffit de résoudre AI.n=0 (plan (A,n)) et I=v1+k*v1v2 (droite donnée par (v1,v2))
  /// - position(i) : vous donne les coordonnées (type Vector3) du sommet i de la face
  /// - normal() : vous donne la normale de la facette (type Vector3)
  /// - v1.position() et v2.position() pour avoir les positions des sommets v1 et v2
  /// - quelques opérateurs sur les Vector3 peuvent être utiles. Exemple : u=b-a, p=a+k*u, etc (avec a,b,p et u de type Vector3)
  /// - dot(u1,u2) : produit scalaire

  VertexBSP res;
  Vector3 V1V2 = v1.position() - v2.position();
  Vector3 A = position(0);
  double k = (dot(A,normal()) - dot(v1.position(),normal()))/ dot(V1V2,normal());
  Vector3 I =v1.position()+k*V1V2;
  if(0.01>=dot(V1V2,normal()) <= -0.01){
      I = v1.position();
  }
  res = VertexBSP(I,normal());
  res.normal(v1.normal());


  return res;
//  VertexBSP res;

//    Vector3 v1pos = v1.position();
//    Vector3 v2pos = v2.position();

//    Vector3 a = position(0);

//    // I = v1 + k * v1v2
//    // k * v1v2 = I - v1
//    // k = (I-v1) / v1v2
//    // On prendra partie1 la partie gauche et partie2 la partie droite de la division

//    double partie1 = dot((a-v1pos),normal());
//    double partie2 = dot(v2pos-v1pos,normal());

//    double k = partie1/partie2;

//    //cout << "K vaut : " << k << endl;

//    Vector3 i = v1pos + k*(v2pos-v1pos);

//    if(-0.05 >= partie2 <= 0.05){
//        i = (v2pos + v1pos) / 2;
//    }

//    res.position(i);
//    res.normal(v1.normal());


//    return res;
}


/// coupe f selon this (i.e. f est découpée par le plan porteur de this) :
/// précondition : f doit être **convexe**
void FaceBSP::separe(const FaceBSP &f,FaceBSP &fPositive,FaceBSP &fNegative) const {
  /// - il faut séparer les sommets de f pour les ajouter dans les faces fPositive et fNegative (algo de séparation vu en cours)
  /// - VertexBSP a=f.vertex(i) vous donne le i-ième sommet de la face f.
  /// - f.size() vous donne le nombre de sommet de f.
  /// - fNegative.add(v) ajoutera le sommet v à fNegative
  /// - lors d'une intersection : il faut ajouter le sommet obtenu à fPositive et fNegative

  fNegative.clear();
  fPositive.clear();
  ESign signe;
  int cpt =0;

  ESign prev_sign=Sign_None;
  for (int i =0;i <= f.size();i++) {
      int id = i % f.size();

      VertexBSP v = f.vertex(id);
       signe = sign(v.position());
      if(signe != prev_sign && prev_sign != Sign_None) {
          VertexBSP intersect = intersection(f.vertex(i-1), f.vertex(id));
          fNegative.add(intersect);
          fPositive.add(intersect);

      }

      if(i < f.size()){
          if(signe == Sign_Minus){
              fNegative.add(f.vertex(i));

          }
          else {
              fPositive.add(f.vertex(i));

          }
      }




  prev_sign = signe;



  }

//   fNegative.add(intersection(f.vertex(f.size()-1), f.vertex(0)));
//   fPositive.add(intersection(f.vertex(f.size()-1), f.vertex(0)));



  /** affecte la normale de f aux 2 nouvelles faces **/
  fPositive.normal(f.normal());
  fNegative.normal(f.normal());

}

/** ************************************************************************ */
/** ************************************************************************ */
FaceBSP::~FaceBSP() {
}

FaceBSP::FaceBSP() {
}


void FaceBSP::add(const VertexBSP &v) {
//  if (_tabVertex.empty() || (((_tabVertex[0]->point().distance2(v->point())>0.0000001) && ((_tabVertex[_tabVertex.size()-1]->point().distance2(v->point())>0.000001)))))
  vertex_.push_back(v);
}

Vector3 FaceBSP::normal() const {return normal_;}
void FaceBSP::normal(const Vector3 &n) {normal_=n;}

void FaceBSP::clear() {
  vertex_.clear();
}

