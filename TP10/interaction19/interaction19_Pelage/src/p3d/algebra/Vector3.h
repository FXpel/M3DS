#pragma once

/**
@file
@author F. Aubert
@brief Vector3 operations (x,y,z)

*/

#include <iostream>

#include <cmath>
#include <array>

namespace p3d {
  class Vector2;

/**
@class Vector3
@brief Vector3 operations (x,y,z)
*/

class Vector3 final
{
public:
  /// @brief constructs the vector (x,y,z)
  explicit Vector3(double x=0.0,double y=0.0,double z=0.0);
  explicit Vector3(const p3d::Vector2 &p,double z);

  /// returns the i-th element lvalue
  double &operator()(size_t i) {return this->c_[i];}
  /// returns the i-th element rvalue
  const double &operator()(size_t i) const {return c_[i];}
  /// @brief get X
  double x() const {return this->c_[0];}
  float xf() const {return float(this->c_[0]);}
  double r() const {return this->c_[0];}
  /// @brief get Y
  double y() const {return this->c_[1];}
  float yf() const {return float(this->c_[1]);}
  double g() const {return this->c_[1];}
  /// @brief get Z
  double z() const {return this->c_[2];}
  float zf() const {return float(this->c_[2]);}
  double b() const {return this->c_[2];}

  /// @brief returns Vector2
  Vector2 xy() const;

  /// @brief if this is (close to) 0
  bool isZero(double epsilon=0.00001) {return length2()<epsilon;}

  /// @brief set this to b-a (a and b interpreted as points).
  void fromPosition(const Vector3 &a,const Vector3 &b);

  /// @brief set X
  inline void x(double k) {this->c_[0]=k;}
  inline void r(double k) {x(k);}
  /// @brief set Y
  inline void y(double k) {this->c_[1]=k;}
  inline void g(double k) {y(k);}
  /// @brief set Z
  inline void z(double k) {this->c_[2]=k;}
  inline void b(double k) {z(k);}

  /// @brief get a double * pointing to (x,y,z) (no copy).
  const double *v() const {return c_.data();}

  /// @brief normalize this
  void normalize();

  /// @brief Ex : p1+=p2
  Vector3 &operator+=(const Vector3 &a);
  /// @brief Ex : p1-=p2
  Vector3 &operator-=(const Vector3 &a);
  /// @brief Ex : p1*=3.0;
  Vector3 &operator*=(double k);
  /// @brief Ex : p1/=3.0;
  Vector3 &operator/=(double k);
  /// @brief Ex : a*=b (coordinate wise mul : ax*=bx, ay*=by, ...)
  Vector3 &operator*=(const Vector3 &a);


  /// @brief get the square length
  double length2() const;
  /// @brief get the length
  double length() const;


  /// @brief multiply then add : this=k*this+t
  Vector3 &mad(double k,const Vector3 &t);



  /// @brief set this to the mid-point \f$ \frac{n1+n2}{2}\f$
  void mid(const Vector3& n1,const Vector3 &n2);
  /// @brief set this to (this+n1)/2
  void mid(const Vector3& n1);

  /// @return this turned by angle (radian) around Y
  Vector3 rotationY(double angle) const;

  double angle(const Vector3 &u, const Vector3 &vertical=Vector3(0,0,1)) const;

  /// @return *one* (infinite choices) normal vector (cross product with x, y or z)
  Vector3 anyNormal();

  /// @brief rotate this about axe by angle (radian)
  void rotate(double angle,const Vector3 &axe);

  /// @brief return true if u is (almost) parallel to u
  bool isParallel(const Vector3 &u);

  /// @brief set x to the min of this.x and a.x (same with y and z).
  void setMinCoordinate(const Vector3 &a);

  /// @brief set x to the max of this.x and a.x (same with y and z).
  void setMaxCoordinate(const Vector3 &a);

  /// @brief returns the min of the coordinates
  double min(unsigned int *which=nullptr);

  /// @brief returns the max of the coordinates
  double max(unsigned int *which=nullptr) const;



  void interpolateDirection(const Vector3 &u, const Vector3 &v, double t);
  void clamp(double min,double max);


private:
  std::array<double,3> c_ {0.0,0.0,0.0}; // x=c_[0], y=c_[1], z=c_[2]

};




/// @brief cout the vector
std::ostream& operator <<(std::ostream &s,const Vector3 &t);

/// @brief return the vector t which is normalized (function)
Vector3 normalize(const Vector3 &t);
/// @brief Ex : a=b*c (=> ax = bx*cx, ay=by*cy, ...)
Vector3 operator*(Vector3 a,const Vector3 &b);
/// @brief Ex : p=p1+p2
Vector3 operator+(Vector3 a,const Vector3 &b);
/// @brief Ex : p=p1-p2
Vector3 operator-(Vector3 a,const Vector3 &b);
/// @brief Ex : p1=3.0*p2
Vector3 operator*(double k,Vector3 b);
/// @brief Ex : p1=p2*3.0
Vector3 operator*(Vector3 b,double k);
/// @brief Ex : p1=p2/3.0
Vector3 operator/(Vector3 b,double k);
/// @brief Ex : p1=-p2
Vector3 operator-(const Vector3 &a);
/// @brief Ex : p1==p2 !caution : dont use it ! (precision problem => may not do what you expect).
bool operator==(const Vector3 &a,const Vector3 &b);
/// @brief linear interpolation : (1-k)*t1+k*t2
Vector3 mix(double k,const Vector3 &t1,const Vector3 &t2);



/// @return the cross product v1 x v2
Vector3 cross(const Vector3 &v1,const Vector3 &v2);
/// @return the dot product a.b
double dot(const Vector3 &a,const Vector3 &b);
/// @return the mid of a,b
Vector3 mid(const Vector3 &a,const Vector3 &b);

double distance(const Vector3 &a,const Vector3 &b);
double distance2(const Vector3 &a,const Vector3 &b);
double length(const Vector3 &a);
double length2(const Vector3 &a);

/// @return the angle between u and v (along vertical; returns between -PI and PI)
double angle(const Vector3 &u,const Vector3 &v,const Vector3 &vertical);




class Vec3f {
  std::array<float,3> m_;
public:
  explicit Vec3f(const Vector3 &v3) {for(size_t i=0;i<3;++i) m_[i]=static_cast<float>(v3(i));}
  float *v() {return m_.data();}
};


}






