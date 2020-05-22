#include "GLApplication.h"

#include <iostream>

#include "GLTool.h"
#include "Plane.h"

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

/**
 * A compléter : mouvement de la caméra (cf sujet)
 */

void GLApplication::updateCamera() {
  if (left()) { // key left pressed
    camera_.translate(-0.5,0,0,Coordinate_Local);
  }
  if (right()) {
    camera_.translate(0.5,0,0,Coordinate_Local);
  }
  if (forward()) {
    camera_.translate(0,0,-0.5,Coordinate_Local);
  }
  if (backward()) {
    camera_.translate(0,0,0.5,Coordinate_Local);
  }
  if (mouseLeft()) { // left click and move
    // rotate camera : deltaMouseX(), deltaMouseY() give the mouse motion
    camera_.rotate(deltaMouseY(),1,0,0,Coordinate_Local);
    camera_.rotate(-deltaMouseX(),(camera_.cameraWorld()*Vector4(0,1,0,0)).xyz(),Coordinate_Local);
  }
}

/**
 * Compléter : manipulation soit en translation soit en rotation de l'objet sélectionné (i.e. variable mesh)
 * Utiliser mesh->translate et mesh->rotate (pointeur) pour modifier le positionnement de l'objet
 */

void GLApplication::moveSelectedObject() {
  if (mouseRight()) { // interact with right mouse button
    if (isSelectedIntersection_) { // an intersection has been found (in GLApplication::selectObject() ).
      MeshObject3D *mesh=selectedIntersection_.mesh(); // selected mesh

      double dx=double(deltaMouseX())/100.0,dy=double(deltaMouseY())/100.0; // /100.0 to attenuate mouse motion

      if (controlMouse_==Manipulation_Translation) {
          Vector3 T = camera_.directionTo(Coordinate_World, Vector3(dx, dy, 0));
          mesh->translate(T ,Coordinate_World);
      }
      else if (controlMouse_==Manipulation_Orientation) {

      }
    }
  }

}

void GLApplication::selectObject() {
  if (mouseRight()) {
    pickingRay_=camera_.pickingRay(mouseX(),mouseY());
  }
  if (mouseRightPressed()) {

    //sceneIntersection_.intersect({&triangle_},pickingRay_); // only triangle for now
    sceneIntersection_.intersect({&triangle_,&triceratops_,&cow_},pickingRay_);
    if (sceneIntersection_.size()>0) {


      testIntersection(); // draws ray and intersections : comment this line once intersection is ok

      isSelectedIntersection_=true;
      selectedIntersection_=sceneIntersection_[0]; // the first intersection is the nearest (from the camera)

      // save the intersection point coordinates (required for interaction) :
      attachPointWorld_=selectedIntersection_.rayWorld().point(selectedIntersection_.lambda());
      sceneIntersection_.intersect({&triangle_,&triceratops_,&cow_},pickingRay_);
    }
    else {
      isSelectedIntersection_=false; // no intersection
    }
  }
}

/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */
/** ************************************************************************************************************************************** */


GLApplication::~GLApplication() {

}

GLApplication::GLApplication() {
  leftPanelMenu_ << "Manipulation Translation" << "Manipulation Rotation";

  cow_.readInit("cow.obj");
  cow_.computeNormal();

  triceratops_.readInit("triceratops.obj");
  triceratops_.computeNormal();

  triangle_.readInit("triangle.obj");
  triangle_.computeNormal();

  cow_.translate(Vector3(-0.7,0.65,-0.3));
  cow_.rotate(-20,Vector3(0,1,0));
  triceratops_.translate(Vector3(0.5,0.5,1.0));
  triangle_.translate(Vector3(0,1,2));
  triangle_.rotate(30,Vector3(1,1,0));



  camera_.translate(Vector3(0,1,10),Coordinate_Local);


  p3d::lightPosition[0]=Vector4(0,0,10,1);

}

