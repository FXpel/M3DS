#include "Curve.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "GLDrawCamera.h"
#include "GLRender.h"
#include "GLDraw.h"
#include "Tools.h"

/**
@file
@author F. Aubert
*/

using namespace p3d;
using namespace std;




/** *********************************************************************************************************************** **/


void Curve::resize(size_t nb) {
  pts_.resize(nb);
}

void Curve::point(size_t i,const Vector3 &p) {
    pts_[i]=p;
}





void Curve::drawControl() {
  vector<Vector3> linePts;
  glPointSize(10);
  p3d::shaderVertexAmbient();
  if (nbPoint()>0) {
    unsigned int i;
    for(i=0;i<nbPoint();++i) {
      linePts.push_back(point(i));
      p3d::draw(int(i),point(i)+Vector3(0.01,0.01,0));
    }
    p3d::drawPoints(linePts);
    p3d::drawLineStrip(linePts);
  }
}



void Curve::interactInsert(size_t i, const Vector3 &insertPoint) {
  pts_.insert(pts_.begin()+int(i),insertPoint);
}












