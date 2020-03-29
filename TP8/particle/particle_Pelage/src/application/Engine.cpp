#include "Engine.h"
#include <cstdlib>

#include "ParticleList.h"
#include "Particle.h"

/**
@file
@author F. Aubert
@brief particle engine

*/

using namespace std;
using namespace p3d;


/**
** Méthode principale pour le mouvement (appelée à chaque tracé d'image) : résolution pour une durée _dt prédéfinie
** - incrémentation du temps
** - résolution des collisions (détection/réponse par rapports aux plans, et aux autres sphères).
** - calcul des forces
** - intégration par euler
**/
void Engine::update() {
  double dt=deltaTimeSimule();



  // gestion de la vie des particules (naissances/morts) :
  particleList_.updateLife(dt);

  // réinitialise à 0 toutes les forces à appliquer :
  resetForce();
  // calcul des forces appliquées à chaque particule :
  computeForce();
  // calcul des positions/vitesses avec euler explicite :
  euler(dt);



  // résolution des collisions avec les plans de la scène (affectation des corrections en vitesse et position) :
  collisionPlane();
  // inter collisions entre toutes les particules quand elles sont des sphères (affectation des corrections en vitesse et position) :
  if (particleList_.particleMode()==Particle_Sphere) {
    interCollision();
  }
  // prise en compte des corrections issues de l'ensemble des collisions :
  updatePositionVelocityCollision();

  // maj temps écoulé
  previousTime_=currentTime();

}


/** Intégration du mouvement
** - on parcourt l'ensemble des particules p : il faut mettre à jour la position et la vitesse de p durant l'intervalle de temps dt (passé en paramètre).
** - p.position() donne la position actuelle (de type Vector3), et p.position(Vector3) affecte la position de p
** - p.velocity() donne la vitesse actuelle (de type Vector3) et p.velocity(Vector3) affecte la vitesse de p
** - p.force() donne la force totale s'appliquant sur la particule p (de type Vector3; elle est calculée par ailleurs).
** - p.mass() donne la masse de la particule

** Il s'agit ici d'affecter la nouvelle position et la nouvelle vitesse en appliquant l'intégration d'Euler (cf cours).

**/
void Engine::euler(double dt) {
  for(size_t i=0; i<particleList_.size(); ++i) { // parcours de toutes les particules
    Particle &p=particleList_[i]; // "synonyme" (référence)
    if (p.isAlive()) { // si "vivante"
      /* A COMPLETER : affecter la nouvelle vitesse et la nouvelle position de p avec p->velocity(??), et p->position(??).
      */
      p.position(p.position() + dt *p.velocity());
      p.velocity(p.velocity() + dt * (p.force()/p.mass()));


    }
  }
}


/** Calcule les forces appliquées à chaque particule
** Pour ajouter une force il suffit de faire p->addForce(unVector3) (permet de cumuler avec, éventuellement, d'autres forces provenant d'autres procédures).
** Ici, la force est initialement nulle, et on se contente d'ajouter la force de gravité dans le sens des y négatifs.
** p->mass() donne la masse de la particule.

**/

void Engine::computeForce() {
  float gravity = 9.81;
  for(size_t i=0; i<particleList_.size(); ++i) {
    Particle &p=particleList_[i];
    if (p.isAlive()) {
      /* A COMPLETER : appliquer les forces subies */

        p.addForce(Vector3(0,-1,0) * p.mass()*gravity );

    }
  }
}


/** Détermine s'il y a une collision avec les plans de la scène et modifie en conséquence la position et la vitesse de la particule

** - chaque plan de la scène est parcouru (tableau _planeList) : il faut déterminer la collision de chaque particule p avec ce plan
** simplement en testant si p est du coté négatif du plan. Si oui, la particule a traversé le plan et on corrige alors sa vitesse et sa position.

**/
void Engine::collisionPlane() {
  float restitution = 0.2;
  for(unsigned int j=0; j<planeList_.size(); ++j) {
    Plane &plane=planeList_[j];

    for(size_t i=0; i<particleList_.size(); ++i) {
      Particle &p=particleList_[i];
      if (p.isAlive()) {
        Vector3 posCorrection(0,0,0); // correction en position à calculer si collision
        Vector3 velCorrection(0,0,0); // correction en vitesse à calculer si collision
        /* A COMPLETER  : détecter collision et corriger vitesse/position */
        /* on peut utiliser p.position(), p.velocity() (et les setters), et plane.point() et plane.normal() */
        /* p.radius() donne le rayon de la particule (attendre l'exercice avec les sphère pour s'en préoccuper). */
        Vector3 sphere = Vector3(p.position().x(),p.position().y() - p.radius(),p.position().z());
        if(plane.distance(sphere)<0){
            //p.position(plane.point());
            Vector3 H =plane.project(sphere);
              //ex2 Q3
//            p.position(H);
//            p.velocity(Vector3(0,0,0));
            //ex2 Q4
//            p.addPositionCorrec(H-p.position());
//            p.addVelocityCorrec((-1)*p.velocity());

            p.addPositionCorrec( (1 + restitution) * (H - sphere));
            p.addVelocityCorrec(- (1 + restitution) * dot(p.velocity(),plane.normal())*plane.normal());


        }
        // appliquer les corrections calculées :
        p.addPositionCorrec(posCorrection);
        p.addVelocityCorrec(velCorrection);
      }

    }

  }
}

