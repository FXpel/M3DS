/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include <iostream>

#include "Plane.h"

#include "GLTool.h"
#include "DebugTool.h"

#include "Collision.h"


using namespace std;
using namespace p3d;


/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "Visual Debug";


  _camera.ortho(-10,10,-10,10,-10,10);
  _camera.position(Vector3(0,0,-2));


  // ajout des plans de collisions (les bords de l'environnement)
  engineBox_.add(Plane(Vector3(0,-9,0),Vector3(0,1,0)));
  engineBox_.add(Plane(Vector3(-9,0,0),Vector3(1,0,0)));
  engineBox_.add(Plane(Vector3(9,0,0),Vector3(-1,0,0)));
  engineBox_.add(Plane(Vector3(0,9,0),Vector3(0,-1,0)));


  // Définition et ajout première boite
  Box b;
  b.dimension(3,2);
  b.position(Vector3(-2,5,0));
  b.theta(-0.3);
  b.mass(1);
  b.computeInertia();
  b.color(Vector3(1,0,1));
  engineBox_.add(b);

  // Définition et ajout seconde boite
  b.dimension(10,2);
  b.position(Vector3(3,0,0));
  b.theta(0.5);
  b.mass(1);
  b.computeInertia();
  b.color(Vector3(0,1,0));
  engineBox_.add(b);



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


  glPixelStorei(GL_UNPACK_ALIGNMENT,4);
  glPixelStorei(GL_PACK_ALIGNMENT,4);


  engineBox_.setBoxTexture(0,"Tyrol.jpg"); // texture pour la boite 0
  engineBox_.setBoxTexture(1,"Tranquil_Lagoon.jpg"); // texture pour la boite 1



}



/** ***************************************************************************
resize
  **/


void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  _camera.viewport(0,0,width,height);
}





void GLApplication::update() {
//  updateCamera();


  engineBox_.update();

  if (mouseLeftPressed()) {
    applyForce_=true;
    pointForce_=_camera.windowToWorld(mouseX(),mouseY());
    pointForce_.z(0);
    engineBox_.activeCursor(pointForce_);
  }
  if (mouseLeft()) {
    pointForce_=_camera.windowToWorld(mouseX(),mouseY());
    pointForce_.z(0);
    engineBox_.motionCursor(pointForce_);
  }
  else {
    applyForce_=false;
    engineBox_.disableCursor();
  }

  if (keyPressed(Qt::Key_A)) {
    engineBox_.mulFactorTime(0.9);
  }

  if (keyPressed(Qt::Key_E)) {
    engineBox_.mulFactorTime(1.1);
  }

  if (keyPressed(Qt::Key_Space)) {
    animate_?engineBox_.factorTime(0):engineBox_.factorTime(1);
    animate_=!animate_;
  }

}

void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:visualDebug_=!visualDebug_;Collision::visualDebug(visualDebug_);break;
  }

}




void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(_camera);


  engineBox_.draw();

  engineBox_.drawPlane2D();

  glDepthFunc(GL_ALWAYS);
  if (engineBox_.isCursorActive()) {
    p3d::ambientColor=Vector4(0,0,1,0);
    p3d::drawThickLineStrip({pointForce_,engineBox_.boxList().selected().attachWorld()});
  }

  if (visualDebug_) {
    p3d::drawDebug();
    p3d::clearDebug();
  }
  glDepthFunc(GL_LESS);


  glUseProgram(0);

  snapshot(); // capture opengl window if requested

}

/** ********************************************************************************
 *
 *
**/








