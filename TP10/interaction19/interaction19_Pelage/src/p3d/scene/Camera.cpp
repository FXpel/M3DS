#include <cmath>
#include "Camera.h"

#include "glsupport.h"

#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Tools.h"
#include "Line.h"

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

Camera::Camera() {
  frustum(-1,1,-1,1,0.1,1000);
}

Camera::~Camera() {
}

void Camera::volumeView(double left,double right,double down,double top,double pnear,double pfar) {
  volumeView_[0]=left;
  volumeView_[1]=right;
  volumeView_[2]=down;
  volumeView_[3]=top;
  volumeView_[4]=pnear;
  volumeView_[5]=pfar;
}

void Camera::frustum(double left,double right,double down,double top,double pnear,double pfar) {
  volumeView(left,right,down,top,pnear,pfar);
  projection_=Projection_Frustum;
}

void Camera::ortho(double left,double right,double down,double top,double pnear,double pfar) {
  volumeView(left,right,down,top,pnear,pfar);
  projection_=Projection_Ortho;
}

void Camera::perspective(double fov,double znear,double zfar) {
  double fovr=M_PI*fov/180.0;
  double aspect=double(viewport_[2])/viewport_[3];
  volumeView_[2]=-tan(fovr)*znear;
  volumeView_[3]=tan(fovr)*znear;
  volumeView_[0]=volumeView_[2]*aspect;
  volumeView_[1]=volumeView_[3]*aspect;
  volumeView_[4]=znear;
  volumeView_[5]=zfar;
}

void Camera::viewport(int x,int y,int width,int height) {
  viewport_[0]=x;
  viewport_[1]=y;
  viewport_[2]=width;
  viewport_[3]=height;
}

Vector2 Camera::windowToNDC(double x, double y) const {
  double xWindow=(double(x-viewport_[0])/viewport_[2])*2-1;
  double yWindow=(double(y-viewport_[1])/viewport_[3])*2-1;
  return Vector2(xWindow,yWindow);
}

Vector3 Camera::windowToCamera(double x, double y) const {
  Vector3 res;
  Vector2 ndc=windowToNDC(x,y);

  res.x((ndc.x()+1)/2.0*(volumeView_[1]-volumeView_[0])+volumeView_[0]);
  res.y((ndc.y()+1)/2.0*(volumeView_[3]-volumeView_[2])+volumeView_[2]);
  if (projection_==Projection_Frustum) {
    res.z(-volumeView_[4]);
  }
  else {
    res.z(-volumeView_[4]);
  }
  return res;
}

Vector3 Camera::windowToCamera(const Vector2 &m) const {
  return windowToCamera(m.x(),m.y());
}


Vector3 Camera::windowToCamera(double x, double y, const Vector3 &pointPlane) const {
  Vector3 res;
  Vector2 ndc=windowToNDC(x,y);
  Vector3 pCam=pointTo(Coordinate_Local,pointPlane);
  res.x((ndc.x()+1)/2.0*(volumeView_[1]-volumeView_[0])+volumeView_[0]);
  res.y((ndc.y()+1)/2.0*(volumeView_[3]-volumeView_[2])+volumeView_[2]);
  if (projection_==Projection_Frustum) {
    res.z(-volumeView_[4]);
    res*=pCam.z()/res.z();
  }
  else {
    res.z(-volumeView_[4]);
    res.z(pCam.z());
  }
  return res;
}


Vector3 Camera::windowToCamera(const Vector2 &pointWindow, const Vector3 &pointPlane) const {
  return windowToCamera(pointWindow.x(),pointWindow.y(),pointPlane);
}


Vector3 Camera::windowToWorld(const Vector2 &pointWindow,const Vector3 &pointPlane) const {
  return pointTo(Coordinate_World,windowToCamera(pointWindow,pointPlane));
}



Matrix4 Camera::cameraWorld() const {
  Matrix4 result;
  result=worldLocal().inverse();
  return result;
}

Matrix4 Camera::worldCamera() const {
  return worldLocal();
}

