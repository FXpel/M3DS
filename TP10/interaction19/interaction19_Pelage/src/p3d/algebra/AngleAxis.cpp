#include "AngleAxis.h"


#include "Quaternion.h"
#include "Matrix4.h"

using namespace p3d;


Quaternion AngleAxis::toQuaternion() const {
  double a_radian=(angle_-360.0*int(angle_/360.0))*M_PI/180.0;
  Vector3 v_unit=normalize(axis_);
  double w=cos(a_radian/2);
  double ss=sin(a_radian/2);
  double x=v_unit.x()*ss;
  double y=v_unit.y()*ss;
  double z=v_unit.z()*ss;
  return normalize(Quaternion(w,x,y,z));
}

Vector3 AngleAxis::rotate(const Vector3 &u) {
  return toQuaternion()*u;
}

double p3d::wrap360(double a) {
  return a-floor(a/360.0)*360.0;
}

double p3d::interpolateAngle(double angle0, double angle1,double lambda) {
  double a0=wrap360(angle0);
  double a1=wrap360(angle1);
  double diff=a1-a0;
  if (diff<-180.0) diff+=360.0;
  if (diff>180.0) diff-=360.0;

  return angle0*(1.0-lambda)+angle1*lambda;

//  return angle0+lambda*diff;
}


