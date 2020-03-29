/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"
#include "DebugTool.h"
#include <iostream>
#include "GLRender.h"

#include "Matrix3.h"


using namespace std;
using namespace p3d;



/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "PerVertex Lighting" << "PerFragment Lighting" << "Texture Transform" << "Shadow+NoLight" << "Shadow map" << ""
                 << "Animate" << "" << "Display RTT (switch color/depth)";


  cameraFrame_=0.01;
  camera_.frustum(-cameraFrame_,cameraFrame_,-cameraFrame_,cameraFrame_,0.03,1000);
  camera_.position(Vector3(15,9,10));
  camera_.lookAt(Vector3(0,0,0),Vector3(0,1,0));



  worldProjectorMatrix_=Matrix4::identity();
  worldProjectorMatrix_.translate(Vector3(0,5,0));
  worldProjectorMatrix_.rotate(20,Vector3(0,1,0));
  worldProjectorMatrix_.translate(Vector3(0,0,4));
  worldProjectorMatrix_.rotate(-30,Vector3(1,0,0));



  animate_=false;

  overlay_=Overlay_None;

  textureEyeMatrix_=Matrix4::identity();

  orbitalCamera_=true;

}

/// dtor
///

GLApplication::~GLApplication() {

}


void GLApplication::initialize() {

  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glClearDepth(1.0);

  glEnable(GL_DEPTH_TEST); // enables depth operations

  glDepthFunc(GL_LESS); // the depth test will be "depth(src) < depth(dest)"
  glDepthMask(GL_TRUE); // enables for writing


  drawScene_.initialize();


  //colorTexture_.read("Tranquil_Lagoon.jpg");



//  diapoTexture_.read("Tyrol.jpg");
  diapoTexture_.read("lightmap.png");


  diapoTexture_.bind(1); // sur l'unité de texture 1

  diapoTexture_.wrap(GL_CLAMP_TO_BORDER); // pas de répétition


  rtt_.create(256,256);       // création d'un Frame Buffer de 256x256 pixels

  //rtt_.rtt(nullptr,nullptr); // 1er paramètre = Color Buffer des pixels (la valeur nullptr crée un color buffer par défaut)
                             // 2ième paramètre = Depth Buffer des pixels
                             // le Color Buffer et le Depth Buffer ont des dimensions fixées par l'appel rtt_.create(256,256) précédent
//rtt_.rtt(&colorTexture_,nullptr);
 rtt_.rtt(&colorTexture_,&depthTexture_);
  depthTexture_.wrap(GL_CLAMP_TO_BORDER); // gestion de la répétition
  //diapoTexture_.wrap(GL_REPEAT);
  depthTexture_.filterLinear(); // lissage pour atténuer l'effet de "pixelisation" de la texture

  colorTexture_.wrap(GL_CLAMP_TO_BORDER); // gestion de la répétition
  colorTexture_.filterLinear(); // lissage pour atténuer l'effet de "pixelisation" de la texture


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
    Vector3 vertical=worldProjectorMatrix_.inverse().transformDirection(Vector3(0,1,0));
    worldProjectorMatrix_*=Matrix4::fromAngleAxis(deltaMouseX()*2,vertical);
    worldProjectorMatrix_*=Matrix4::fromAngleAxis(deltaMouseY()*2,Vector3(1,0,0));
  }

  textureEyeMatrix_=Matrix4::identity();

//  //textureEyeMatrix_.translate(-4,0,0);
//  textureEyeMatrix_.translate(4,0,0);
//  textureEyeMatrix_.rotate(moveAngle_,0,0,1);
//  textureEyeMatrix_.scale(0.4);
  textureEyeMatrix_ = worldProjectorMatrix_.inverse() * camera_.worldLocal();
  textureEyeMatrix_ = Matrix4::fromFrustum(-0.1,0.1,-0.1,0.1,0.1,100) * textureEyeMatrix_;
  if (keyPressed(Qt::Key_A)) {
    animate_=!animate_;
  }

  if (animate_) {
    drawScene_.animateStep();
    moveAngle_+=1;
  }

}

