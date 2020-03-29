#pragma once

#include <string>
#include <ostream>

#include "Vector3.h"
/*!
*
* @file
*
* @brief Line Class. A line is defined by \f$P=A+\lambda u\f$
* @author F. Aubert
*
*/

namespace p3d {
  class Plane;
/**
@class Line
@brief Line representation \f$P=A+\lambda u\f$
*/
class Line {

	public:
  /// constructor from (origin,direction)
  Line(const Vector3 &aa=Vector3(),const Vector3 &uu=Vector3(1,0,0));

  /// returns the origin
  const Vector3 &point() const {return a_;}
  /// returns the origin
  const Vector3 &origin() const {return a_;}
  /// returns the origin
  const Vector3 &a() const {return a_;}
  /// returns the direction
  const Vector3 &direction() const {return u_;}
  /// returns the direction
  const Vector3 &u() const {return u_;}

  /// returns the point on the line : a+ku
  Vector3 point(double k) const;


  /// sets the origin
  void a(const Vector3 &aa) {a_=aa;}
  /// sets the origin
  void point(const Vector3 &aa) {a_=aa;}
  /// sets the origin
  void origin(const Vector3 &aa) {a_=aa;}
  /// sets the direction
  void u(const Vector3 &uu) {u_=uu;}
  /// sets the direction
  void direction(const Vector3 &uu)  {u_=uu;}


  /// returns the squared distance from the point m to the line
  double distance2(const Vector3 &m) const;
  /// print the line
	void print(std::string mesg) const;
  /// output stream. Ex : cout << line << endl; (that prints (A=...,U=...))
	friend std::ostream& operator<<(std::ostream &s,const Line &l);

  double distanceSegment2(const Vector3 &p);

private:
    Vector3 a_; ///< origin of the line
    Vector3 u_; ///< direction of the line


};


/// sets according to the intersection of the 2 planes p1 and p2
Line fromIntersection(const Plane &p1,const Plane &p2);
Line fromSegment(const Vector3 &a,const Vector3 &b);


std::ostream& operator<<(std::ostream &s,const Line &l);

}


