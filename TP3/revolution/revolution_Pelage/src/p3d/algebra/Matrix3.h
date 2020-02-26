#pragma once

#include <array>

/*!
*
* @file
*
* @brief 3x3 Matrix Class
* @author F. Aubert
*
*/

namespace p3d {
  class Matrix4;
  class Vector3;


class Matrix3 {
public:
  /// returns the pointer to data (column-major order)
  inline const double *v() const {return c_.data();}
  Matrix3 inverse() const;
  Matrix3 transpose() const;
  void subScaleColumn(size_t i, size_t j, double s);
  void setIdentity();


  /// returns the i-th coefficient of this (Matrix3 is column-major)
  inline double &operator()(size_t i) {return c_[i];}
  /// returns the i-th coefficient of this (Matrix3 is column-major)
  inline double operator()(size_t i) const {return c_[i];}
  /// returns the element at row i and at column j
  inline double &operator()(size_t i,size_t j) {return c_[i+(j*3)];}
  /// returns the element at row i and at column j
  inline double operator()(size_t i,size_t j) const {return c_[i+(j*3)];}

  Vector3 column(size_t c) const;
  Vector3 row(size_t r) const;
  void column(size_t c,double x,double y,double z);
  void column(size_t c,const Vector3 &u);
  void row(size_t r,double x,double y,double z);
  void row(size_t r,const Vector3 &u);


  void swapColumn(size_t i, size_t j);
  void scaleColumn(size_t i, double k);

  static Matrix3 identity();
  static Matrix3 fromColumn(const Vector3 &i,const Vector3 &j,const Vector3 &k);

private:
  std::array<double,9> c_ {1.0,0.0,0.0,
                           0.0,1.0,0.0,
                           0.0,0.0,1.0};


};

/// returns the transformation of p by the matrix m. Ex : p2=m*p1
Vector3 operator*(const Matrix3 &m,const Vector3 &p);



class Mat3f {
  std::array<float,9> m;
public:
  explicit Mat3f(const Matrix3 &m3) {for(size_t i=0;i<9;++i) m[i]=static_cast<float>(m3(i));}
  float *v() {return m.data();}

};

}


