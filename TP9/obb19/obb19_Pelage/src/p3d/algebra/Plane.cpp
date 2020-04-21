#include "Plane.h"
#include "Matrix4.h"
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


Plane::Plane(const Vector3 &a,const Vector3 &n) : a_(a),n_(n) {
}


Plane p3d::fromTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3) {
    return Plane(p1,cross(p2-p1,p3-p2));
}


double Plane::interK(const Line &d) const {
    return (dot(n_,a_-d.a()))/dot(n_,d.u());
}

double Plane::distance(const Vector3 &p) const {
  return dot(p-a_,n_)/n_.length2();
}

PlaneSide Plane::side(const Vector3 &p) const {
  Vector3 ap=p-a_;
  if (dot(ap,n_)>=0) return PlaneSide::Plus;
  else return PlaneSide::Minus;
}

Vector3 Plane::project(const Vector3 &p) const {
    Line d(p,n_);
    double k=this->interK(d);
    return k*n_+p;
}

Vector3 Plane::project(const Vector3 &p,const Vector3 &u) const {
    Line d(p,u);
    double k=this->interK(d);
//    UtilGL::add(k*u+p);
    return k*u+p;
}

Vector3 Plane::intersect(const Line &d) const {
  double k=this->interK(d);
  return k*d.u()+d.a();
}

const Vector3 &Plane::normal() const {
  return n_;
}

const Vector3 &Plane::point() const {
  return a_;
}

void Plane::point(const Vector3 &p) {
  a_=p;
}

void Plane::normal(const Vector3 &norm) {
  n_=norm;
}

ostream& p3d::operator<<(ostream &s,const Plane &p) {
    s << "[POINT=" << p.point() << ",N=" << p.normal() << "]";
    return s;
}

Matrix4 Plane::projectionDirection(const Vector3 &u) const {
  Matrix4 res;
  Vector3 n=normal();
  double un=dot(u,n);
  double an=dot(point(),normal());
  res.row(0,un-n.x()*u.x(),-n.y()*u.x(),-n.z()*u.x(),an*u.x());
  res.row(1,-n.x()*u.y(),un-n.y()*u.y(),-n.z()*u.y(),an*u.y());
  res.row(2,-n.x()*u.z(),-n.y()*u.z(),un-n.z()*u.z(),an*u.z());
  res.row(3,0,0,0,un);
  return res;
}

Matrix4 Plane::projectionPoint(const Vector3 &L) const {
  Matrix4 res;
    Vector3 n=normal();
    Vector3 A=point();
    double LAn=dot(A-L,n);
    double An=dot(A,n);
    double Ln=dot(L,n);
    res.row(0,LAn+n.x()*L.x(),n.y()*L.x(),n.z()*L.x(),-An*L.x());
    res.row(1,n.x()*L.y(),LAn+n.y()*L.y(),n.z()*L.y(),-An*L.y());
    res.row(2,n.x()*L.z(),n.y()*L.z(),LAn+n.z()*L.z(),-An*L.z());
    res.row(3,n.x(),n.y(),n.z(),-Ln);
    return res;
}




