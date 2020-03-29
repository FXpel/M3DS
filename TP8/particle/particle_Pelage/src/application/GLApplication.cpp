/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"
#include <iostream>


using namespace std;
using namespace p3d;

/// dtor
///
GLApplication::~GLApplication() {

}


/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "Particle" << "Sphere" << "Animate";


  frustum_=0.1;
  camera_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  camera_.position(Vector3(0,0,10));
  camera_.lookAt(Vector3(0,0,0),Vector3(0,1,0));

  animate_=true;



  engine_.initParticle(Particle_Point);

  // Ajout de plans (gérés par la détection de collision) :
  // le sol :
  engine_.addCollisionPlane(Plane(Vector3(0,-3,0),Vector3(0,1,0)));

  // des "murs" (non visibles)

  engine_.addCollisionPlane(Plane(Vector3(-4,0,0),Vector3(1,0,0)));
  engine_.addCollisionPlane(Plane(Vector3(4,0,0),Vector3(-1,0,0)));
  engine_.addCollisionPlane(Plane(Vector3(0,0,-4),Vector3(0,0,1)));
  engine_.addCollisionPlane(Plane(Vector3(0,0,4),Vector3(0,0,-1)));




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


  particleTexture_.read("lightmap.png");
  particleTexture_.setAlpha(Vector4(0,0,0,0),0.0,0.1);

  engine_.texture(&particleTexture_);



  groundTexture_.read("mosaic_pierre.jpg");
  groundTexture_.generateMipmap();


}



/** ***************************************************************************
resize
  **/


void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  camera_.viewport(0,0,width,height);
}





void GLApplication::update() {
  updateCamera();

  if (mouseRight()) {
    windRay_=camera_.windowToRayWorld(Vector2(mouseX(),mouseY()));
    engine_.enableWind(windRay_);
  }
  else {
    engine_.disableWind();
  }
  engine_.update();

  if (keyPressed(Qt::Key_A)) {
    engine_.mulFactorTime(0.7);
  }

  if (keyPressed(Qt::Key_E)) {
    engine_.mulFactorTime(1.2);
  }

  if (keyPressed(Qt::Key_Space)) {
  }
}

void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:
    engine_.initParticle(Particle_Point);
    break;
  case 1:
    engine_.initParticle(Particle_Sphere);
    break;
  case 2:
    animate_=!animate_;
    if (animate_) engine_.factorTime(1);
    else engine_.factorTime(0);
    break;
  }

}

void GLApplication::updateCamera() {
  if (mouseLeft()) {
    Vector3 center=camera_.pointTo(Coordinate_Local,Vector3(0,0,0));
    Vector3 vertical=Vector3(0,1,0);
    camera_.translate(center,Coordinate_Local);
    camera_.rotate(-deltaMouseX()/2.0,vertical,Coordinate_World);
    camera_.rotate(deltaMouseY()/2.0,Vector3(1,0,0),Coordinate_Local);
    camera_.translate(-center,Coordinate_Local);
  }
  if (left()) camera_.left(0.3);
  if (right()) camera_.right(0.3);
  if (forward()) camera_.forward(0.3);
  if (backward()) camera_.backward(0.3);
  if (accelerateWheel()) {
    frustum_*=1.05;
    camera_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
  if (decelerateWheel()) {
    frustum_/=1.05;
    camera_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
}




void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera_);


  drawGround();
  engine_.draw();


  glUseProgram(0);

  snapshot(); // capture opengl window if requested

}

/** ********************************************************************************
 *
 *
**/



void GLApplication::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::textureMatrix.push();
  p3d::textureMatrix.scale(135,135,135);
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.scale(10,10,10);
  p3d::ambientColor=Vector4(0.1,0.1,0.1,1.0);
  p3d::shaderTextureLight();
  p3d::drawGround(groundTexture_);
  p3d::textureMatrix.pop();
  p3d::modelviewMatrix.pop();
}







