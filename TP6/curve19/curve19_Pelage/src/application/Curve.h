#pragma once

#include "InteractPosition.h"

/*!
*
* @file
*
* @brief Curve operations
* @author F. Aubert
*
*/


#include <vector>

/**
@class Curve
@brief Curve operations (control points are in an array of points)
*/

namespace p3d {
class Matrix4;
class Vector3;
}

class Curve : public p3d::InteractPosition {



public:
    void resize(size_t nb);
    void point(size_t i,const p3d::Vector3 &p);

    const p3d::Vector3 &point(size_t i) {return pts_[i];}
    size_t nbPoint() {return pts_.size();}

    const std::vector<p3d::Vector3> &controlPts() {return pts_;}

    void drawControl();
    void drawPoint();

    void insert(size_t where,const p3d::Vector3 &p);

    /////
    p3d::Vector3 *interactPoint(size_t i) override {return &pts_[i];}
    size_t interactSize() override {return pts_.size();}
    void interactInsert(size_t i, const p3d::Vector3 &insertPoint) override;
    /////


    p3d::Vector3 evalCubicVelocity(double t);
    p3d::Vector3 evalCubicAcceleration(double t);
    p3d::Matrix4 tbn(double t);
    p3d::Vector3 evalCubicTierce(double t);
    p3d::Vector3 rN(double t);

private:
    std::vector<p3d::Vector3> pts_{};
};


