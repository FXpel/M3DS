#include "Vector2.h"
#include <math.h>

#include "p3helpers.h"

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




Vector2::Vector2(double x,double y) : c_{x,y} {
}


const double *Vector2::dv() const {return c_.data();}


double Vector2::length2() {
    return c_[0]*c_[0]+c_[1]*c_[1];
}

double Vector2::length() {
    return sqrt(this->length2());
}

void Vector2::clamp(double min, double max) {
  if (x()<min) x(min);
  else if (x()>max) x(max);
  if (y()<min) y(min);
  else if (y()>max) y(max);
}

void Vector2::normalize() {
    double d=this->length();
    if (fabs(d)<epsilon) {
        throw "normalize with nul length vector";
    }
    c_[0]/=d;
    c_[1]/=d;

}


Vector2 &Vector2::operator+=(const Vector2 &u) {
  c_[0]+=u(0);
  c_[1]+=u(1);
  return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &u) {
  c_[0]-=u(0);
  c_[1]-=u(1);
  return *this;
}

Vector2 &Vector2::operator*=(double k) {
  c_[0]*=k;
  c_[1]*=k;
  return *this;
}

Vector2 &Vector2::operator*=(const Vector2 &b) {
  c_[0]*=b.x();
  c_[1]*=b.y();
  return *this;
}

double Vector2::dot(const Vector2 &a) {
    return x()*a.x()+y()*a.y();
}


void Vector2::mid(const Vector2& n1,const Vector2 &n2) {
    *this=(n1+n2)/2.0;
}

float Vector2::xf() const {
  return float(c_[0]);
}

float Vector2::yf() const {
  return float(c_[1]);
}

double Vector2::x() const {
  return c_[0];
}

double Vector2::y() const {
  return c_[1];
}

void Vector2::x(double x) {
  c_[0]=x;
}

void Vector2::y(double y) {
  c_[1]=y;
}


Vector2 p3d::operator+(Vector2 a,const Vector2 &b) {
  return a+=b;
}

Vector2 p3d::operator-(Vector2 a,const Vector2 &b) {
  return a-=b;
}

Vector2 p3d::operator*(double k,Vector2 b) {
  return b*=k;
}

Vector2 p3d::operator*(const Vector2 &b,double k) {
  return k*b;
}


Vector2 p3d::operator/(Vector2 b,double k) {
  return b*(1.0/k);
}

Vector2 p3d::operator*(Vector2 a,const Vector2 &b) {
  return a*=b;
}

ostream& p3d::operator <<(std::ostream &s,const Vector2 &q) {
    s << "(" << q.x() << "," << q.y() << ")";
    return s;
}


Vector2 Vector2::normalSegment(const Vector2 &p2) {
    Vector2 res(this->y()-p2.y(),p2.x()-this->x());
    res.normalize();
    return res;
}

double Vector2::distance2(const Vector2 &a) const {
  Vector2 l=*this-a;
  return l.length2();
}

double Vector2::distance(const Vector2 &a) const {
  Vector2 l=*this-a;
  return l.length();
}

double p3d::distance(const Vector2 &a,const Vector2 &b) {
  return a.distance(b);
}

double p3d::distance2(const Vector2 &a,const Vector2 &b) {
  return a.distance2(b);
}




