#include "BoxList.h"


/**
@file
@author F. Aubert
@brief opérations sur OBB pour mini simu physique (collision+impulsion)

*/

using namespace std;
using namespace p3d;

BoxList::BoxList() {
}


void BoxList::select(const Vector3 &p) {
  isSelected_=false;
  size_t i=0;
  for(auto &b:box_) {
    if (b.isInside(p)) {
      isSelected_=true;
      selected_=i;
      break;
    }
    ++i;
  }
}

void BoxList::add(const Box &b) {
  box_.push_back(b);
}

void BoxList::draw() {
  for(const auto &b:box_) {
    b.draw();
  }
}


