#pragma once

/*!
*
* @file
*
* @brief Operations on (x,y)
* @author F. Aubert
*
*/

#include <iostream>
#include <array>


namespace p3d {
/**
@class Vector2
@brief Operations on (x,y)
*/
class Vector2 {
public:
    /// @brief ctor from (x,y) (default)
    Vector2(double x=0.0,double y=0.0);
    /// @getter
    double x() const;
    float xf() const;
    double y() const;
    float yf() const;
    /// @return the pointer to data (direct access to coordinates)
    const double *dv() const;
    /// @return the pointer to data with cast to float * !caution : the pointer is shared between all Vector2. Ex : foo(a.fv(),b.fv()) => same values into the two parameters
    const float *fv() const;

    /// returns the ith coordinate of this
    inline double &operator()(size_t i) {return c_[i];}
    /// returns the ith coordinate of this
    inline const double &operator()(size_t i) const {return c_[i];}

    /// sets x
    void x(double x);
    /// sets y
    void y(double y);
    /// adds a to this. Ex : p1+=p2
    Vector2 &operator+=(const Vector2 &a);
    /// subs a to this. Ex : p1-=p2
    Vector2 &operator-=(const Vector2 &a);
    /// sets this to a+b
    void add(const Vector2 &a,const Vector2 &b);
    /// sets this to a-b
    void sub(const Vector2 &a,const Vector2 &b);
    /// multiplies this by the number k. Ex : p1*=a
    Vector2 &operator*=(double k);
    /// @return this*=b (coordinate wise)
    Vector2 &operator*=(const Vector2 &b);
    /// returns the squared length of this
    double length2();
    /// returns the length of this
    double length();
    /// returns the squared distance from this to a.
    double distance2(const Vector2 &a) const;
    /// returns the distance from this to a
    double distance(const Vector2 &a) const;
    /// normalize this
    void normalize();
    /// sets this to k*this. Ex : p.scale(3.0)
    void scale(double k);
    /// sets this to (this.x*a.x, ...) (multiplies each coordinate of this by the corresponding coordinate of a)
    void scale(const Vector2 &a);
    /// sets this to k/this (i.e. this.x = k/this.x, ... for each coordinate)
    void invScale(double k);
    /// returns dot product between this and a. Ex : p1.dot(p2)
    double dot(const Vector2 &a);
    /// sets this to the mid-point \f$\frac{(n1+n2)}{2}\f$
    void mid(const Vector2& n1,const Vector2 &n2);
    /// sets this to the mid-point this=\f$\frac{this+n1}{2}\f$
    void mid(const Vector2& n1);
    /// print (x,y,z)
    void print();
    /// returns a normal vector to the edge [this,p2]
    Vector2 normalSegment(const Vector2 &p2);

    void clamp(double min,double max);

public:
    std::array<double,2> c_ {0.0,0.0};
};

/// Ex : p=a+b
Vector2 operator+(Vector2 a,const Vector2 &b);
/// Ex : p=a-b
Vector2 operator-(Vector2 a,const Vector2 &b);
/// @brief compatibilité avec cout (affiche les coordonnées).
std::ostream& operator <<(std::ostream &s,const Vector2 &t);
/// Ex : p=3.0*a
Vector2 operator*(double k,Vector2 b);
/// Ex : p=a*3.0
Vector2 operator*(const Vector2 &b,double k);
/// Ex : p=a/3.0
Vector2 operator /(Vector2 b,double k);
/// Ex : p=p1*p2 => p.x()=p1.x()*p2.x(), ... (for each coordinate)
Vector2 operator *(Vector2 p1,const Vector2 &p2);
/// Ex : p=p1/p2 => p.x()=p1.x()/p2.x(), ... (for each coordinate)
Vector2 operator /(Vector2 p1,const Vector2 &p2);

double distance(const Vector2 &a,const Vector2 &b);
double distance2(const Vector2 &a,const Vector2 &b);


class Vec2f {
  std::array<float,2> m_;
public:
  explicit Vec2f(const Vector2 &v2) {for(size_t i=0;i<2;++i) m_[i]=static_cast<float>(v2(i));}
  float *v() {return m_.data();}
};

}





