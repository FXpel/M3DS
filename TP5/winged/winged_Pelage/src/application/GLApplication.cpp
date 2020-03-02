#include "GLApplication.h"
#include "GLRender.h"
#include "GLTool.h"
#include <iostream>

#include "WEdge.h"

/**

  @author F. Aubert
  **/

using namespace std;
using namespace p3d;

GLApplication::~GLApplication() {
}

GLApplication::GLApplication() {

  leftPanelMenu_ << "Draw OBJ" << "WFace::draw (wire)" << "Show/Hide Vertex Normal" << "WFace::draw (light)" << "Boundary" << "Next Object";


  camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  camera_.position(Vector3(0,0,10));
  camera_.lookAt(Vector3(0,0,0));

  nbObject_=wingedObject_.size();

  visuPoint_=false;
  visuBoundary_=false;
  visuHide_=0;
  computeVertexNormal_=false;
  drawLineCatmull_=false;
  boundary_.clear();
  active_=0;

  wingedObject_[0].read("002_cube.obj");
  wingedObject_[1].read("cow.obj");
  wingedObject_[2].read("al.obj");

  visuNormal_=false;

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

  ground_.readMipmap("mosaic_pierre.jpg");

  for(unsigned int i=0;i<nbObject_;++i) {
    wingedObject_[i].initLineAttrib();
  }

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

  updateCamera();

  switch(keyPressed()) {
  case Qt::Key_C:
    wingedObject_[active_].catmullClark();
    wingedObject_[active_].computeNormalVertex();
    wingedObject_[active_].stats();
    break;
  case Qt::Key_V:
    wingedObject_[active_].computePointFace();

    wingedObject_[active_].computePointEdge();

    wingedObject_[active_].computePointVertex();

    break;
  case Qt::Key_P:
    visuPoint_=!visuPoint_;
    break;
  case Qt::Key_B:
    visuBoundary_=!visuBoundary_;
    if (visuBoundary_) boundary_=wingedObject_[active_].boundary();

    break;
  case Qt::Key_N:
    wingedObject_[active_].computeNormalVertex();
    break;
  case Qt::Key_G:
    visuHide_=(visuHide_+1)%3;
    break;
  case Qt::Key_H:
    drawLineCatmull_=!drawLineCatmull_;
    break;
  case Qt::Key_Less:
    active_=(active_+1)%nbObject_;
    visuBoundary_=false;
    break;
  case Qt::Key_Greater:
    if (active_==0) active_=nbObject_-1;
    else active_=(active_-1);
    visuBoundary_=false;
    break;
  }
}

void GLApplication::draw() {
  // appelée après chaque update
  // => tracer toute l'image
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera_);

  drawGround();


  if (visuBoundary_) {
    glLineWidth(4);
    p3d::ambientColor=Vector4(1,0,0);
    shaderVertexAmbient();
    for(size_t i=0; i<boundary_.size(); i++) {
      boundary_[i]->drawLine();
    }
  }

  drawWinged();

  glUseProgram(0);


  snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(size_t i,[[maybe_unused]]const std::string &s) {
//  cout << "GLApplication : button clicked " << i << " " << s << endl;

  switch (i) {
    case 0:case 1:
      visuHide_=i;
      break;
    case 2:
      visuNormal_=!visuNormal_;wingedObject_[active_].computeNormalVertex();
      break;
    case 3:
      visuHide_=2;
      wingedObject_[active_].computeNormalVertex();
      break;
    case 4:
      visuBoundary_=!visuBoundary_;
      if (visuBoundary_) boundary_=wingedObject_[active_].boundary();
      break;
    case 5:
      active_=(active_+1)%3;
      if (visuHide_==2 || visuNormal_) {
        wingedObject_[active_].computeNormalVertex();
      }
      break;
    default:break;
  }

}



/** *********************************************************** **/

void GLApplication::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::textureMatrix.push();
  p3d::textureMatrix.scale(135,135,135);
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.scale(10,10,10);
  p3d::drawGround(ground_);
  p3d::textureMatrix.pop();
  p3d::modelviewMatrix.pop();
}

void GLApplication::updateCamera() {
  if (mouseLeft()) {
//    _camera.lookAt(Vector3(0,0,0));
    Vector3 t=camera_.pointTo(Coordinate_Local,Vector3(0,0,0));
    Vector3 vertical=camera_.directionTo(Coordinate_Local,Vector3(0,1,0));
    camera_.translate(t,Coordinate_Local);
    camera_.rotate(-deltaMouseX(),vertical,Coordinate_Local);
    camera_.rotate(deltaMouseY(),Vector3(1,0,0),Coordinate_Local);
    camera_.translate(-t,Coordinate_Local);
  }
  if (accelerateWheel()) {
    frustum_*=1.05;
    camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  }
  if (decelerateWheel()) {
    frustum_/=1.05;
    camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  }
  if (left()) camera_.left(0.2);
  if (right()) camera_.right(0.2);
  if (forward()) camera_.forward(0.2);
  if (backward()) camera_.backward(0.2);
}








/** ******************************************************************* **/
/**
  Drawings
**/


void GLApplication::drawWinged() {
  p3d::materialBlueGreen();
  glLineWidth(2);
  if (drawLineCatmull_) {
    wingedObject_[active_].drawLineCatmull();
  }
  else {
    switch(visuHide_) {
    case 0:
      p3d::shaderVertexAmbient();
      wingedObject_[active_].drawWire(Vector4(0,0,0));
      break;
    case 1:
      p3d::ambientColor=Vector4(0,0.4,0.9,1);
      p3d::shaderVertexAmbient();
      wingedObject_[active_].draw(false);
      break;
    case 2:
      p3d::shaderLightPhong();
      wingedObject_[active_].draw(true);
      break;
    }
  }
  if (visuPoint_) {
    p3d::ambientColor=Vector4(0,0,1,1);
    wingedObject_[active_].drawPointFace();
    p3d::ambientColor=Vector4(1,0,0,1);
    wingedObject_[active_].drawPointEdge();
    p3d::ambientColor=Vector4(0,1,0,1);
    wingedObject_[active_].drawPointVertex();
  }
  if (visuNormal_) {
    wingedObject_[active_].drawNormal();
  }
}



