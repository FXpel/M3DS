#include "SceneIntersection.h"

#include "Plane.h"
#include "MeshObject3D.h"

using namespace p3d;
using namespace std;




/**
  l : pickingRay supposé dans le repère local au triangle. l.point() donne l'origine, l.direction() donne le vecteur directeur.
  s0,s1,s2 : sommets du triangle.
  Il faut :
  - d'abord déterminer si la droite intersecte le triangle (i.e. l passe du même coté des segments [s0s1],[s1s2],[s2s0])
  - si la droite intersecte le triangle, calculer le lambda de l'intersection (i.e. intersection entre le plan du triangle qui serait donné par I=l.point()+lambda*l.direction(); inutile de calculer I, mais seulement lambda).
  - il faut affecter le résultat res.isIntersect_ et res.lambda_ (nécessaire seulement si isIntersect est true)
  - rappel : cross(a,b) donne le produit vectoriel, et dot(a,b) le produit scalaire
 */
IntersectResult SceneIntersection::intersect(const Line &ray,const Vector3 &s0,const Vector3 &s1,const Vector3 &s2) {
  IntersectResult res;
  Vector3 e1, e2;
  Vector3 P, Q, T;
  float det, idet, u, v;
  float t;

  e1 = (s0, s1);
  e2 = (s0, s2);


  P = cross(ray.direction(),e2);
  det = dot(e1,P);
  if(det == 0 )
      return res;
  idet = 1.f / det;
  T = s0, ray.point();
  u = dot(T,P) * idet;

  if(u < 0.f || u > 1.f)
      return res;
  Q = cross(T,e1);
  v = dot(ray.direction(),Q) * idet;
  if(v < 0.f || u + v > 1.f)
      return res;

  t = dot(e2,Q) * idet;

  res.isIntersect_ = t > 0.;

  if(res.isIntersect_)
      res.lambda_ = t;
   return res;
}

/** ******************************************************************************************************************************************************* */
/** ******************************************************************************************************************************************************* */
/** ******************************************************************************************************************************************************* */
/** ******************************************************************************************************************************************************* */
/** ******************************************************************************************************************************************************* */
/** ******************************************************************************************************************************************************* */

/**
  Intersection du rayon avec un objet donné (parcours de tous les triangles avec appel de intersect; seul la plus proche est retenue)
 */
void SceneIntersection::intersect(MeshObject3D *e) {
  Line rayLocal;
  rayLocal.point(e->pointTo(Coordinate_Local,pickingRay_.point())); // ray in local frame
  rayLocal.direction(e->directionTo(Coordinate_Local,pickingRay_.direction()));

  for(size_t i=0;i<e->nbFace();i++) {
    IntersectResult ok=intersect(rayLocal,e->positionVertexFace(i,0),e->positionVertexFace(i,1),e->positionVertexFace(i,2));
    if (ok.isIntersect_ && ok.lambda_>1) { // intersection and front of screen
      Intersection inter;
      inter.mesh(e);
      inter.lambda(ok.lambda_);
      inter.rayWorld(pickingRay_);
      insert(inter);
    }
  }
}

/**
   initialisation de la liste des intersections (1 intersection par
objet) : liste d'objets + rayon => détermination de toutes les
intersections avec le rayon
  */
void SceneIntersection::intersect(const std::vector<p3d::MeshObject3D *> &allObj, const p3d::Line &pickingRay) {
   result_.clear(); //******* ligne à ajouter ******
   pickingRay_=pickingRay;
   for(auto e:allObj) {
     intersect(e);
   }
}

/**
  insertion dans la liste d'intersections en conservant le tri (les lambdas ordonnés par ordre croissant)
 */

void SceneIntersection::insert(const Intersection &inter) {
  bool found=false;
  size_t place=result_.size();
  result_.push_back(inter);
  while(!found) {
    if (place==0 || result_[place-1].lambda()<=inter.lambda()) {
      found=true;
    }
    else {
      result_[place]=result_[place-1];
      place--;
    }
  }
  result_[place]=inter;
}

