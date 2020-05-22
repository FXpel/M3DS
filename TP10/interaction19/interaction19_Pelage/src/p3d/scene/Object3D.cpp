#include "Object3D.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Tools.h"
#include "Line.h"


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


std::stack<Object3D> Object3D::stack_;

Object3D::Object3D() {
}


void Object3D::push() {
  stack_.push(*this);
}

void Object3D::pop() {
  if (stack_.empty()) throw ErrorD("Object3D::pop : stack is empty (more pop than push ?)");
  *this=stack_.top();
  stack_.pop();
}


Matrix4 Object3D::worldLocal() const {
  Matrix4 result=Matrix4::identity();
  result.translate(position_);
  result.rotate(orientation_);
  result.scale(scale_);
  return result;
}

Matrix4 Object3D::localWorld() const {
  Matrix4 result;
  result=worldLocal().inverse();
  return result;
}


void Object3D::compose(const Object3D &f) {
  position_=position_+orientation_*f.position();
  orientation_=orientation_*f.orientation();
  scale_=scale_*f.scale(); // !!!! no scale for this ! (difficult to find the orientation after scale)
}



void Object3D::orientation(const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  orientation_.fromFrame(i,j,k);
}


void Object3D::translate(const Vector3 &tr,ECoordinate f){
  if (f==Coordinate_Local) {
    position_=position_+orientation_*tr;
  }
  else {
    position_=position_+tr;
  }
}

void Object3D::rotate(double angle,const Vector3 &axe,ECoordinate f) {
  if (f==Coordinate_Local) {
    orientation_.rotate(angle,axe);
  }
  else {
    auto q=Quaternion::fromAngleAxis(angle,axe);
    orientation_=q*orientation_;
  }
}

void Object3D::rotate(double angle,double x,double y,double z,ECoordinate f) {
  rotate(angle,Vector3(x,y,z),f);
}

void Object3D::rotate(double angle,const Vector3 &axe,const Vector3 &center,ECoordinate f) {
  translate(center,f);
  rotate(angle,axe,f);
  translate(-center,f);
}

void Object3D::rotateXY(double ax,double ay,const Vector3 &center,const Vector3 &verticalAxis,ECoordinate f) {
  Vector3 trans=this->pointTo(f,center);
  translate(trans,Coordinate_Local);
  rotate(ax,Vector3(1,0,0),Coordinate_Local);
  Vector3 vertical=this->directionTo(f,verticalAxis);
  rotate(ay,vertical,Coordinate_Local);
  translate(-trans,Coordinate_Local);
}




void Object3D::rotate(const Quaternion &q,ECoordinate f) {
  if (f==Coordinate_Local) {
    orientation_*=q;
  }
  else {
    orientation_=q*orientation_;
  }
}



void Object3D::lookAt(const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k=normalize(position_-at);
  i=normalize(cross(up,k));

  j=normalize(cross(k,i));

  auto q=Quaternion::fromFrame(i,j,k);
  orientation(q);
}


void Object3D::setIdentity() {
  position_=Vector3(0,0,0);
  orientation_=Quaternion(1,0,0,0);
}


void Object3D::orientation(double a,const Vector3 &u) {
  orientation_=Quaternion::fromAngleAxis(a,u);
}


// linear interpolation this=(1-t)f1+tf2 (i.e. linear interpolation of the translation and the quaternion)
void Object3D::mix(const Object3D &f1,const Object3D &f2,double t) {
  position_=p3d::mix(t,f1.position(),f2.position());
  orientation_=p3d::mix(t,f1.orientation(),f2.orientation());
  scale_=p3d::mix(t,f1.scale(),f2.scale());
}

Vector3 Object3D::pointTo(ECoordinate f,const Vector3 &p) const {
  Vector3 res;
  if (f==Coordinate_Local) {
    res=conjugate(orientation_)*(p-position_);
    res*=Vector3(1.0/scale_.x(),1.0/scale_.y(),1.0/scale_.z());
  }
  else {
    res=orientation_*(p*scale_)+position_;
  }
  return res;
}

Vector3 Object3D::directionTo(ECoordinate f,const Vector3 &p) const {
  Vector3 res(p);
  if (f==Coordinate_Local) {
      res=conjugate(orientation_)*p;
      res*=Vector3(1.0/scale_.x(),1.0/scale_.y(),1.0/scale_.z());
  }
  else {
      res=orientation_*(p*scale_);
  }
  return res;
}



Line Object3D::lineTo(ECoordinate f,const Line &l) {
  Line res=l;
  res.point(pointTo(f,l.point()));
  res.direction(directionTo(f,l.direction()));
  return res;
}


Vector3 Object3D::direction() const {
  return orientation_*Vector3(0,0,1);
}

/// caution : might be what you dont want (use lookAt ?)
void Object3D::direction(const Vector3 &u) {
  orientation_.fromRotation(Vector3(0,0,1),u);
}



