#include "ParticleList.h"

#include "Texture.h"

#include <ctime>
#include <cstdlib>

#include "GLTool.h"

using namespace std;
using namespace p3d;


/** random number in [a,b] **/
double randDouble(double a, double b) {
  double c=double(rand())/RAND_MAX;
  return c*(b-a)+a;
}

/** random number in [0,1] **/
double randUnit() {
  return double(rand())/RAND_MAX;
}



/** Etat initial d'une particule (position/vitesse à la naissance)
**/
void ParticleList::initState(Particle *p) {

  if (particleMode_==Particle_Point) {
    p->position(Vector3(randDouble(-1,1),randDouble(-1,1),0));
    p->velocity(Vector3(randDouble(-1,1),randDouble(0,5),randDouble(-1,1)));
    p->timeToLive(randDouble(4,10)); // entre 4 et 10 secondes
    p->mass(1.0);

    p->color(randDouble(0,1),randDouble(0,1),0.8);
  }
  else if (particleMode_==Particle_Sphere) {
//    p->position(Vector3(randDouble(-1,1),10,randDouble(-1,1)));
      p->position(Vector3(randDouble(-1,1),5,randDouble(-1,1)));
    p->velocity(Vector3(0,0,0));
    p->timeToLive(randDouble(4,10)); // entre 4 et 10 secondes
    p->mass(1.0);

    p->color(randDouble(0,1),randDouble(0,1),0.8);
  }
  p->birth();
}



ParticleList::ParticleList() {
  //ctor
}

void ParticleList::texture(Texture *t) {
  texture_=t;
}


void ParticleList::draw() {

  texture_->bind();


  for(size_t i=0;i<size();++i) {
    if (particle_[i].isAlive()) {
      if (particleMode_==Particle_Point) {
        particle_[i].drawPoint();
      }
      else if (particleMode_==Particle_Sphere) {
        particle_[i].drawSphere();
      }
    }
  }

}

void ParticleList::birthRate(double nb) {
  nbBirthPersec_=nb;
}

void ParticleList::updateLife(double deltaTime) {
  remainingBirth_+=deltaTime*nbBirthPersec_;
  while ((remainingBirth_>=1) && (deadParticle_.size()>0)) {
    Particle *newp=deadParticle_.front();
    deadParticle_.pop_front();
    initState(newp);
    remainingBirth_-=1;
  }


  for(unsigned int i=0;i<size();i++) {
    Particle &p=particle_[i];
    if (p.isAlive()) {
      p.updateLife(deltaTime);
      if (!p.isAlive()) {
        deadParticle_.push_back(&p);
      }
    }
  }
}

void ParticleList::drawData() {
  for(unsigned int i=0;i<size();i++) {
    Particle &p=particle_[i];
    glColor3f(0,0,1);
    p3d::drawArrow(p.position(),Vector3(2.0*p.velocity()),0.2,"","Vitesse");
    glColor3f(0,1,0);
    if (p.force().length()>0.1)
    p3d::drawArrow(p.position(),Vector3(10.0*p.force()),0.2,"","Force");
  }
}

void ParticleList::maxi(unsigned int nb) {
  particle_.clear();
  deadParticle_.clear();
  particle_.resize(nb);
  for(size_t i=0;i<nb;++i) {
    deadParticle_.push_back(&particle_[i]);
  }
}

size_t ParticleList::nbAlive() {
  return particle_.size()-deadParticle_.size();
}


void ParticleList::link(size_t i,size_t j) {
  i=0;
  j=0;
}



