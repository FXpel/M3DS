#include "CameraInteract.h"
#include "Object3D.h"

#include "Matrix4.h"
#include <iostream>
#include <QDebug>

#include "GLTool.h"

using namespace p3d;
using namespace std;


/**
  Compléter : coordonnées souris (x,y) => NDC (le viewport est donné par viewX_, viewY_, viewWidth_ et viewHeight_)
 */
p3d::Vector3 CameraInteract::windowToNDC(int x,int y) {
  Vector3 res(0,0,0);
  cout << "tu passes là?" << endl;

  res.x( 2. * (double) ( x - viewX_) / (double)viewWidth_ - 1.);
  res.y( 2. * (double) ( y - viewY_) / (double)viewHeight_ - 1.);
  res.z(-1.);
  cout << "window to NDC : " << res << endl;

  return res;
}

/**
  Compléter : coordonnées souris (x,y) => repère de la caméra (appeler windowToNDC, puis convertir NDC dans le repère de la caméra; la matrice de projection est donnée par projection_)
 */
p3d::Vector3 CameraInteract::windowToCamera(int x,int y) {
  Vector3 res(0,0,0);
  Vector3 temp = windowToNDC(x,y);
  res = projection_.inverse().transformPoint(temp);
  return res;
}

/**
  Compléter : coordonnées souris (x,y) => repère global world (appeler windowToCamera puis convertir dans le repère world; le changement world->camera est donné par la matrice worldCamera_)
 */

p3d::Vector3 CameraInteract::windowToWorld(int x,int y) {
  Vector3 res(0,0,0);
  Vector3 temp = windowToCamera(x,y);

  res = worldCamera().transformPoint(temp);
  return res;
}

/**
  Compléter : coordonnées souris (x,y) => rayon (i.e. une droite) dans le repère global; l'origine du rayon est (0,0,0) dans le repère de la caméra et passe par le point windowToWorld(x,y) (dans le repère global; appel méthode précédente)
  Il faut affecter res.point(a) (l'origine du rayon) et res.direction(u) (la direction du rayon)
 */


p3d::Line CameraInteract::pickingRay(int x, int y) {
  Line res;
  // res.point(a), res.direction(u) to set the line (a and u are Vector3)
  Vector3 A = worldCamera().transformPoint(Vector3(0.,0.,0.));

  Vector3 P = windowToWorld(x,y);
  res.point(A);
  res.direction((P-A));
  return res;
}



/** ***************************************************************************************************************************************************************** */
/** ***************************************************************************************************************************************************************** */
/** ***************************************************************************************************************************************************************** */

CameraInteract::CameraInteract() {
  worldCamera_=Matrix4::identity();
  projection_=Matrix4::identity();
  viewX_=viewY_=viewWidth_=viewHeight_=0;
}

void CameraInteract::frustum(double left, double right, double bottom, double top, double znear, double zfar) {
  projection_=Matrix4::fromFrustum(left,right,bottom,top,znear,zfar);
}

void CameraInteract::viewport(int x, int y, int width, int height) {
  viewX_=x;viewY_=y;viewWidth_=width;viewHeight_=height;
}


const p3d::Matrix4 &CameraInteract::worldCamera() const {
  return worldCamera_;
}

p3d::Matrix4 CameraInteract::cameraWorld() const {
  return worldCamera_.inverse();
}

void CameraInteract::translate(double x,double y, double z,ECoordinate frame) {
  if (frame==Coordinate_Local) {
    worldCamera_*=Matrix4::fromTranslation(x,y,z);
  }
  else {
    worldCamera_=Matrix4::fromTranslation(x,y,z)*worldCamera_;
  }
}

void CameraInteract::translate(const Vector3 &t, ECoordinate frame) {
  translate(t.x(),t.y(),t.z(),frame);
}

void CameraInteract::rotate(double angle, double xAxis,double yAxis, double zAxis, ECoordinate frame) {
  if (frame==Coordinate_Local) {
    worldCamera_*=Matrix4::fromAngleAxis(angle,xAxis,yAxis,zAxis);
  }
  else {
    worldCamera_=Matrix4::fromAngleAxis(angle,xAxis,yAxis,zAxis)*worldCamera_;
  }
}

void CameraInteract::rotate(double angle, const Vector3 &axis, ECoordinate frame) {
  rotate(angle,axis.x(),axis.y(),axis.z(),frame);
}

p3d::Vector3 CameraInteract::pointTo(ECoordinate frame,const p3d::Vector3 &p) {
  if (frame==Coordinate_World) {
    return worldCamera_.transformPoint(p);
  }
  else {
    return worldCamera_.inverse().transformPoint(p);
  }
}

p3d::Vector3 CameraInteract::directionTo(ECoordinate frame,const p3d::Vector3 &u) {
  if (frame==Coordinate_World) {
    return worldCamera_.transformDirection(u);
  }
  else {
    return worldCamera_.inverse().transformDirection(u);
  }
}

void CameraInteract::applyGL() {
  p3d::projectionMatrix=projection_;
  p3d::modelviewMatrix=cameraWorld();
  glViewport(viewX_,viewY_,viewWidth_,viewHeight_);
}



