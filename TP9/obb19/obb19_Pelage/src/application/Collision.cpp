#include "Collision.h"

#include "Box.h"
#include "DebugTool.h"
#include "Plane.h"

#include <algorithm>
#include <vector>

using namespace p3d;
using namespace std;


bool Collision::visualDebug_=true;

Collision::Collision() {
}

/** détecte la collision entre b1 et b2 et donne les informations nécessaires à la réponse dans *collision
  - principe : il faut déterminer les distances de recouvrement sur chacun des 4 axes possibles (2 axes pour b1 et 2 axes pour b2).
    si on trouve une distance négative, c'est que les boites ne se recouvrent pas sur cet axe => pas de collision (arrêt).
    si toutes les distances sont positives : b1 et b2 sont en intersection, et on renseigne *collision :
    - collision->mtd=un double : distance minimal sur les 4 axes (i.e. distance nécessaire pour séparer les 2 boites : sera utilisé pour la correction en position)
    - collision->normale=un Vector3 : l'axe de la distance minimal qui donne la normale au contact (i.e. direction de séparation)
    - collision->apply=un Vector3 : le point d'application qui sera utilisé pour l'impulsion (utilisé pour le calcul du moment de la force d'impulsion) :
      Le point d'application est déjà calculé (moyenne de tous les sommets intérieurs aux boites).
**/
void Collision::detectCollision(const Box &b1,const Box &b2) {


  array <Vector3,4> axis; // 4 axes à tester
  Vector3 axe_min; // l'axe qui correspond à la plus petite distance de séparation.
  double dist_min; // plus petite distance de séparation

  // les 4 axes potentiellement séparateurs
  axis[0]=b1.directionX(); // axe x de b1 (exprimé dans repère global)
  axis[1]=b1.directionY(); // axe y de b1
  axis[2]=b2.directionX(); // axe x de b2
  axis[3]=b2.directionY(); // axe y de b2

  // A completer (1 seul axe pour l'instant => il faut tenir compte des 4 axes) :
  // - déterminez la distance minimale dist_min de recouvrement entre les 4 axes axis[i] qui sont non séparateurs (Attention : minimale en valeur absolue !, mais dist_min est négative s'il y a recouvrement)
  // - vous devez affecter correctement axe_min (l'axe correspondant à dist_min) qui est un des axis[i] *mais* en tenant compte du sens de séparation de
  //   b2 par rapport à b1 (i.e. multiplier axis[i] par le signe (-1 ou 1) retourné par la méthode distance(b1,b2,...,)).
  // - assurez vous d'avoir affecté correctement detect à la fin (==true il y a collision, == false pas de collision).
  isCollide_ = true;
    for (int i=0;i < 4;i++){
      DistanceInfo d=distance(b1,b2,axis[i]);
      if (d.distance_<0){
          p3d::addDebug(b1.position(),b1.position()-d.direction_*d.distance_*axis[i],"",Vector3(0.2,0.2,1));
      }
      else{
          isCollide_=false; // force une non détection (à enlever lorsque votre détection sera achevée).
          if (abs(d.distance_)< abs(dist_min)){
              dist_min = d.distance_;
              axe_min = d.direction_*axis[i];
          }
      }
     }



  // affecter les informations nécessaires à la réponse
  if (isCollide_) {
    mtd_=dist_min;
    axis_=axe_min;

    // Calcul du point d'application de l'impulsion (ici barycentre des sommets intérieurs).
    // on créé la liste de tous les sommets inclus dans une des boites
    vector<Vector3> liste;
    liste.clear();
    for(size_t i=0; i<4; ++i) { // 4 vertices for each box
      if (b1.isInside(b2.vertex(i))) liste.push_back(b2.vertex(i));
    }

    for(size_t i=0; i<4; ++i) {
      if (b2.isInside(b1.vertex(i))) liste.push_back(b1.vertex(i));
    }

    Vector3 apply=Vector3(0,0,0);
    for(unsigned int i=0; i<liste.size(); i++) {
      apply+=liste[i];
    }
    apply/=liste.size();

    applicationPoint_=apply;

  }

}


void Collision::collision(Box &b1,Box &b2) {

  detectCollision(b1,b2); // set isCollide_ (and, if true, set all parameters for the response)
  if (isCollide_) {
    b1.enableVisualEffect(1);
    b2.enableVisualEffect(1); // sert uniquement à faire un retour visuel de collision.

    Vector3 normal=axis_;
    Vector3 r1=b1.position()-applicationPoint_;
    Vector3 r2=b2.position()-applicationPoint_;

    double impulse=impulseCoefficient(b1,b2);




 // début Réponse à la collision :

        // décomposition du calcul : normal, vP1_old : vitesse du point de contact par rapport à b1, r1xN : produit vectoriel PG x normal, etc


    // calcul de la force d'impulsion
    Vector3 force=impulse*normal;

    // correction du mouvement :
    // - on corrige la vitesse angulaire et la vitesse des boites
    // - on corrige la position (car on répond à une position en recouvrement).
    b1.addVelocityCorrec(force/b1.mass());
    b1.addOmegaCorrec(cross(force,r1)/b1.inertia());

    b2.addVelocityCorrec(-force/b2.mass());
    b2.addOmegaCorrec(-cross(force,r2)/b2.inertia());

    b1.addPositionCorrec(normal*mtd_);
    b2.addPositionCorrec(-normal*mtd_);
         // fin réponse à la collision


  }
  else {
    b1.disableVisualEffect(1); // pas d'effet visuel de détection de collision
    b2.disableVisualEffect(1);
  }
}