Matrix4 Camera::projectionMatrix() const {
  switch(projection_) {
    case Projection_Frustum:return Matrix4::fromFrustum(volumeView_[0],volumeView_[1],volumeView_[2],volumeView_[3],volumeView_[4],volumeView_[5]);
    case Projection_Ortho:return Matrix4::fromOrtho(volumeView_[0],volumeView_[1],volumeView_[2],volumeView_[3],volumeView_[4],volumeView_[5]);
  }
  return Matrix4::identity();
}


void Camera::forward(double k) {
  translate(Vector3(0,0,-k),Coordinate_Local);
}

void Camera::backward(double k) {
  translate(Vector3(0,0,k),Coordinate_Local);
}

void Camera::left(double k) {
  translate(Vector3(-k,0,0),Coordinate_Local);
}

void Camera::right(double k) {
  translate(Vector3(k,0,0),Coordinate_Local);
}



void Camera::setViewport(int x,int y,int w,int h) {
  x_=x;y_=y;w_=w;h_=h;
}


void Camera::lookAt(const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k=normalize(position()-at);
  i=cross(up,k);
  if (i.length()<fabs(1e-4)) i=cross(Vector3(0,0,-1),k); // TODO : hack
  i.normalize();
  j=cross(k,i);

  i.normalize();
  j.normalize();
  k.normalize();

  Quaternion q;
  q=Quaternion::fromFrame(i,j,k);
  orientation(q);
}

Line Camera::windowToRayWorld(const Vector2 &mouse) const {
  Vector3 mouseCam=windowToCamera(mouse);

  Vector3 a;
  Vector3 u=mouseCam;
  if (projection_==Projection_Frustum) {
    a=Vector3(0,0,0);
    u=mouseCam;
  }
  else {
    a=Vector3(mouseCam.x(),mouseCam.y(),0);
    u=Vector3(0,0,mouseCam.z());
  }
  a=pointTo(Coordinate_World,a);
  u=directionTo(Coordinate_World,u);
  return Line(a,u);
}

double Camera::windowToNDC(double r) {
  Vector2 p1=windowToNDC(Vector2(r,r));
  Vector2 p0=windowToNDC(Vector2(0,0));
  return (p1-p0).length2();
}

Vector3 Camera::windowToWorld(const Vector2 &mouse) const {
  return pointTo(Coordinate_World,windowToCamera(mouse));
}


size_t Camera::selectNearest(const Vector2 &mouse,const std::vector<Vector3> &point,double radius) {
  return selectNearest(mouse,point,radius,Matrix4::identity());
}

/*
int Camera::selectNearest(const Vector2 &mouse,const std::vector<Vector4> &point,double radius) {
  vector<Vector3> point3;
  for(auto &p:point) {
    point3.push_back(p.project());
  }
  return selectNearest(mouse,point3,radius,Matrix4::identity());
}
*/

size_t Camera::selectNearest(const Vector2 &mouse,const std::vector<Vector3> &point,double radius,const Matrix4 &transformPoint) {
  size_t imin=point.size();
  if (point.size()==0) return imin;
  Line ray;
  Matrix4 NDCtoLocal=projectionMatrix()*cameraWorld()*transformPoint;
  Vector2 mouseNDC=windowToNDC(mouse);
  double radiusNDC=windowToNDC(radius);
  Vector3 pNDC=NDCtoLocal.transformPoint(point[0]);
  double dmin=Vector2(pNDC.x(),pNDC.y()).distance2(mouseNDC);
  for(size_t i=0;i<point.size();++i) {
    pNDC=NDCtoLocal.transformPoint(point[i]);
    auto d=Vector2(pNDC.x(),pNDC.y()).distance2(mouseNDC);
    if (d<radiusNDC && (d<dmin || imin==point.size())) {
      dmin=d;imin=i;
    }
  }
  return imin;
}


Vector2 Camera::windowToNDC(const Vector2 &pointWindow) const {
  return windowToNDC(pointWindow.x(),pointWindow.y());
}


Vector3 Camera::windowToWorld(double x,double y) const {
  return pointTo(Coordinate_World,windowToCamera(x,y));
}

