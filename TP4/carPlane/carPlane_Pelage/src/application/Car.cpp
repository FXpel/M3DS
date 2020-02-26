#include <math.h>
#include "Car.h"
#include "GLTool.h"
#include "GLRender.h"

using namespace p3d;
using namespace std;


// ************************************************************
Car::~Car() {
}

Car::Car() {
}

// ******************************************************************
// méthodes à compléter lors du TP
void Car::drawRim() {
    //premier cyl horiz
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.scale(0.2,0.2,3);
    drawCylinder();
    p3d::modelviewMatrix.pop();

    //deuxième cyl vert
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(90,1,0,0);
    p3d::modelviewMatrix.scale(0.2,0.2,3);
    p3d::modelviewMatrix.translate(0,7.5,-0.5);

    drawCylinder();
    p3d::modelviewMatrix.pop();

    //troisième cyl
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(45,1,0,0);
    p3d::modelviewMatrix.scale(0.2,0.2,3);
    p3d::modelviewMatrix.translate(0,5,-0.15);
    drawCylinder();
    p3d::modelviewMatrix.pop();

    //quatrième cyl
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(-45,1,0,0);
    p3d::modelviewMatrix.scale(0.2,0.2,3);
    p3d::modelviewMatrix.translate(0,-5,-0.15);
    drawCylinder();
    p3d::modelviewMatrix.pop();
}

void Car::drawWheel() {
    p3d::modelviewMatrix.push();
    drawRim();
    p3d::modelviewMatrix.pop();
}

void Car::drawAxle() {

}

void Car::drawBody() {
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.scale(1,1,3);
    p3d::diffuseColor=Vector3(0.4,0,1);
    drawCube();
    p3d::modelviewMatrix.pop();

    //deuxième cube
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(0,1,2);// 1height 2 depth
    p3d::modelviewMatrix.scale(1,2,1);

    drawCube();


    p3d::modelviewMatrix.pop();
}

void Car::draw() {
  p3d::modelviewMatrix.push();



  drawWheel();

  p3d::modelviewMatrix.pop();
}


void Car::drawWorld() {

  p3d::modelviewMatrix.push();

  draw(); // tracé de la voiture dans son repère local
  p3d::modelviewMatrix.pop();
}

void Car::move() {
  acceleration_+=-velocity_/50;
  velocity_+=acceleration_;
  rotateWheel_+=velocity_*20;
  steering_-=steering_/10*fabs(velocity_);

  orientation_.rotate(steering_*velocity_/(1.0+fabs(velocity_)),Vector3(0,1,0)); // le /(1.0+fabs(_velocity)) a été déterminé empiriquement

}


void Car::accelerate() {
  acceleration_=0.05;

}

void Car::decelerate() {
  acceleration_=0;
}

void Car::brake() {
  acceleration_=-0.02;

}

void Car::turnLeft() {
  steering_+=0.5;
  if (steering_>35) steering_=35;
}

void Car::turnRight() {
  steering_-=0.5;
  if (steering_<-35) steering_=-35;
}



/***************************************************************/

void Car::drawCube() {p3d::shaderLightPhong();p3d::drawCube();}
void Car::drawTorus() {p3d::shaderLightPhong();p3d::drawTorus();}
void Car::drawSphere() {p3d::shaderLightPhong();p3d::drawSphere();}
void Car::drawCone() {p3d::shaderLightPhong();p3d::drawCone();}
void Car::drawCylinder() {p3d::shaderLightPhong();p3d::drawCylinder();}

void Car::rotateWheel(double angle) {rotateWheel_+=angle;}

Vector3 Car::position() {return position_;}
Quaternion Car::orientation() {return orientation_;}

