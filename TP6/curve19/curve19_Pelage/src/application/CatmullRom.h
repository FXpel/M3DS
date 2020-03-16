#pragma once

#include "Curve.h"
#include <iostream>

/**
@author F. Aubert
@brief representation/computation of a catmull rom curve (inherits Curve)
*/

class Bezier;

class CatmullRom: public Curve {



    public:
    std::vector<p3d::Vector3> *intermediatev(); // get the reference

    void intermediate(size_t i, size_t j, const p3d::Vector3 &p);
    const p3d::Vector3 &intermediate(size_t i, size_t j) const;

    void setup(bool close);
    void draw();
    void drawControl(int mode=0);
    void drawPoint();

    //////
    p3d::Vector3 *interactPoint(size_t i) override {if (i<nbPoint()) return Curve::interactPoint(i); else return &(_intermediate[i-nbPoint()]);}
    size_t interactSize() override {return nbPoint()+_intermediate.size();}
    void interactInsert(size_t i,const p3d::Vector3 &p) override;


    void resize(size_t nb);
    void drawTBN(double t);

    void drawFrame(double t);

    p3d::Vector3 eval(double t);
    Bezier segment(double t,double *tCubic);




    p3d::Matrix4 tbn(double tValue);
    void whichSegment(double t, size_t *cubicNumber, double *tCubic);
    Bezier segment(size_t i);
private:
    std::vector<p3d::Vector3> _intermediate; // list of intermediate Bezier control points
                    // between pts[i] and pts[i+1] there should be inter[i*2] and inter[i*2+1] (cubic)

    void addIntermediate();
};


