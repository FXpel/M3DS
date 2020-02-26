#pragma once

#include <iostream>

/**
@file
@author F. Aubert
@brief Quaternion class for rotation
*/


namespace p3d {
  class Matrix4;
  class Vector3;
  class AngleAxis;

/**
@class Quaternion
@brief Quaternion class for rotation
*/
class Quaternion {
public:
  /// @brief constructor (default = unit quaternion (1,(0,0,0)))
  explicit Quaternion(double w=1.0,double x=0.0,double y=0.0,double z=0.0);
  /// @brief constructor : (w,(x,y,z))=(a,n)
  explicit Quaternion(double ww,const Vector3 &n);

  /// @brief getter/setter (for coherence access)
  double x() const {return x_;}
  double y() const {return y_;}
  double z() const {return z_;}
  double w() const {return w_;}
  void x(double v) {x_=v;}
  void y(double v) {y_=v;}
  void z(double v) {z_=v;}
  void w(double v) {w_=v;}

  /// @brief set this to identity
  void setIdentity();

  /// @return the angle and the rotation axis in *angle and *u (TODO : no AngleAxis class)
  AngleAxis toAngleAxis() const;
  /// @return the length
  double length() const;
  /// @return the square of the length
  double length2() const;
  /// @return the length of the (x,y,z) element (the vector element of this)
  double lengthVector() const;
  /// @brief this is composed with the rotation (angle in degrees)
  Quaternion &rotate(double angle,const Vector3 &u);
  /// @brief this is composed with the rotation (angle in degrees)
  Quaternion &rotate(const AngleAxis &r);
  /// @return the rotation matrix
  Matrix4 toMatrix();
  /// @brief apply the quaternion (i.e. the rotation) to vector u
  void transform(Vector3 &u) const;



  Quaternion &operator*=(const Quaternion &q2);
  Quaternion &operator+=(const Quaternion &q2);
  Quaternion &operator-=(const Quaternion &q2);
  Quaternion &operator*=(double k);

  static Vector3 interpolateDirection(const Vector3 &u, const Vector3 &v, double t);
  /// @return the identity quaternion
  static Quaternion identity();
  /// @return the quaternion corresponding to the rotation through angle (in degrees) about v axis
  static Quaternion fromAngleAxis(const AngleAxis &r);
  static Quaternion fromAngleAxis(double angle,const Vector3 &axis);


  /// @return quaternion according to the frame (i,j,k) (i.e. this represents the frame transformation from the reference frame)
  static Quaternion fromFrame(const Vector3 &i, const Vector3 &j, const Vector3 &);
  /// @return quaternion according to the rotation between v1 and v2
  static Quaternion fromRotation(const Vector3 &v1,const Vector3 &v2);
  /// @return quaternion from linear interpolation this=(1-t)q1+tq2  (then post normalized)
  static Quaternion mix(double t,const Quaternion &q1, const Quaternion &q2);




private:
  double w_,x_,y_,z_;

};
/// returns the conjugate of q
Quaternion conjugate(const Quaternion &q);
/// returns the quaternion that is normalized q
Quaternion normalize(const Quaternion &q);
/// returns the transformation of u by the rotation q (i.e. conjugate(q)*u*q).
Vector3 operator*(const Quaternion &q,const Vector3 &u);
/// returns the quaternion q=q1*q2
Quaternion operator*(Quaternion q1,const Quaternion &q2);
/// returns the quaternion q=q1+q2
Quaternion operator+(Quaternion q1,const Quaternion &q2);
/// returns the quaternion q=q1-q2
Quaternion operator-(Quaternion q1,const Quaternion &q2);
/// returns the quaternion q=k*q
Quaternion operator*(double k,Quaternion q);
/// returns the quaternion q=q*k
Quaternion operator*(Quaternion q,double k);
Quaternion operator/(Quaternion q,double k);
Quaternion pow(const Quaternion &q,double k);
Vector3 operator *(const Quaternion &q,const Vector3 &u);
Quaternion mix(double lambda,const Quaternion &q1, const Quaternion &q2);


/// out stream
std::ostream& operator<<(std::ostream &s,const Quaternion &q);


Quaternion invert(const Quaternion &q);
Quaternion slerp(const Quaternion &q1, const Quaternion &q2, double t);
double dot(const Quaternion &a,const Quaternion &b);




}

