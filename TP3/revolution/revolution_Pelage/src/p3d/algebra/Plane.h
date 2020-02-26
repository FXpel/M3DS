#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include "Line.h"
/*!
*
* @file
*
* @brief Plane Class : a Plane is defined by AM.n=0 (i.e. defined by a point A and a normal n)
* @author F. Aubert
*
*/

namespace p3d {
  class Line;
  enum class PlaneSide {Plus,Minus,None}; // SideNone => not computed

/**
@class Plane
@brief Representation of a plane (based on AM.n=0 : A is a point of the plane, and n its normal)
*/
class Plane {

    public:
    /// default constructor
    Plane(const Vector3 &a=Vector3(0,0,0),const Vector3 &n=Vector3(0,0,1));

    /// get the normal
    const Vector3 &normal() const;
    /// get the origin
    const Vector3 &point() const;
    /// sets the normal to n
    void normal(const Vector3 &n);
    /// sets the point to p
    void point(const Vector3 &p);

    /// orthogonal projection of the point P
    Vector3 project(const Vector3 &p) const;
    /// projection of p along the direction u
    Vector3 project(const Vector3 &p,const Vector3 &u) const;
    /// intersection of the plane with the line d : returns k such P=a+ku
    double interK(const Line &d) const;

    /// returns the intersection point P between the plane and the line d
    Vector3 intersect(const Line &d) const;

    /// returns the signed distance of the point p from this plane (>0 if p is at the normal side).
    double distance(const Vector3 &p) const;
    /// returns the sign of the point p (normal side or the opposite side)
    PlaneSide side(const Vector3 &p) const;
    /// output stream
    friend std::ostream& operator<<(std::ostream &s,const Plane &p);
    /// returns the transformation (a Matrix4) that corresponds to the projection on the plane about the direction u
    Matrix4 projectionDirection(const Vector3 &u) const;
    /// returns the transformation (a Matrix4) that corresponds to the projection on the plane from the point p
    Matrix4 projectionPoint(const Vector3 &p) const;

private:
    Vector3 a_;  ///< point of the plane (named origin).
    Vector3 n_; ///< normal
};


Plane fromTriangle(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3);


std::ostream& operator<<(std::ostream &s,const Plane &p);

}


