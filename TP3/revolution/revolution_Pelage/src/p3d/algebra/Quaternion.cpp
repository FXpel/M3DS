#include <stdlib.h>
#include <cmath>

#include "Quaternion.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Tools.h"
#include "p3helpers.h"
#include "AngleAxis.h"

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



Quaternion::Quaternion(double w,double x, double y, double z) : w_(w),x_(x),y_(y),z_(z){
//  normalize();
}

Quaternion::Quaternion(double ww, const Vector3 &n) : w_(ww), x_(n.x()), y_(n.y()), z_(n.z()) {
//  normalize();
}

void Quaternion::setIdentity() {
    w_=1;x_=y_=z_=0;
}


double Quaternion::length() const {
  return sqrt(w_*w_+x_*x_+y_*y_+z_*z_);
}

double Quaternion::lengthVector() const {
  return sqrt(x_*x_+y_*y_+z_*z_);
}


Quaternion p3d::conjugate(const Quaternion &q) {
  return Quaternion(q.w(),-q.x(),-q.y(),-q.z());
}

Quaternion p3d::invert(const Quaternion &q) {
  return conjugate(q)/q.length();
}

Quaternion p3d::slerp(const Quaternion &q1, const Quaternion &q2,double t) {
  double cos=dot(q1,q2);
  Quaternion qp1;
  if (cos<0) qp1=-1.0*q1;
  else qp1=q1;
  double angle=acos(cos);
  Quaternion res=sin(fabs((1.0-t)*angle))/sin(angle)*qp1+sin(fabs(t*angle))/sin(angle)*q2;
  return normalize(res);
}

Quaternion p3d::pow([[maybe_unused]]const Quaternion &q,[[maybe_unused]]double k) {
  // TODO : pow is incorrect
  throw ErrorD("No pow on quaternion");
  /*
  double w=std::pow(q.w(),k);
  double x=std::pow(q.x(),k);
  double y=std::pow(q.y(),k);
  double z=std::pow(q.z(),k);
  //double l=x*x+y*y+z*z+w*w;
  Quaternion res(w,x,y,z);
  //cout << "POW = " << res << " Length = " << std::pow(-0.1,k) << " op = " << q.x() << endl;
  return res;
  */
}

Quaternion p3d::normalize(const Quaternion &q) {
  Quaternion result;
  double l=q.length();
  result.w(q.w()/l);
  result.x(q.x()/l);
  result.y(q.y()/l);
  result.z(q.z()/l);
  return result;
}


AngleAxis Quaternion::toAngleAxis() const {
  double angle=2.0*acos(w_);
  double s=sqrt(1-w_*w_);
  double ax,ay,az;
  if (s<0.0001) {
    ax=1.0;
    ay=0.0;
    az=0.0;
  } else {
    ax=x_/s;
    ay=y_/s;
    az=z_/s;
  }
  Vector3 u(ax,ay,az);

  angle*=180.0/M_PI;
  return AngleAxis(angle,u);
}


ostream &p3d::operator <<(ostream &s,const Quaternion &q) {
  s<<"(w= " << q.w() << " x= " << q.x() << " y= " << q.y() << " z= " << q.z() << ")";
  s<<"norme=" << q.length() << endl;
  return s;
}


Quaternion Quaternion::fromFrame(const Vector3 &ii,const Vector3 &jj,const Vector3 &kk) {
/*
  Quaternion q;
  q.setRotation(Vector3(1,0,0),ii);

  Quaternion q2;
  Quaternion qConj=q;
  qConj.conjugate();

  Vector3 y0=Vector3(0,1,0);
  Vector3 yy=qConj*jj;

  q2.setRotation(y0,yy);

  *this=q*q2;
*/
  Quaternion res;

   // something s wrong (i.e q*(1,0,0), q*(0,1,0), q*(0,0,1) dont (always) give ii,jj,kk (unit tests))
  Vector3 i=normalize(ii);
  Vector3 j=normalize(jj);
  Vector3 k=normalize(kk);

  // from http://www.euclideanspace.com/
  double t=i.x()+j.y()+k.z();
  double s,x,y,z,w;
  if (t>0) {
    s=0.5/sqrt(t+1);

    x=(j.z()-k.y())*s;
    y=(k.x()-i.z())*s;
    z=(i.y()-j.x())*s;

    w=0.25/s;
  }
  else {
    if ((i.x()>j.y()) && (i.x()>k.z())) {
      s=sqrt(1.0+i.x()-j.y()-k.z())*2.0;
      w=(j.z()-k.y())/s;
      x=0.25*s;
      y=(j.x()+i.y())/s;
      z=(k.x()+i.z())/s;
    } else
    if (j.y()>k.z()) {
      s=sqrt(1.0-i.x()+j.y()-k.z())*2.0;
      w=(k.x()-i.z())/s;
      x=(j.x()+i.y())/s;
      y=0.25*s;
      z=(k.y()+j.z())/s;
    } else {
      s=sqrt(1.0-i.x()-j.y()+k.z())*2.0;
      w=(i.y()-j.x())/s;
      x=(k.x()+i.z())/s;
      y=(k.y()+j.z())/s;
      z=0.25*s;
    }


  }
  return Quaternion(w,x,y,z);

}



