#include "GLApplication.h"

#include "GLRender.h"
#include "Tools.h"

#include <iostream>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


using namespace p3d;
using namespace std;

GLApplication::~GLApplication() {
  makeCurrent();
  p3d::releaseGLRender();
}

GLApplication::GLApplication() {
  leftPanelMenu_ << "Sphere" << "Cube" << "Revolution" << " " << "Wire" << "Light" << "Texture" << "" << "Input Profile";

  camera_.position(0,2,5);
  camera_.lookAt(Vector3(0,0,0));

  revolution_.cameraInteract(&cameraInput_);


}

void GLApplication::initialize() {
  glClearColor(1,1,1,1);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  p3d::initGLRender();

  shaderVertexAmbient();

  glPointSize(5.0);

  glFrontFace(GL_CCW);

  revolution_.init();



}


void GLApplication::apply(const p3d::Camera &c) {
  p3d::projectionMatrix=c.projectionMatrix();
  p3d::modelviewMatrix=c.cameraWorld();
  glViewport(c.viewX(),c.viewY(),c.viewWidth(),c.viewHeight());
}


void GLApplication::drawScene() {

  p3d::modelviewMatrix.push();

  revolution_.draw(shaderMode_);

  p3d::modelviewMatrix.pop();

}




/** ***************************************************************** **/
void GLApplication::resize(int width,int height) {
  camera_.viewport(0,0,width,height);
  double ratio=double(width)/height;
  camera_.frustum(-0.01*ratio,0.01*ratio,-0.01,0.01,0.03,100.0);

  cameraInput_.viewport(0,0,width,height);
  cameraInput_.ortho(-1*ratio,1*ratio,-1,1,0,1);
}


void GLApplication::update() {
  if (viewMode_==View_Input) {
    Vector2 mouse(mouseX(),mouseY());
    if (mouseLeftPressed()) {
      revolution_.selectNearest(mouse,10);
    }
    if (mouseLeft()) {
      revolution_.moveSelected(mouse);
    }
    if (mouseRight()) {
      revolution_.addEnd(mouse);
    }
    if (keyPressed(Qt::Key_C)) {
      revolution_.clearProfile();
    }
  }
  else {
    updateCamera();
  }
}


void GLApplication::updateCamera() {
  if (left()) {
    camera_.translate(-0.1,0,0,Coordinate_Local);
  }
  if (right()) {
    camera_.translate(0.1,0,0,Coordinate_Local);
  }
  if (forward()) {
    camera_.translate(0,0,-0.1,Coordinate_Local);
  }
  if (backward()) {
    camera_.translate(0,0,0.1,Coordinate_Local);
  }
  if (mouseLeft()) {
    // rotate camera : deltaMouseX(), deltaMouseY() give the mouse motion
    Vector3 vertical=camera_.directionTo(Coordinate_Local,Vector3(0,1,0));
    auto t=camera_.pointTo(Coordinate_Local,Vector3(0,0,0));
    camera_.translate(t,Coordinate_Local);
    camera_.rotate(-deltaMouseX()/5.0,vertical,Coordinate_Local);
    camera_.rotate(deltaMouseY()/5.0,1,0,0,Coordinate_Local);
    camera_.translate(-t,Coordinate_Local);
  }

  p3d::lightPosition[0]=Vector4(0,0,0,1);
}


void GLApplication::draw() {
  switch (requestInit_) {
  case Init_Sphere:revolution_.initSphere();break;
  case Init_Cube:revolution_.initCube();break;
  case Init_Revolution:revolution_.initRevolution();break;
  default:break;
  }
  requestInit_=Init_None;


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


  if (viewMode_==View_3D) {
    apply(camera_);
    drawScene();
  }
  else {
    apply(cameraInput_);
    revolution_.drawProfile();
  }

}


void GLApplication::leftPanel(size_t i,const std::string &s) {
  if (i!=8) viewMode_=View_3D;
  switch(i) {
  case 1:requestInit_=Init_Cube;break;
  case 0:requestInit_=Init_Sphere;break;
  case 2:requestInit_=Init_Revolution;break;

  case 4:shaderMode_=Revolution::Shader_Ambient;break;
  case 5:shaderMode_=Revolution::Shader_Light;break;
  case 6:shaderMode_=Revolution::Shader_Texture;break;

  case 8:viewMode_=View_Input;break;
  }
}