void GLApplication::initialize() {
  glClearColor(1,1,1,1);

  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLTool();
  glPointSize(5.0);


  groundTexture_.readMipmap("mosaic_pierre.jpg");
  glEnable(GL_STENCIL_TEST);

  cow_.initVAO();
  triceratops_.initVAO();
  triangle_.initVAO();

}



void GLApplication::drawScene() {

  p3d::modelviewMatrix.push();

  drawGround();
  p3d::ambientColor=Vector4(0,0,0,1);
  drawCow();
  drawTriceratops();
  drawTriangle();

  p3d::modelviewMatrix.pop();

}

void GLApplication::drawCow() {
  // tracé de la vache
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix*=cow_.worldLocal();
    p3d::diffuseColor=Vector3(0.8,0.0,0.0);
    p3d::shaderLightPhong();
    cow_.draw();
    p3d::modelviewMatrix.pop();
    // fin tracé de la vache
}

void GLApplication::drawTriceratops() {
  // tracé du tricératops
  p3d::modelviewMatrix.push();

  p3d::modelviewMatrix*=triceratops_.worldLocal();
  p3d::diffuseColor=Vector3(0.0,0.6,0.6);
  p3d::shaderLightPhong();
  triceratops_.draw();
  p3d::modelviewMatrix.pop();
  // fin tracé tricératops
}

void GLApplication::drawTriangle() {
  // tracé du triangle
  p3d::modelviewMatrix.push();

  p3d::modelviewMatrix*=triangle_.worldLocal();
  p3d::diffuseColor=Vector3(0.4,0.6,0.9);
  p3d::diffuseBackColor=Vector3(0.8,0.2,0.7);
  p3d::shaderLightPhong();
  triangle_.draw();
  p3d::modelviewMatrix.pop();
  // fin tracé triangle
}


void GLApplication::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::textureMatrix.push();
  p3d::textureMatrix.scale(135,135,135);
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.scale(10,10,10);
  p3d::ambientColor=Vector4(0,0,0,1);
  p3d::shaderTextureLight();
  p3d::drawGround(groundTexture_);
  p3d::textureMatrix.pop();
  p3d::modelviewMatrix.pop();
}

void GLApplication::drawCursor() {
  p3d::modelviewMatrix.push();
  Vector3 cursor=pickingRay_.point(100);
  p3d::modelviewMatrix.translate(cursor.x(),cursor.y(),cursor.z());
  p3d::modelviewMatrix.scale(0.05,0.05,0.05);
  p3d::diffuseColor=Vector3(0.7,0.0,0.3);
  p3d::ambientColor=Vector4(0,0,0,0);
  p3d::specularColor=Vector3(0,0,0);
  p3d::shaderLightPhong();
  p3d::drawSphere();
  p3d::modelviewMatrix.pop();
}

void GLApplication::testIntersection() {
  debug_.clear();
  cout << "Nombre d'intersections = " << sceneIntersection_.size() << endl;
  for(unsigned int i=0;i<sceneIntersection_.size();i++) {
    debug_.point(pickingRay_.point(sceneIntersection_[i].lambda()),std::to_string(i),Vector3(1,0,0));
  }
  debug_.segment(Line(pickingRay_.point(0),pickingRay_.direction()*1000),"","",Vector3(0,1,0));
}


/** ***************************************************************** **/
void GLApplication::resize(int width,int height) {
  camera_.viewport(0,0,width,height);
  double ratio=double(width)/height;
  camera_.frustum(-0.01*ratio,0.01*ratio,-0.01,0.01,0.03,100.0);
}


void GLApplication::update() {
  updateCamera();
  selectObject();
  moveSelectedObject();
}




void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  camera_.applyGL();


  drawCursor();
  drawScene();

  p3d::ambientColor=Vector4(0.2,0.9,0.4,1.0);
  glDepthFunc(GL_ALWAYS);
  debug_.drawPoint();
  glDepthFunc(GL_LESS);
  debug_.drawSegment();

}


void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:controlMouse_=Manipulation_Translation;break;
  case 1:controlMouse_=Manipulation_Orientation;break;
  }
}

