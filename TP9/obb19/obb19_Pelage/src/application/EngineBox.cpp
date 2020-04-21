#include "EngineBox.h"

#include <ctime>
#include <cstdlib>
#include <iostream>


#include <cmath>

#include "Collision.h"

#include "GLRender.h"
#include "GLDraw.h"
#include "Texture.h"

using namespace p3d;
using namespace std;


/** Application des forces et des moments à chacune des obb
- b->addForce(un Vector3 f) => ajoute la force f à la boite b (appliquée au centre de masse)
- b->addMoment(un Vector3 m) => ajoute le moment m
- si l'attachement par un ressort est actif sur la boite b (_cursorActive == true) :
  - b=_boxList->selected() donne la boite qui subit le ressort (de type Box *)
  - b->attachWorld() donne le point d'attache du ressort de cette boite dans le repère du monde
  - _cursor donne la position de la souris (dans le repère du monde)
**/
void EngineBox::computeForce() {
  double coeff = 0.3;
  for(size_t i=0; i<boxList_.size(); ++i) {
    Box &b=boxList_[i];

    double my_g = b.mass()*9.81;
    Vector3 force = Vector3(0,-my_g,0);
    b.addForce(force);
    b.addForce(-coeff*b.velocity());
    //Vector3 test = Vector3(2,3,5);
    b.addMoment(-coeff*b.omega());

  }
  //Q3
  if(cursorActive_){
        Box &b = boxList_.selected();
        Vector3 l =cursor_-b.attachWorld();
        l = l*3;
        b.addForce(l);
        Vector3 AP = b.position() - b.attachWorld();
        b.addMoment(cross(b.force(),AP));
    }


}



/** Gère la collision entre toutes les boites
**/
void EngineBox::interCollision() {
  for(size_t i=0; i<boxList_.size(); ++i) {
    Box &b1=boxList_[i];
    for(size_t j=i+1; j<boxList_.size(); ++j) {
      Box &b2=boxList_[j];
      Collision c;
      c.collision(b1,b2);
    }
  }
}

/** Intégration Euler
**/
void EngineBox::euler(double dt) {
  for(auto &b:boxList_.all()) {
    b.position(b.position()+dt*b.velocity());
    b.velocity(b.velocity()+dt*b.force()/b.mass());
    // à compléter
    Vector3 old_v = b.omega();
    b.omega(old_v+(b.moment()/b.inertia()*dt));

    double oold = b.theta();
    b.theta(oold+(old_v.z()*dt));

    // à laisser en fin :
    b.resetForce();
    b.resetMoment();
  }
}



/** ***************************************************************************************************************************** **/

EngineBox::EngineBox() {
  //ctor
  startTime();
}

EngineBox::~EngineBox() {
  //dtor
}





void EngineBox::add(const Plane &p) {
  planeList_.push_back(p);
}

void EngineBox::add(const Box &b) {
  boxList_.add(b);
}

void EngineBox::collisionPlane() {

  // pour chacun des plans
  for(auto &plane:planeList_) {
    // pour chacune des boites
    for(auto &b:boxList_.all()) {
      Collision c;
      c.collision(b,plane);

    }

  }
}


void EngineBox::applyCorrection() {
  for(auto &b:boxList_.all()) {
    b.positionCorrection();
    b.velocityCorrection();
    b.omegaCorrection();
  }
}



void EngineBox::update() {
  double dt=deltaTimeSimule();

  computeForce();
  euler(dt);

  interCollision();
  collisionPlane();
  applyCorrection();

  previousTime_=currentTime();
}



void EngineBox::draw() {
  boxList_.draw();
}

void EngineBox::activeCursor(const Vector3 &p) {
  boxList_.select(p);
  if (boxList_.isSelected()) {
    Box &b=boxList_.selected();
    b.enableVisualEffect(2);
    cursor_=p;
    b.attachWorld(p);
    cursorActive_=true;
  } else
    cursorActive_=false;
}

void EngineBox::motionCursor(const Vector3 &p) {
  if (cursorActive_) {
    cursor_=p;
  }
}

void EngineBox::disableCursor() {
  if (boxList_.isSelected()) {
    Box &b=boxList_.selected();
    b.disableVisualEffect(2);
    boxList_.unSelect();
  }
  cursorActive_=false;
}

void EngineBox::drawPlane2D() {
  p3d::ambientColor=Vector4(0.8,0.7,1,1);
  vector<Vector3> pts;
  for(unsigned int i=0; i<planeList_.size(); i++) {
    Plane pl=planeList_[i];
    Vector3 direction=cross(pl.normal(),Vector3(0,0,1));
    pts.push_back((pl.point()-100*direction));
    pts.push_back((pl.point()+100*direction));
  }
  p3d::shaderVertexAmbient();
  p3d::drawLines(pts);
}

bool EngineBox::isCursorActive() {
  return cursorActive_;
}

double EngineBox::deltaTimeSimule() {
  return (currentTime()-previousTime_)*factorTime_;
}

void EngineBox::startTime() {
  startTime_=std::chrono::steady_clock::now();
}

double EngineBox::currentTime() {
  return (std::chrono::duration<double>(std::chrono::steady_clock::now()-startTime_)).count();
}

double EngineBox::deltaTime() {
  return currentTime()-previousTime_;
}

void EngineBox::factorTime(double f) {
  factorTime_=f;
}

void EngineBox::mulFactorTime(double f) {
  factorTime_*=f;
}



void EngineBox::setBoxTexture(size_t i,std::string filename) {
  texture_.push_back(make_unique<Texture>());
  Texture *t=texture_.back().get();
  t->read(filename);
  t->filterLinear();
  boxList_[i].texture(t);

}

