#include "Particle.h"
#include "Quaternion.h"
#include "GLTool.h"

/**
@file
@author F. Aubert
@brief Particle

*/

#include <ctime>
#include <cstdlib>


using namespace std;
using namespace p3d;







/** *********************************************************** **/


Particle::Particle() {
  //ctor
  mass_=1;
  velocity_=Vector3(0,0,0);
  isAlive_=false;
  radius_=0.5;

  positionCorrection_=Vector3(0,0,0);
  velocityCorrection_=Vector3(0,0,0);
}

Particle::~Particle() {
  //dtor
}


Vector3 Particle::position() const {
  return position_;
}

Vector3 Particle::velocity() const {
  return velocity_;
}


void Particle::position(const Vector3 &p) {
  position_=p;
}

void Particle::velocity(const Vector3 &v) {
  velocity_=v;
}

void Particle::kill() {
  isAlive_=false;
}

void Particle::birth() {
  birthDate_=double(clock())/CLOCKS_PER_SEC;
  isAlive_=true;
}


double Particle::mass() const {
  return mass_;
}

void Particle::mass(double m) {
  mass_=m;
}


void Particle::drawSphere() const {
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.translate(position_.x(),position_.y(),position_.z());
  p3d::diffuseColor=Vector3(color_.x(),color_.y(),color_.z());
  p3d::modelviewMatrix.scale(radius_,radius_,radius_);
  p3d::shaderLightPhong();
  p3d::drawSphere();
  p3d::modelviewMatrix.pop();
}


void Particle::drawPoint() const {

//  glDepthMask(GL_FALSE);
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.translate(position_);
  Vector3 t=(p3d::modelviewMatrix.column(3)).xyz();
  p3d::modelviewMatrix=Matrix4::fromTranslation(t);
  p3d::modelviewMatrix.scale(radius_/5.0);

  p3d::ambientColor=Vector4(color_,0.5);

  p3d::shaderTextureAmbient();
  p3d::drawSquare();
  p3d::modelviewMatrix.pop();

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

}

void Particle::updateLife(double deltaTime) {
  timeLeft_-=deltaTime;
  if (timeLeft_<0.0) {
      kill();
  }
}


bool Particle::isAlive() const {
  return isAlive_;
}

void Particle::addForce(const Vector3 &f) {
  cumulForce_+=f;
}

Vector3 Particle::force() {
  return cumulForce_;
}

void Particle::resetForce() {
  cumulForce_=Vector3(0,0,0);
}

void Particle::timeToLive(double t) {
  timeLeft_=t;
}

double Particle::timeToLive() {
  return timeLeft_;
}

double Particle::radius() {
  return radius_;
}

void Particle::radius(double r) {
  radius_=r;
}

void Particle::addVelocityCorrec(const Vector3 &v) {
  velocityCorrection_+=v;
}

void Particle::addPositionCorrec(const Vector3 &p) {
  positionCorrection_+=p;
}



void Particle::velocityCorrection() {
  velocity_+=velocityCorrection_;
  velocityCorrection_=Vector3(0,0,0);
}

void Particle::positionCorrection() {
  position_+=positionCorrection_;
  positionCorrection_=Vector3(0,0,0);
}