Quaternion p3d::operator+(Quaternion q1,const Quaternion &q2) {
  return q1+=q2;
}


Quaternion &Quaternion::operator+=(const Quaternion &q) {
  w_+=q.w();
  x_+=q.x();
  y_+=q.y();
  z_+=q.z();
  return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &q) {
  w_-=q.w();
  x_-=q.x();
  y_-=q.y();
  z_-=q.z();
  return *this;
}

Quaternion p3d::operator*(Quaternion q1,const Quaternion &q2) {
  return q1*=q2;
}

Quaternion &Quaternion::operator*=(const Quaternion &q2) {
//Q1 * Q2 =( w1.w2 - v1.v2, w1*v2 + w2*v1 + v1 x v2)
  Quaternion r(*this); // copy is required for quaternion (mutual dependancies of x,y,z,w) (allow self-asignment to q2 also)
  w_=  r.w()*q2.w() - r.x()*q2.x() - r.y()*q2.y() - r.z()*q2.z();
  x_ = r.w()*q2.x() + r.x()*q2.w() + r.y()*q2.z() - r.z()*q2.y();
  y_ = r.w()*q2.y() + r.y()*q2.w() + r.z()*q2.x() - r.x()*q2.z();
  z_ = r.w()*q2.z() + r.z()*q2.w() + r.x()*q2.y() - r.y()*q2.x();
  return *this;
}

Quaternion &Quaternion::operator*=(double k) {
  x_*=k;
  y_*=k;
  z_*=k;
  w_*=k;
  return *this;
}


Quaternion p3d::operator-(Quaternion q1,const Quaternion &q2) {
  return q1-=q2;
}



// square norme
double Quaternion::length2() const {
  return w_*w_+x_*x_+y_*y_+z_*z_;
}


// linear interpolation of unit quaternions in [0,1]
Quaternion p3d::mix(double lambda,const Quaternion &q1, const Quaternion &q2) {
  return normalize((1-lambda)*q1+lambda*q2);
}


Quaternion p3d::operator*(double k,Quaternion q) {
    return q*=k;
}

Quaternion p3d::operator*(Quaternion q,double k) {
  return q*=k;
}

Quaternion p3d::operator/(Quaternion q,double k) {
  return q*=(1.0/k);
}


Vector3 p3d::operator*(const Quaternion &q,const Vector3 &u) {
  Quaternion uq(0,u);
  Quaternion resq=q*uq*conjugate(q);
  return Vector3(resq.x(),resq.y(),resq.z());
}


Quaternion Quaternion::fromRotation(const Vector3 &v1,const Vector3 &v2) {
  Quaternion q;
  Vector3 a = cross(v1,v2);

  if (a.length2()<0.000001 && dot(v1,v2)<0) { // 180 degrees rotation
    return Quaternion(0,Vector3(0,1,0)); // TODO : ok if v1, v2 in plane (x,0,z) (! anyNormal(v1) should be done)
  }
  else {
    return p3d::normalize(Quaternion(sqrt(v1.length2() * v2.length2()) + dot(v1,v2),a));
  }
}

Quaternion Quaternion::identity() {
  return Quaternion(1,0,0,0);
}


Quaternion &Quaternion::rotate(const AngleAxis &r) {
  return *this*=r.toQuaternion();
}

Quaternion &Quaternion::rotate(double angle,const Vector3 &u) {
  return rotate(AngleAxis(angle,u));
}



Matrix4 Quaternion::toMatrix() {
  return Matrix4::fromQuaternion(*this);
}


double p3d::dot(const Quaternion &a,const Quaternion &b) {return a.x()*b.x()+a.y()*b.y()+a.z()*b.z()+a.w()*b.w();}

Vector3 Quaternion::interpolateDirection(const Vector3 &u,const Vector3 &v,double t) {
  // TODO : heavy computation
  Vector3 axis=p3d::cross(u,v);
  Quaternion q;
  if (axis.length()<0.00001) q.setIdentity();
  else {
    double angle=p3d::angle(u,v,cross(u,v));
    angle*=t;
    q=Quaternion::fromAngleAxis(toDegree(angle),axis);
  }
  double ilength=(1.0-t)*u.length()+t*v.length();
  return (q*normalize(u))*ilength;
}

Quaternion Quaternion::fromAngleAxis(double angle,const Vector3 &axis) {
  return AngleAxis(angle,axis).toQuaternion();
}