/** Calcul de l'impulsion entre 2 sphères
** - n = normale lors de la collision {\bf normalisée !}
** - restititution : > 0.9 superball ;  <0.1 ecrasement
**/
double Engine::computeImpulse(const Particle &p1, const Particle &p2, const Vector3 &n, double restitution) {
  double res;
  double v12n=dot(p2.velocity()-p1.velocity(),n);
  double inv_mass=1.0/p1.mass()+1.0/p2.mass();
  res=-(1.0+restitution)*v12n/(inv_mass);
  return res;

}

/** Collisions entre toutes les sphères :
** - on teste toutes les paires de particules (double boucle, sans redondances) : s'il y a collision entre 2 sphères p1 et p2 on répond par impulsion.
** - utilisez p1->addVelocityCorrec(...), p2->addVelocityCorrec(...), etc pour tenir compte des corrections issues de cette collision qui seront à appliquer à p1 et p2
** - l'appel à computeImpulse vous donne le coefficient d'impulsion (cette méthode se trouve juste au dessus pour déterminer les paramètres).

**/
void Engine::interCollision() {
  float restitution=0.1;
  for(size_t i=0; i<particleList_.size(); ++i) {
    Particle &p1=particleList_[i];
    if (p1.isAlive()) {
      for(size_t j=i+1; j<particleList_.size(); ++j) {
        Particle &p2=particleList_[j];
        if (p2.isAlive()) {


          Vector3 posCorrectionP1(0,0,0); // correction en position de p1
          Vector3 velCorrectionP1(0,0,0); // correction en vitesse de p1
          Vector3 posCorrectionP2(0,0,0); // correction en position de p2
          Vector3 velCorrectionP2(0,0,0); // correction en vitesse de p2

          /* A COMPLETER */
          double center_dist = (p2.position()-p1.position()).length();
          double sum_rad = p1.radius() + p2.radius();
          if(center_dist < sum_rad){
              Vector3 N = p2.position() - p1.position() ;
              double J = (-(1. + restitution) *dot( (p2.velocity() - p1.velocity()),N)) / dot(((1./p1.mass()) + (1./p2.mass())) * N,N);
              double D = center_dist - p1.radius() - p2.radius();
              Vector3 v1New =  - (J/p1.mass()) *N;
              Vector3 v2New =  (J/p2.mass()) *N;
              Vector3 p1New =  (1+restitution) *(p1.mass()/(p1.mass() + p2.mass()))*D*N;
              Vector3 p2New =  - (1+restitution) *(p2.mass()/(p1.mass() + p2.mass()))*D*N;

              p1.addPositionCorrec(p1New);
              p1.addVelocityCorrec(v1New);

              p2.addPositionCorrec(p2New);
              p2.addVelocityCorrec(v2New);

          }

          // cumuler la correction :
          p1.addPositionCorrec(posCorrectionP1);
          p1.addVelocityCorrec(velCorrectionP1);
          p2.addPositionCorrec(posCorrectionP2);
          p2.addVelocityCorrec(velCorrectionP2);


        }
      }
    }
  }


}

/** ************************************************************************************************************ */
/** ************************************************************************************************************ */
/** ************************************************************************************************************ */
/** ************************************************************************************************************ */


Engine::Engine() {
  //ctor
  startTime();
}


void Engine::addCollisionPlane(const Plane &p) {
  planeList_.push_back(p);
}


void Engine::draw() {
  particleList_.draw();
}




void Engine::resetForce() {
  for(size_t i=0; i<particleList_.size(); ++i) {
    Particle &p=particleList_[i];
    if (p.isAlive()) {
      p.resetForce();
    }
  }
}





void Engine::updatePositionVelocityCollision() {
  for(size_t i=0; i<particleList_.size(); ++i) {
    Particle &p=particleList_[i];
    if (p.isAlive()) {
      p.velocityCorrection();
      p.positionCorrection();
    }
  }
}


size_t Engine::nbAlive() {
  return particleList_.nbAlive();
}






void Engine::enableWind(const p3d::Line &ray) {
  wind_=ray;
  windEnable_=true;
}

void Engine::disableWind() {
  windEnable_=false;
}

void Engine::initParticle(EParticleMode mode) {
  particleList_.particleMode(mode);
  if (mode==Particle_Point) {
    particleList_.birthRate(10); // nombre de naissances par seconde
    particleList_.maxi(20000); // nombre maxi de particules "vivantes"
  }
  else if (mode==Particle_Sphere) {
    particleList_.birthRate(10);
    particleList_.maxi(20000);
  }
}

double Engine::currentTime() {
  return (std::chrono::duration<double>(std::chrono::steady_clock::now()-startTime_)).count();
}

double Engine::deltaTime() {
  return currentTime()-previousTime_;
}

void Engine::factorTime(double f) {
  factorTime_=f;
}

void Engine::mulFactorTime(double f) {
  factorTime_*=f;
}


double Engine::deltaTimeSimule() {
  return (currentTime()-previousTime_)*factorTime_;
}

void Engine::startTime() {
  startTime_=std::chrono::steady_clock::now();
}

void Engine::texture(Texture *t) {particleList_.texture(t);}


