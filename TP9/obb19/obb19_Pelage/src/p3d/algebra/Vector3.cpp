#include "Vector3.h"


#include <cmath>
#include <iostream>

#include "Vector2.h"

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




//float Vector3::fc[3];


Vector3::Vector3(double x,double y,double z) : c_{x,y,z} {
}

Vector3::Vector3(const Vector2 &p,double z) :Vector3(p.x(),p.y(),z) {
}


void Vector3::normalize() {
  (*this)*=1.0/length();
}


Vector2 Vector3::xy() const {
  return Vector2(c_[0],c_[1]);
}


void Vector3::clamp(double min, double max) {
  for(unsigned int i=0;i<3;++i) {
    if (c_[i]<min) c_[i]=min; else if (c_[i]>max) c_[i]=max;
  }
}

/*
float *Vector3::fv() const {
    Vector3::fc[0]=static_cast<float>(c_[0]);
    Vector3::fc[1]=static_cast<float>(c_[1]);
    Vector3::fc[2]=static_cast<float>(c_[2]);
    return fc;
}
*/

double Vector3::length2() const {
    return c_[0]*c_[0]+c_[1]*c_[1]+c_[2]*c_[2];
}

double Vector3::length() const {
    return sqrt(this->length2());
}

double p3d::distance2(const Vector3 &a,const Vector3 &b) {
  return (b-a).length2();
}

double p3d::distance(const Vector3 &a,const Vector3 &b) {
  return (b-a).length2();
}

double p3d::length(const Vector3 &a) {
  return a.length();
}

double p3d::length2(const Vector3 &a) {
  return a.length2();
}


Vector3 p3d::mid(const Vector3& a,const Vector3 &b) {
    return (a+b)/2.0;
}

Vector3 Vector3::rotationY(double angle) const {
  Vector3 res;
  res.x(cos(angle)*x()+sin(angle)*z());
  res.y(y());
  res.z(-sin(angle)*x()+cos(angle)*z());
  return res;
}


double p3d::dot(const Vector3 &a,const Vector3 &b) {
  return a.x()*b.x()+a.y()*b.y()+a.z()*b.z();
}


Vector3 p3d::cross(const Vector3 &a, const Vector3 &b) {
  return Vector3(a.y()*b.z()-a.z()*b.y(),
      a.z()*b.x()-b.z()*a.x(),
      a.x()*b.y()-b.x()*a.y());
}


Vector3 p3d::operator+(Vector3 a,const Vector3 &b) {
    return a+=b;
}

Vector3 p3d::operator-(Vector3 a,const Vector3 &b) {
    return a-=b;
}

Vector3 p3d::operator*(double k,Vector3 b) {
    return b*=k;
}

Vector3 p3d::operator*(Vector3 b,double k) {
    return b*=k;
}

Vector3 p3d::operator/(Vector3 b,double k) {
    return b*=1.0/k;
}



Vector3 p3d::operator*(Vector3 u,const Vector3 &v) {
  return u*=v;
}


Vector3 p3d::operator-(const Vector3 &a) {
    return Vector3(-a.x(),-a.y(),-a.z());
}


ostream& p3d::operator<<(std::ostream &s,const Vector3 &q) {
    s << "(" << q.x() << "," << q.y() << "," << q.z() << ")";
    return s;
}



bool p3d::operator==([[maybe_unused]] const Vector3 &a,[[maybe_unused]] const Vector3 &b) {
  throw std::logic_error("Comparing Vector3 is unsafe (floating point coordinates)");
//  return false;
//  return (a.x()==b.x() && a.y()==b.y() && a.z()==b.z());
}


Vector3 &Vector3::operator +=(const Vector3 &a) {
  c_[0]+=a.x();
  c_[1]+=a.y();
  c_[2]+=a.z();
  return *this;
}

Vector3 &Vector3::operator -=(const Vector3 &a) {
  c_[0]-=a.x();
  c_[1]-=a.y();
  c_[2]-=a.z();
  return *this;
}

Vector3 &Vector3::operator *=(const Vector3 &u) {
  c_[0]*=u.x();
  c_[1]*=u.y();
  c_[2]*=u.z();
  return *this;
}


Vector3 &Vector3::operator *=(double k) {
  c_[0]*=k;
  c_[1]*=k;
  c_[2]*=k;
  return *this;
}

Vector3 &Vector3::operator /=(double k) {
  c_[0]/=k;
  c_[1]/=k;
  c_[2]/=k;
  return *this;
}


Vector3 &Vector3::mad(double k,const Vector3 &t) {
  return ((*this*=k)+=t);
}

Vector3 p3d::mix(double k,const Vector3 &t1,const Vector3 &t2) {
  return (1-k)*t1+k*t2;
}



double p3d::angle(const Vector3 &u,const Vector3 &v,const Vector3 &vertical) {
  Vector3 u1=normalize(u);
  Vector3 v1=normalize(v);

  double angle=acos(dot(u1,v1));

  Vector3 n=cross(u1,v1);
  if (dot(n,vertical)<0) angle=-angle;

  return angle;
}

void Vector3::setMinCoordinate(const Vector3 &a) {
  if (a.x()<this->x()) this->x(a.x());
  if (a.y()<this->y()) this->y(a.y());
  if (a.z()<this->z()) this->z(a.z());
}

void Vector3::setMaxCoordinate(const Vector3 &a) {
  if (a.x()>this->x()) this->x(a.x());
  if (a.y()>this->y()) this->y(a.y());
  if (a.z()>this->z()) this->z(a.z());
}

double Vector3::min(unsigned int *which) {
	unsigned int w;
  if (c_[0]<c_[1]) {
    w=(c_[0]<c_[2])?0:2;
  }
  else {
    w=(c_[1]<c_[2])?1:2;
  }
  if (which!=nullptr) *which=w;
  return c_[w];
}

double Vector3::max(unsigned int *which) const {
	unsigned int w;
  if (c_[0]>c_[1]) {
    w=(c_[0]>c_[2])?0:2;
  }
  else {
    w=(c_[1]>c_[2])?1:2;
  }
  if (which!=nullptr) *which=w;
  return c_[w];
}

Vector3 p3d::normalize(const Vector3 &t) {
  return t/t.length();
}





