#include "InteractPosition.h"
#include <algorithm>
#include <iostream>
#include <math.h>


#include "Vector3.h"
#include "Line.h"
#include "Camera.h"

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

InteractPosition::~InteractPosition() {
}

InteractPosition::InteractPosition() : selected_{interactSize()} {
}



void InteractPosition::backup() {
  backup_.clear();
  for (unsigned int i=0;i<interactSize();++i) {
    backup_.push_back(*interactPoint(i));
  }
}


void InteractPosition::restore() {
  for(unsigned int i=0;i<interactSize();++i) {
    *interactPoint(i)=backup_[i];
  }
}


void InteractPosition::add(const Vector2 &mouse) {
  Vector3 addPoint;
  Line ray=cameraInteract_->windowToRayWorld(mouse);
  addPoint=ray.point(1.1);
  size_t i;
  if (interactSize()>1) {
    i=cameraInteract_->selectNearest(mouse,{*interactPoint(0),
                                            *interactPoint(interactSize()-1)},0.1);
    if (i!=0) i++;
  } else {
    i=0;
  }
  interactInsert(i,addPoint);
  interactUpdate(i);
}

void InteractPosition::addEnd(const Vector2 &mouse) {
  Vector3 addPoint;
  Line ray=cameraInteract_->windowToRayWorld(mouse);
  addPoint=ray.point(1.1);
  interactInsert(interactSize(),addPoint);
  interactUpdate(interactSize()-1);
}



size_t InteractPosition::nearestPoint(const Vector2 &m,double radius) {
  vector<Vector3> src;
  src.resize(interactSize());
  for(unsigned int i=0;i<interactSize();++i) {
    src[i]=*interactPoint(i);
  }
  return cameraInteract_->selectNearest(m,src,radius*2);
}

size_t InteractPosition::selectNearest(const Vector2 &m, double seuil) {
    selected_=nearestPoint(m,seuil);
    return selected_;
}


void InteractPosition::movePoint(size_t i,const Vector2 &m) {
  Vector3 *current=interactPoint(i);
  *current=cameraInteract_->windowToWorld(m,*current);
  interactUpdate(i);
}

/*
void InteractPosition::startMouse(const Vector2 &m) {
  Vector3 *nearest;
  nearest=interactPoint(nearestPoint(m,0.1));
  camera_->windowToWorld(m,*nearest,&_startingMouse);
}
*/
void InteractPosition::moveSelected(const Vector2 &m) {
    if(selected_<interactSize()) {
        movePoint(selected_,m);
    }
}

void InteractPosition::forceSelect(size_t i) {
  selected_=i;
}


void InteractPosition::moveValue(const Vector2 &m,double incr) {
  auto nearest=nearestPoint(m,10);
  if (nearest!=interactSize()) {
    double *v=interactValue(nearest);
    if (v) *v+=incr;
  }
}




