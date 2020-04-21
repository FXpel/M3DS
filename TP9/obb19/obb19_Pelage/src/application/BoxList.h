#pragma once


/**
@file
@author F. Aubert
@brief opérations sur OBB pour mini simu physique (collision+impulsion)

*/
#include "Box.h"
#include <vector>


namespace p3d {
class Vector3;
}

class BoxList {
  public:
    BoxList();

    void add(const Box &b);
    std::vector<Box> &all() {return box_;}

    void draw();


    Box &operator[](size_t i) {return box_[i];}
    size_t size() {return box_.size();}
    Box &selected() {return box_[selected_];}
    bool isSelected() {return isSelected_;}
    void select(const p3d::Vector3 &p);
    void unSelect() {isSelected_=false;}

    void insertForce(const p3d::Vector3 &p,const p3d::Vector3 &f);
    double moment(const p3d::Vector3 &f,const p3d::Vector3 &a);

  protected:
  private:
    bool isSelected_{false};
    size_t selected_{0};
    std::vector<Box> box_{};

};