void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:drawScene_.enablePass(Pass_PerVertexLighting);break;
  case 1:drawScene_.enablePass(Pass_PerFragmentLighting);break;
  case 2:drawScene_.enablePass(Pass_TexProj | Pass_NoLighting);break;
  case 3:drawScene_.enablePass(Pass_Shadow | Pass_NoLighting);break;
  case 4:drawScene_.enablePass(Pass_Shadow | Pass_PerFragmentLighting);break;
  case 6:animate_=!animate_;break;
  case 8:overlay_=EOverlay((overlay_+1)%3);break;
  }

}

void GLApplication::updateCamera() {
  if (mouseLeft()) {
    Vector3 center=camera_.pointTo(Coordinate_Local,Vector3(0,0,0));
    Vector3 vertical=Vector3(0,1,0);
    if (orbitalCamera_) {
      camera_.translate(center,Coordinate_Local);
      camera_.rotate(-deltaMouseX()/2.0,vertical,Coordinate_World);
      camera_.rotate(deltaMouseY()/2.0,Vector3(1,0,0),Coordinate_Local);
      camera_.translate(-center,Coordinate_Local);
    }
    else {
      camera_.rotate(-deltaMouseX()/2.0,vertical,Coordinate_World);
      camera_.rotate(deltaMouseY()/2.0,Vector3(1,0,0),Coordinate_Local);
    }
  }
  if (left()) camera_.left(0.3);
  if (right()) camera_.right(0.3);
  if (forward()) camera_.forward(0.3);
  if (backward()) camera_.backward(0.3);
  if (accelerateWheel()) {
    cameraFrame_*=1.05;
    camera_.frustum(-cameraFrame_,cameraFrame_,-cameraFrame_,cameraFrame_,0.03,1000);
  }
  if (decelerateWheel()) {
    cameraFrame_/=1.05;
    camera_.frustum(-cameraFrame_,cameraFrame_,-cameraFrame_,cameraFrame_,0.03,1000);
  }
  if (keyPressed(Qt::Key_Space)) {
    orbitalCamera_=!orbitalCamera_;
  }
}



void GLApplication::renderToTexture() {
  rtt_.begin(); // activation : tous les tracés qui suivent seront faits dans les color et depth buffers de rtt_
  glViewport(0,0,rtt_.width(),rtt_.height()); // viewport aux dimensions du frame buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear le frame buffer




//  p3d::projectionMatrix=camera_.projectionMatrix();
//  p3d::modelviewMatrix=camera_.localWorld(); // visualisation depuis la caméra
  p3d::projectionMatrix= camera_.projectionMatrix().fromFrustum(-0.5,0.5,-0.5,0.5,0.9,100);
  p3d::modelviewMatrix= worldProjectorMatrix_.inverse();
  drawScene_.useShaderPerVertexLighting(); // shader actif
  drawScene_.drawSimple(); // trace les objets : sol, bonhomme, avion, terre





  rtt_.end(); // retour au frame buffer par défaut pour un tracé "normal"
}


void GLApplication::draw() {


  renderToTexture(); // render to own Frame Buffer


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(camera_);


  //drawScene_.depthMap(&colorTexture_);
  drawScene_.depthMap(&depthTexture_);


  drawScene_.diapoTexture(&diapoTexture_);
  drawScene_.textureEye(textureEyeMatrix_);
  drawScene_.worldProjectorMatrix(worldProjectorMatrix_);


  drawScene_.draw();

  drawOverlay();


  snapshot(); // capture opengl window if requested

}


void GLApplication::drawOverlay() {
  p3d::ambientColor=Vector4(1,1,1,1);
  switch(overlay_) {
  case Overlay_None:break;
  default:
  p3d::drawTexture(colorTexture_,0,0,0.3,0.3,false);
  p3d::drawTexture(depthTexture_,0.7,0,0.3,0.3,true);break;

  }
}



