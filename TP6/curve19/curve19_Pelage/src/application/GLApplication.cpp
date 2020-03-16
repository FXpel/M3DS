/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"

using namespace std;
using namespace p3d;


/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "Hermite" << "Bezier" << "Bezier Interpolation" << "Catmull-Rom" << "";
  leftPanelMenu_ << "Switch Control Points" << "Switch TBN";

  camera2D_.ortho(-1,1,-1,1,1,2);
  camera2D_.position(0,0,0);

  // ************************
  modeDraw_=Draw_Hermite;
  viewControlPoint_=0;

  bezier_.cameraInteract(&camera2D_);
  catmullRom_.cameraInteract(&camera2D_);
  hermite_.cameraInteract(&camera2D_);

  whichInteraction_=&hermite_;


  tAnimation_=0;
  animateTBN_=false;


}

/// dtor
///
GLApplication::~GLApplication() {

}


void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);

  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLTool();
  glPointSize(4.0);
}



/** ***************************************************************************
resize
  **/


void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  camera2D_.viewport(0,0,width,height);
}



void GLApplication::update() {
  Vector2 mouse(mouseX(),mouseY());
  if (mouseLeftPressed()) {
    whichInteraction_->selectNearest(mouse,10);
  }
  if (mouseLeft()) {
    whichInteraction_->moveSelected(mouse);
  }
  if (mouseRightPressed()) {
    whichInteraction_->addEnd(mouse);
  }

  if (animateTBN_) {
    tAnimation_+=0.01;
    if (tAnimation_>1) {
      tAnimation_=0;
    }
  }
}



void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:modeDraw_=Draw_Hermite;whichInteraction_=&hermite_;break;
  case 1:modeDraw_=Draw_Bezier;whichInteraction_=&bezier_;break;
  case 2:modeDraw_=Draw_Catmull;whichInteraction_=&catmullRom_;break;
  case 3:modeDraw_=Draw_Catmull;whichInteraction_=&catmullRom_;catmullRom_.setup(false);break;
  case 5:viewControlPoint_=(viewControlPoint_+1)%2;break;
  case 6:animateTBN_=!animateTBN_;break;
  }
}




void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera2D_);

  printMode();


  switch (modeDraw_) {
  case Draw_Bezier:
    drawBezier();
    break;
  case Draw_Catmull:
    drawCatmullRom();
    break;
  case Draw_Hermite:
    drawHermite();
    break;
  }

  if (animateTBN_) {

    if (modeDraw_==Draw_Catmull) {
      catmullRom_.drawTBN(tAnimation_);
    }
  }


  glUseProgram(0);

  snapshot(); // capture opengl window if requested

}






/** ******************************************************************* **/
/**
  Drawings
**/

void GLApplication::drawBezier() {
  p3d::shaderVertexAmbient();
  glLineWidth(3);
  if (viewControlPoint_==0) {
    p3d::uniformAmbient(Vector4(1,0,0));
    bezier_.drawControl();
  }
  p3d::uniformAmbient(Vector4(0,1,0.3));
  bezier_.draw();
  glLineWidth(1);
}


void GLApplication::drawCatmullRom() {
  p3d::shaderVertexAmbient();
  if (viewControlPoint_==0) {
    p3d::uniformAmbient(1,0,0);
    catmullRom_.drawControl(int(3-viewControlPoint_));
  }
  p3d::uniformAmbient(0,1,0.3);
  catmullRom_.draw();
}

void GLApplication::drawHermite() {
  hermite_.drawControl();
  if (hermite_.nbInput()==4) {
    p3d::uniformAmbient(1,0,0);
    hermite_.draw();
  }
}


void GLApplication::printMode() {
  switch (modeDraw_) {
  case Draw_Bezier:
    p3d::draw("Bezier",Vector2(-1,-1));
    break;
  case Draw_Catmull:
    p3d::draw("Catmull Rom",Vector2(-1,-1));
    break;
  case Draw_Hermite:
    p3d::draw("Hermite",Vector2(-1,-1));
    break;
  }
}