DistanceInfo Collision::distance(const Box &b1,const Box &b2, const Vector3 &axe) {
  DistanceInfo res;
  double dist;
  Interval i1=b1.project(axe);
  Interval i2=b2.project(axe);
  double d1 = i1.inf_;
  double f1 = i1.sup_;
  double d2 = i2.inf_;
  double f2 = i2.sup_;
  double direction;

  if (visualDebug_) drawDebugProjection(b1,b2,axe,i1,i2);

// A completer
  // d1,f1 : intervalle de projection pour la boite b1
  // d2,f2 : intervalle de projection pour la boite b2
  // quelle est la distance de recouvrement ? (*distance = ??)
  // affecter correctement *direction (-1 ou 1 ?)

  if (d2 < f1 && d1 < f2){
           res.distance_=min( d2-f1, d1-f2 );
        }else {
            if (d1 < d2){
                res.distance_=abs(f1-d2);
            }else{
                res.distance_=abs(f2-d1);
            }
        }



        if ( ((f2-d2)/2 + d2) < ((f1-d1/2)+d1) ){
            res.direction_ = -0.1;
        }
        else{
            res.direction_= 0.1;
        }

  return res;
}

/************************************************************************** */
/************************************************************************** */
/************************************************************************** */
/************************************************************************** */

/*
 * @precondition : detectCollision was called (and true)
 *
*/
double Collision::impulseCoefficient(const Box &b1,const Box &b2) {
  // décomposition du calcul : normal, vP1_old : vitesse du point de contact par rapport à b1, r1xN : produit vectoriel PG x normal, etc
  Vector3 normal=axis_;
  normal.normalize();
  Vector3 r1=b1.position()-applicationPoint_;
//  Vector3 vP1_old=b1.velocity()+cross(r1,b1.omega());

  Vector3 r2=b2.position()-applicationPoint_;
//  Vector3 vP2_old=b2.velocity()+cross(r2,b2.omega());

  Vector3 r1xN=-cross(r1,normal);
  Vector3 r2xN=-cross(r2,normal);

  // calcul de l'impulsion.
  double impulseNum;
  double impulseDen;

  impulseNum=-(1+0.5)*(dot(normal,b1.velocity()-b2.velocity())+dot(b1.omega(),r1xN)-dot(b2.omega(),r2xN));
  impulseDen=1.0/b1.mass()+1.0/b2.mass()+1.0/b1.inertia()*dot(r1xN,r1xN)+1.0/b2.inertia()*dot(r2xN,r2xN);

  return impulseNum/impulseDen;

}

double Collision::impulseCoefficient(const Box &b, const Plane &plane) {
  Vector3 normal=normalize(plane.normal());

  // on calcule la vitesse du point choisi pour la collision dans vP_old
  Vector3 r1=b.position()-applicationPoint_;
  //Vector3 vP_old=b.velocity()+cross(r1,b.omega());

  // on calcule l'impulsion (restitution à 0.5)
  Vector3 r1xN=-cross(r1,normal);
  double impulseNum;
  double impulseDen;

  impulseNum=-(1+0.5)*(dot(normal,b.velocity())+dot(b.omega(),r1xN));
  impulseDen=1.0/b.mass()+1.0/b.inertia()*dot(r1xN,r1xN);

  return impulseNum/impulseDen;

}

void Collision::detectCollision(const Box &b,const p3d::Plane &plane) {
  isCollide_=false;
  std::vector<Vector3> list;
  list.clear();
  for(unsigned int k=0;k<4;k++) {
    list.push_back(b.vertex(k));
  }

  size_t plus_loin=0;
  double dist=plane.distance(list[0]);
  for(size_t k=1; k<list.size(); ++k) {
    double comp=plane.distance(list[k]);
    if (comp<dist) {
      dist=comp;
      plus_loin=k;
    }
  }
  Vector3 c_choice=list[plus_loin];
  // c_choice correspond au sommet le plus loin, dist donne la distance de recouvrement

  if (dist<0) {
    isCollide_=true;
    applicationPoint_=c_choice;
    mtd_=dist;
  }
}

void Collision::collision(Box &b,const Plane &plane) {
  detectCollision(b,plane);
  // on détermine le sommet de la boite le plus négatif
  // TODO => Box method


  if (isCollide_) { // il y a collision
    Vector3 normal=normalize(plane.normal());
    Vector3 r1=b.position()-applicationPoint_;

    double impulse=impulseCoefficient(b,plane);
    Vector3 force=impulse*normal; // "force" d'impulsion

    b.addVelocityCorrec(force/b.mass()); // correction en vitesse
    b.addOmegaCorrec(cross(force/b.inertia(),r1)); // correction en vitesse angulaire

    Vector3 PH;
    PH=dot(plane.point()-applicationPoint_,normal)/normal.length2()*normal;
    b.addPositionCorrec((1.0+0.5)*PH); // correction en position


  }

}


void Collision::drawDebugProjection(const Box &b1,const Box &b2,const Vector3 &axe,Interval i1,Interval i2) {

  Vector3 ref=0.5*(b1.position()+b2.position());
  double lambda=dot(ref,axe);
  Vector3 decal=cross(axe,Vector3(0,0,1));
  decal.normalize();
  decal=decal/10.0;
  p3d::addDebug(ref+decal+(i1.inf_-lambda)*axe,ref+decal+(i1.sup_-lambda)*axe,"",b1.color());
  p3d::addDebug(ref-decal+(i2.inf_-lambda)*axe,ref-decal+(i2.sup_-lambda)*axe,"",b2.color());
}

