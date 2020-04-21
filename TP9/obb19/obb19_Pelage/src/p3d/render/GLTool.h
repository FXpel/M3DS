#pragma once

#include <string>
#include <vector>

/**
@file
@author F. Aubert
@brief Useful draw : arrow, ground, texture
*/

#include "GLRender.h"
#include "GLPrimitive.h"
#include "GLDrawCamera.h"
#include "GLDraw.h"


namespace p3d {

class Texture;
class Vector4;
class Vector3;
class Vector2;
class Camera;
class Matrix4;


void initGLTool();

void drawArrow(const p3d::Vector3 &a,const p3d::Vector3 &u,double radius,const std::string &s1="",const std::string &s2="",double arrowRatio=0.1);
void drawGround(Texture &texture);
void drawTexture(Texture &texture, double x=0, double y=0, double w=0.5, double h=0.5, bool depthTexture=false);
void drawFrame(const Matrix4 &m, const std::string &name="", double size=1.0, int pos=0); // m[3] is the origin, then m[0] is x-axis, m[1] is y-axis, m[2] is z-axis. Ignores the fourth component.

}


