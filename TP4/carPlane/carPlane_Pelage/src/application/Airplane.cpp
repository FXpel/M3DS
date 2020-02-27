#include "Airplane.h"
#include "GLTool.h"
#include "Tools.h"

using namespace p3d;
using namespace std;

Airplane::Airplane() {
  increment_=1.0;
  accelerate_=0.1;
  velocity_=0.0;
  orientation_.setIdentity();
  position_=Vector3(0,0,0);
  angleX_=angleY_=angleZ_=0.0;
}

Airplane::~Airplane() {
  delete objGL_;

}

void Airplane::initDraw() {
  delete objGL_;
  objGL_=new MeshGL(&obj_);
  objGL_->initVAO();
}


void Airplane::draw() {

  p3d::diffuseColor=Vector3(1,0,0);
  p3d::shaderLightPhong();
  objGL_->draw();
}

void Airplane::drawWorld() {
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.translate(position_);

//  p3d::modelviewMatrix.rotate(angleY_,0,1,0); // yaw
//  p3d::modelviewMatrix.rotate(angleX_,1,0,0); // pitch
//  p3d::modelviewMatrix.rotate(angleZ_,0,0,1); // roll
  p3d::modelviewMatrix.rotate(orientation_);

  p3d::modelviewMatrix.scale(2,2,2); // grossir un peu l'avion
  draw();

  p3d::modelviewMatrix.pop();
}

void Airplane::move() {
//    Vector3 uneDirection = *new Vector3(0, 0, 1); // on avance sur l'axe x
//    uneDirection = orientation_ * uneDirection; // orientation influe sur la direction de l'avion
//    uneDirection = uneDirection * velocity_ ; // vitesse de l'avion
//    position_ = uneDirection + uneDirection; // déplacement de l'avion
    position_ += orientation_ * Vector3(0,0,1) * velocity_;
}

void Airplane::pitchDown() {
//  angleX_+=increment_;
    orientation_.rotate(increment_,Vector3(1,0,0)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de X).



}

void Airplane::pitchUp() {
//  angleX_-=increment_;
    orientation_.rotate(increment_,Vector3(-1,0,0));


}

void Airplane::rollRight() {
//  angleZ_+=increment_;
    orientation_.rotate(increment_,Vector3(0,0,1));

}

void Airplane::rollLeft() {
//  angleZ_-=increment_;
    orientation_.rotate(increment_,Vector3(0,0,-1));
}

void Airplane::yawLeft() {
//  angleY_+=increment_;
    orientation_.rotate(increment_,Vector3(0,1,0));

}

void Airplane::yawRight() {
//  angleY_-=increment_;
    orientation_.rotate(increment_,Vector3(0,-1,0));
}

void Airplane::accelerate() {
  velocity_+=accelerate_;
  if (velocity_>3) velocity_=3;
}

void Airplane::decelerate() {
  velocity_-=accelerate_;
  if (velocity_<0) velocity_=0;
}

// lecture fichier .obj
void Airplane::read(const string &filename) {
  obj_.readInit(filename);
  obj_.rotateY(90);
}



