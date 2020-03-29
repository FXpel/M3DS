#pragma once

#include <cstddef>

/**
@file
@author F. Aubert
@brief Cone, Cube, Cylinder, Sphere, Torus, Square, Segment, Lines, Grid
*/

namespace p3d {

/** PRIMITIVES **/
void initGLPrimitive();
void initCube();
void initCylinder(bool cap=true,size_t nbSlice=20);
void initCone(bool cap=true,size_t nbSlice=20);
void initSphere(size_t nbSlice=20,size_t nbStack=20);
void initTorus(size_t nbSlice=20,size_t nbStack=20, double rg=2.0, double rp=0.2);
void initGrid(size_t =20, size_t =20);
void initSquare();
void initSegment();


void drawSquare(double size=2.0);
void drawCube(double size=2.0);
void drawCylinder(double size=2.0);
void drawSphere(double size=1.0);
void drawCone(double size=1.0);
void drawTorus(double size=1.0);


}




