#include <iostream>

#include "Line.h"
#include "Plane.h"

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


Line::Line(const Vector3 &a,const Vector3 &u) : a_(a),u_(u) {
}


Vector3 Line::point(double k) const {
  return a()+k*u();
}

Line p3d::fromSegment(const Vector3 &a,const Vector3 &b) {
  return Line(a,b-a);
}

double Line::distanceSegment2(const Vector3 &p) {
        Vector3 ab=u_;
        ab-=a_;
        Vector3 pab(p);
        pab-=a_;
        if (dot(pab,ab)<0) {
            return p3d::distance2(p,a_);
        }
        pab=p-u_-a_;
        if (dot(pab,ab)>0) {
            return p3d::distance2(p,u_+a_);
        }


        return distance2(p);
    }




double Line::distance2(const Vector3 &m) const {
  Vector3 am(m);
  am-=a_;

  double k=dot(am,u_)/u_.length2();

  Vector3 h(u_);
  h.mad(k,a_);

  am=m-h;
  return am.length2();

}


void Line::print(string mesg) const {
  cout << mesg << "A=" << this->a() << ", U=" << this->u() << endl;
}

ostream& p3d::operator<<(ostream &s,const Line &l) {
  s << "[A=" << l.a() << ",U=" << l.u() << "]";
  return s;
}

Line p3d::fromIntersection(const Plane &p1,const Plane &p2) {
  Vector3 direction=normalize(cross(p1.normal(),p2.normal()));
  Line lineP1=Line(p1.point(),cross(direction,p1.normal()));
  return Line(p2.intersect(lineP1),direction);
}



