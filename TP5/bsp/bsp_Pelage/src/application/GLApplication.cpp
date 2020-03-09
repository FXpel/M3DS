/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLRender.h"
#include "GLDraw.h"
#include "GLDrawCamera.h"
#include "GLText.h"
#include <iostream>

using namespace std;
using namespace p3d;

static double myRand(double binf,double bsup) {
  return double(rand())/RAND_MAX*(bsup-binf)+binf;
}

/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "Face Side" << "Segment Intersection" << "Face cutting" << "Build BSP" << "BSP Visualisation" << "Transparency";

  camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  camera_.position(0,2,5);
  camera_.lookAt(Vector3(0,0,0));

  nbSolution_=3;
  zCam_=6.0;
  modeDisplay_=1;
  solution_=0;

  srand(unsigned(time(nullptr)));

  // pour test signe et intersection
  bsp1_.read("une_face.obj");

  // pour test de découpe sur 2 faces
  bsp2_.read("deux_face.obj");


  bsp3_.read("simple_nontri.obj");
//  bsp3_.read("wolf.obj");
}

/// dtor
///
GLApplication::~GLApplication() {

}


void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);

//  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLRender();
  glPointSize(4.0);
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

}

void GLApplication::leftPanel(size_t i,[[maybe_unused]]const std::string &s) {
//  cout << "GLApplication : button clicked " << i << " " << s << endl;

  switch (i) {
  case 0:case 1:case 2:
    modeDisplay_=i+1;
    break;
  case 4:
    modeDisplay_=4;
    if (bsp3_.bsp()) solution_=(solution_+1)%nbSolution_; else solution_=0;
    break;
  case 3:
    if (!bsp3_.bsp()) {
      cout << "BSP construction" << endl;
      cout << "Number of faces = " << bsp3_.nbFaceInit() << endl;
      bsp3_.consBSP();
      cout << "After creation : number of faces = " << bsp3_.nbFaceBSP() << endl;
      modeDisplay_=4;
      solution_=0;
    }
    break;
  case 5:
    withBlend_=!withBlend_;
    break;
  default:break;
  }

}





/** ******************************************************************* **/
/**
  Drawings
**/

void GLApplication::display1() {

  // Test du signe (génération brutale à chaque passage => uniquement pour tester).
  bsp1_.clearDraw();
  bsp1_.addDrawAllInit();
  bsp1_.drawDepth();
  srand(1);
  p3d::GLText::draw("Face sign...",-1,-1);
  p3d::shaderVertexAmbient();
  vector<Vector3> pts;
  vector<Vector3> color;
  pts.clear();color.clear();
  for(int i=0; i<200; i++) {
    Vector3 p;
    p.x(myRand(-1.0,1.0));
    p.y(myRand(-1.0,1.0));
    p.z(myRand(-1.0,1.0));
    pts.push_back(p);
    FaceBSP f=bsp1_.face(0);
    ESign s=f.sign(p);
    switch (s) {
    case Sign_None:
      color.push_back(Vector3(0,0,1));
      break;
    case Sign_Plus:
      color.push_back(Vector3(0,1,0));
      break;
    case Sign_Minus:
      color.push_back(Vector3(1,0,0));
      break;
    }
  }
  p3d::shaderVertexColor();
  p3d::drawPoints(pts,color);
  p3d::shaderVertexAmbient();
//  _bsp1.drawNormal();

}

void GLApplication::display2() {
  bsp1_.drawDepth();
  srand(1);
  vector<Vector3> ptsLine,ptsPoint;
  p3d::GLText::draw("Segment intersection...",-1,-1);
  for(size_t i=0; i<10; ++i) {
    Vector3 p1,p2;
    p1=Vector3(myRand(-2.0,0.0),myRand(-1.0,1.0),myRand(0.0,2.0));
    p2=Vector3(myRand(0.0,2.0),myRand(-1.0,1.0),myRand(-2.0,0.0));
    ptsLine.push_back(p1);
    ptsLine.push_back(p2);
    VertexBSP v=bsp1_.face(0).intersection(VertexBSP(p1,Vector3(0,0,0)),VertexBSP(p2,Vector3(0,0,0)));
    ptsPoint.push_back(v.position());
  }
  p3d::shaderVertexAmbient();
  p3d::uniformAmbient(0,0.2,0.8);
  p3d::drawLines(ptsLine);
  p3d::uniformAmbient(0.6,0.7,1);
  p3d::drawPoints(ptsPoint);
}

void GLApplication::display3() {
  p3d::GLText::draw("Face cutting",-1,-1);

  FaceBSP facePlus,faceMinus;
  bsp2_.face(1).separe(bsp2_.face(0),facePlus,faceMinus);

  bsp2_.clearDraw();
  bsp2_.addDraw(bsp2_.face(1));
  bsp2_.addDraw(facePlus);
  bsp2_.addDraw(faceMinus);

  bsp2_.drawDepth();
}


void GLApplication::display4() {
  if (!bsp3_.bsp()) {
    p3d::uniformAmbient(1,0,0);
    p3d::GLText::draw("Press \"Build BSP\" button...",-0.5,0);
  }
  else {
  switch (solution_) {
  case 1:
    p3d::GLText::draw("Render BSP without hidden removal",-1,-1);
    bsp3_.clearDraw();
    bsp3_.addDrawAllTree();
    bsp3_.drawBrut();
    break;
  case 0:
    p3d::GLText::draw("Render BSP with depth buffer",-1,-1);
    bsp3_.clearDraw();
    bsp3_.addDrawAllTree();
    p3d::uniformAmbient(1,0,0);
    if (withBlend_) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }
    bsp3_.drawDepth();
    glDisable(GL_BLEND);
    break;
  case 2:
    p3d::GLText::draw("Render BSP with painter algorithm",-1,-1);
    p3d::uniformAmbient(0,0.8,0.2);
    if (withBlend_) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }
    bsp3_.drawBSP(obs_);
    glDisable(GL_BLEND);
    break;
  }
  }
}



void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera_);

  obs_=camera_.pointTo(Coordinate_World,Vector3(0,0,0));


  //drawGround();

  switch (modeDisplay_) {
  case 1:

    display1();
    break;
  case 2:
    display2();
    break;
  case 3:
    display3();
    break;
  case 4:
    display4();
    break;
  }

  glUseProgram(0);

  snapshot(); // capture opengl window if requested

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
  if (left()) camera_.left(0.1);
  if (right()) camera_.right(0.1);
  if (forward()) camera_.forward(0.1);
  if (backward()) camera_.backward(0.1);
  if (accelerateWheel()) {
    frustum_*=1.05;
    camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  }
  if (decelerateWheel()) {
    frustum_/=1.05;
    camera_.frustum(-0.01,0.01,-0.01,0.01,frustum_,1000);
  }
}



