#pragma once

#include <string>
#include "Vector3.h"

/**
@file
@author F. Aubert
@brief Operations on homegeneous co-ordinates (x,y,z,w)
*/

namespace p3d {

/**
@class Vector4
@brief Operations on homegeneous co-ordinates (x,y,z,w)
*/
class Vector4 {
    public:
    /// ctor from Vector3,w
    explicit Vector4(const Vector3 &p,double w=1.0);
    /// ctor from (x,y,z,w) (default)
    explicit Vector4(double x=0.0,double y=0.0,double z=0.0,double w=1.0);

    double &operator()(size_t i) {return c_[i];}
    const double &operator()(size_t i) const {return c_[i];}

    /// @brief get
    inline double x() const {return c_[0];}
    inline double y() const {return c_[1];}
    inline double z() const {return c_[2];}
    inline double w() const {return c_[3];}
    inline float xf() const {return float(x());}
    inline float yf() const {return float(y());}
    inline float zf() const {return float(z());}
    inline float wf() const {return float(w());}
    inline double r() const {return x();}
    inline double g() const {return y();}
    inline double b() const {return z();}
    inline double a() const {return w();}

    /// @brief set
    void x(double x) {c_[0]=x;}
    void y(double y) {c_[1]=y;}
    void z(double z) {c_[2]=z;}
    void w(double w) {c_[3]=w;}
    void r(double r) {x(r);}
    void g(double g) {y(g);}
    void b(double b) {z(b);}
    void a(double a) {w(a);}
    void rgb(double r,double g,double b) {x(r);y(g);z(b);}
    void xyz(double xx,double yy,double zz) {x(xx);y(yy);z(zz);}


    /// @return the first 3 components (*no* w-division)
    Vector3 xyz() const;

    /// @return the pointer to data
    const double *dv() const;


    Vector4 &operator *=(double k);
    Vector4 &operator +=(const Vector4 &u);
    Vector4 &operator -=(const Vector4 &u);
    Vector4 operator -();

    /// sets this to the conversion of p (i.e. this=(p*w,w) ).
    void fromPosition(const Vector3 &p, double w);
    void oppose();

    Vector3 toPosition();
    Vector3 toDirection();
private:
    std::array<double,4> c_ {0.0,0.0,0.0,1.0};
};

/// output stream this
std::ostream& operator<<(std::ostream &s,const Vector4 &p);
/// Ex : p=3.0*a
Vector4 operator *(double k,const Vector4 &u);
/// Ex : p=a*3.0
Vector4 operator *(const Vector4 &u,double k);
/// Ex : p=a+b
Vector4 operator +(const Vector4 &u,const Vector4 &v);
/// Ex : p=a-b
Vector4 operator -(const Vector4 &u,const Vector4 &v);
/// @return 4D dot product
double dot(const Vector4 &a,const Vector4 &b);




class Vec4f {
  std::array<float,4> m_;
public:
  explicit Vec4f(const Vector4 &v4) {for(size_t i=0;i<4;++i) m_[i]=static_cast<float>(v4(i));}
  float *v() {return m_.data();}
};


}

