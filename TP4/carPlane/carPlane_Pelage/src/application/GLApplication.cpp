#include "GLApplication.h"
#include "GLTool.h"

#include "Car.h"

#include <QDebug>

#include <iostream>


/*!
*
* @file
*
* @brief
* @author F. Aubert - Univ Lille 1
*
*/

using namespace std;
using namespace p3d;

GLApplication::~GLApplication() {
}

GLApplication::GLApplication() {

  leftPanelMenu_ << "Car Setup" << "Follow Plane" << "Follow Car";

  airplane_.read("cessna.obj");
  airplane_.position(0,10,-10);

  camera_.frustum(-0.01,0.01,-0.01,0.01,0.01,1000);
  camera_.position(Vector3(0,1,10));
  cameraMode_ = Camera_Car_Setup;

  cameraStart_=camera_;
  cameraStop_=camera_;

}




/** ********************************************************************** **/
void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);

//  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLTool();
  p3d::initTorus(20,20,1.0,0.2);

  p3d::diffuseBackColor=Vector3(0,1,0);

  ground_.readMipmap("mosaic_pierre.jpg");


  airplane_.initDraw();

}

void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  glViewport(0,0,width,height);
  camera_.viewport(0,0,width,height);
}

void GLApplication::update() {
  // appelée toutes les 20ms (60Hz)
  // => mettre à jour les données de l'application
  // avant l'affichage de la prochaine image (animation)
  // ...


  if (keyPressed(Qt::Key_F)) cameraMode_=Camera_Car_Setup;
  if (keyPressed(Qt::Key_G)) cameraMode_=Camera_Follow_Plane;
  if (keyPressed(Qt::Key_H)) cameraMode_=Camera_Follow_Car;

  car_.decelerate();
  if (cameraMode_==Camera_Follow_Car || cameraMode_==Camera_Car_Setup) {
    if (left()) {car_.turnLeft();}
    if (right()) {car_.turnRight();}
    if (forward()) {car_.accelerate();}
    if (backward()) {car_.brake();}

  }
  else {
    if (left()) airplane_.rollLeft();
    if (right()) airplane_.rollRight();
    if (forward()) airplane_.pitchDown();
    if (backward()) airplane_.pitchUp();
    if (rotateLeft()) airplane_.yawLeft();
    if (rotateRight()) airplane_.yawRight();
    if (accelerateWheel()) airplane_.accelerate();
    if (decelerateWheel()) airplane_.decelerate();
  }



  car_.move();
  airplane_.move();
  updateCamera();

}

void GLApplication::draw() {
  // appelée après chaque update
  // => tracer toute l'image
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera_);

  car_.drawWorld();

  p3d::diffuseColor=Vector3(1,0,0);
  airplane_.drawWorld();
  drawGround();

  glUseProgram(0);


  snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(size_t i,[[maybe_unused]] const std::string &s) {
  switch (i) {
  case 0:cameraMode_=Camera_Car_Setup;
      break;
  case 1:cameraMode_=Camera_Follow_Plane;
      break;
  case 2:cameraMode_=Camera_Follow_Car;
      break;
  }
  cameraStart_=camera_;
  lambda_=0;
}

/** *********************************************************** **/

void GLApplication::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::textureMatrix.push();
  p3d::textureMatrix.scale(135,135,135);
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.scale(10,10,10);
  p3d::ambientColor=Vector4(0.2,0.2,0.2,1);
  p3d::shaderTextureLight();
  p3d::drawGround(ground_);
  p3d::textureMatrix.pop();
  p3d::modelviewMatrix.pop();
}

void GLApplication::updateCamera() {
  lambda_+=0.02;
  if (lambda_>1) lambda_=1;


  switch (cameraMode_) {
    case Camera_Car_Setup:
      if (mouseLeft()) {
        camera_.lookAt(car_.position());
        Vector3 t=camera_.pointTo(Coordinate_Local,car_.position());
        Vector3 vertical=camera_.directionTo(Coordinate_Local,Vector3(0,1,0));
        camera_.translate(t,Coordinate_Local);
        camera_.rotate(-deltaMouseX(),vertical,Coordinate_Local);
        camera_.rotate(deltaMouseY(),Vector3(1,0,0),Coordinate_Local);
        camera_.translate(-t,Coordinate_Local);
      }
      break;
    case Camera_Follow_Car: {

      }
      break;
    case Camera_Follow_Plane: {

        camera_.position(airplane_.position()+Vector3(0,1,-3));
        camera_.orientation(180,Vector3(0,1,0));

      }
      break;
  }
}


