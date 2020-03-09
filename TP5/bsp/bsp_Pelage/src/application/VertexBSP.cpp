#include "VertexBSP.h"
#include "ObjectBSP.h"
#include "Vector3.h"

using namespace p3d;


VertexBSP::VertexBSP(const Vector3 &p,const Vector3 &n) : position_{p},normal_{n} {
}



/*
void VertexBSP::interpolateNormal(const VertexBSP &v1,const VertexBSP &v2) {
  double k=Vector3(v1.position(),this->position()).length()/Vector3(v1.position(),v2.position()).length();
  this->normal_=(1.0-k)*v1.normal()+k*v2.normal();
}
*/

