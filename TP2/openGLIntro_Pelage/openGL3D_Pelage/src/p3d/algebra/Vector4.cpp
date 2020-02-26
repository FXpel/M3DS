#include "Vector4.h"
#include "Vector3.h"
#include <iostream>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace std;
using namespace p3d;


Vector4::Vector4(const Vector3 &p,double w) : c_{p.x(),p.y(),p.z(),w} {
}


Vector4::Vector4(double x,double y,double z,double w) : c_{x,y,z,w} {
}



const double *Vector4::dv() const {
    return c_.data();
}

Vector3 Vector4::xyz() const  {return Vector3(x(),y(),z());}

ostream& p3d::operator<<(ostream &s,const Vector4 &p) {
    s << "(" << p.x() << "," << p.y() << "," << p.z() << "," << p.w() << ")";
    return s;
}

Vector4 &Vector4::operator *=(double k) {
  c_[0]*=k;
  c_[1]*=k;
  c_[2]*=k;
  c_[3]*=k;
  return *this;
}

Vector4 &Vector4::operator +=(const Vector4 &a) {
  c_[0]+=a.x();
  c_[1]+=a.y();
  c_[2]+=a.z();
  c_[3]+=a.w();
  return *this;
}

Vector4 &Vector4::operator -=(const Vector4 &a) {
  c_[0]-=a.x();
  c_[1]-=a.y();
  c_[2]-=a.z();
  c_[3]-=a.w();
  return *this;
}

void Vector4::oppose() {
  c_[0]=-c_[0];
  c_[1]=-c_[1];
  c_[2]=-c_[2];
  c_[3]=-c_[3];
}


Vector4 Vector4::operator -() {
  Vector4 res(*this);
  res.oppose();
  return res;
}

Vector4 p3d::operator*(double k,const Vector4 &u) {
  Vector4 res=u;
  res*=k;
  return res;
}

Vector4 p3d::operator*(const Vector4 &u,double k) {
  return k*u;
}

Vector4 p3d::operator+(const Vector4 &a,const Vector4 &b) {
  Vector4 res=a;
  res+=b;
  return res;
}

Vector4 p3d::operator-(const Vector4 &a,const Vector4 &b) {
  Vector4 res=a;
  res-=b;
  return res;
}

double p3d::dot(const Vector4 &a,const Vector4 &b) {return a.x()*b.x()+a.y()*b.y()+a.z()*b.z()+a.w()*b.w();}

Vector3 Vector4::toPosition() {
  Vector3 res(c_[0],c_[1],c_[2]);
  return res/=c_[3];
}

Vector3 Vector4::toDirection() {
  return Vector3(c_[0],c_[1],c_[2]);
}


