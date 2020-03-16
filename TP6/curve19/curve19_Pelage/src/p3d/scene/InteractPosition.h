#pragma once

/*!
*
* @file
*
* @brief Control an array of points
* @author F. Aubert
*
*/

#include <vector>
#include "Vector3.h"

namespace p3d {
/**
@class ControlMouse
@brief allows to control points in an array with mouse (select/move/add/remove)
*/

class Camera;

class InteractPosition {
  std::vector<Vector3> backup_={}; // in case we want to backup the points (undo motion)
  Camera *cameraInteract_=nullptr;      // contains the tool to transform 2d screen in 3d world
  size_t selected_;                // the point that can be moved by motion
  Vector3 mouseStart_=Vector3();       // where the mouse is pressed in world coordinates

public:
  InteractPosition();
  virtual ~InteractPosition();
  void cameraInteract(Camera *c) {cameraInteract_=c;}

  virtual p3d::Vector3 *interactPoint(size_t i)=0;
  virtual void interactUpdate(size_t) {}
  virtual size_t interactSize() {return 0;}
  virtual void interactInsert(size_t,const p3d::Vector3 &)=0;
  virtual double *interactValue(size_t) {return nullptr;}

  void backup();
  void restore();


  size_t nearestPoint(const Vector2 &m, double radius);

  void forceSelect(size_t i);

  void movePoint(size_t i, const Vector2 &m);


  void moveSelected(const Vector2 &m);

  void startMouse(const Vector2 &m);

  void add(const Vector2 &mouse);
  void addEnd(const Vector2 &mouse);
  size_t selectNearest(const Vector2 &m, double seuil);
  void moveValue(const Vector2 &m, double incr);
};


}


