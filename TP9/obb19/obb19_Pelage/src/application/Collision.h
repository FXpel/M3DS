#pragma once


/**
@file
@author F. Aubert
@brief informations concernants la détection/réponse à la collision

*/

#include "Vector3.h"

class Box;
class Interval;
namespace p3d {
class Plane;
}



class DistanceInfo {
public:
  double distance_;
  double direction_;
};

class Collision {
public:
  Collision();


  void collision(Box &b1,Box &b2);
  void collision(Box &b1,const p3d::Plane &p);

  DistanceInfo distance(const Box &b1, const Box &b2, const p3d::Vector3 &axe);
  void detectCollision(const Box &b1, const Box &b2);
  void detectCollision(const Box &b, const p3d::Plane &plane);

  static void drawDebugProjection(const Box &b1,const Box &b2,const p3d::Vector3 &axe,Interval i1,Interval i2);
  static void visualDebug(bool b) {visualDebug_=b;}

private:
  double impulseCoefficient(const Box &b1,const Box &b2);
  double impulseCoefficient(const Box &b,const p3d::Plane &plane);

  bool isCollide_;
  p3d::Vector3 applicationPoint_;
  p3d::Vector3 axis_;
  double mtd_;

  static bool visualDebug_;

};


