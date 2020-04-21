#pragma once

#include "Object3D.h"

#include <stack>
#include <vector>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

namespace p3d {
class Vector2;
class Camera : public Object3D {
public:
  enum EProjection {Projection_Ortho,Projection_Frustum};
  /// constructor
  Camera();
  /// destructor
  virtual ~Camera();

  /// returns the matrix camToWorld
  Matrix4 cameraWorld() const;
  /// returns the matrix worldToCam
  Matrix4 worldCamera() const;

  /// sets the camera such it looks the point at and the up direction is given by up
  void lookAt(const Vector3 &at,const Vector3 &up=Vector3(0,1,0));

  /// translates in the $z$ direction of the camera
  void forward(double k);
  void backward(double k);

  /// translates in the $x$ direction
  void right(double k);
  void left(double k);

  /// set the viewport (unused)
  void setViewport(int x,int y,int w,int h);

  /// sets with linear interpolation between c1 and c2
  void mix(const Camera &c1,const Camera &c2,double k);

  void volumeView(double left, double right, double down, double top, double pnear, double pfar);
  void frustum(double left, double right, double down, double top, double pnear, double pfar);
  void ortho(double left, double right, double down, double top, double pnear, double pfar);
  void perspective(double fov, double znear, double zfar);
  p3d::Matrix4 projectionMatrix() const;
  void viewport(int x,int y,int w,int h);

  /// returns the conversion of pointWindow (given in pixels) in NDC coordinates (u undergoes the inverse of viewport)
  Vector2 windowToNDC(const Vector2 &pointWindow) const;
  Vector2 windowToNDC(double x,double y) const;

  Vector3 windowToCamera(double x,double y) const;
  Vector3 windowToCamera(const Vector2 &m) const;
  Vector3 windowToCamera(double x, double y, const Vector3 &pointPlane) const;
  Vector3 windowToCamera(const Vector2 &pointWindow, const Vector3 &pointPlane) const;


  Vector3 windowToWorld(double x,double y) const;
  Vector3 windowToWorld(const Vector2 &m) const;
  Vector3 windowToWorld(const Vector2 &pointWindow,const Vector3 &pointPlane) const;

  double windowToNDC(double r);

  Line windowToRayWorld(const Vector2 &mouse) const;


  double vLeft() const {return volumeView_[0];}
  double vRight() const {return volumeView_[1];}
  double vTop() const {return volumeView_[3];}
  double vBottom() const {return volumeView_[2];}
  double vNear() const {return volumeView_[4];}
  double vFar() const {return volumeView_[5];}

  int viewX() const {return viewport_[0];}
  int viewY() const {return viewport_[1];}
  int viewWidth() const {return viewport_[2];}
  int viewHeight() const {return viewport_[3];}

  size_t selectNearest(const Vector2 &mouse, const std::vector<Vector3> &point, double radius,const Matrix4 &modelview);
  size_t selectNearest(const Vector2 &mouse, const std::vector<Vector3> &point, double radius);
//  int selectNearest(const Vector2 &mouse, const std::vector<Vector4> &point, double radius);
private:
  float x_,y_,w_,h_; ///< stores the viewport
  double volumeView_[6]; ///< left,right,down,top,near,far
  int viewport_[4]; ///< x,y,width,height
  EProjection projection_;

};

}


