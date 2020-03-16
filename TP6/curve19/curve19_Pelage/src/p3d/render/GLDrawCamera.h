#pragma once

/*
 *
 *
 */

#include <vector>
#include <string>

namespace p3d {

class Camera;
class Vector4;
class Vector3;
class Vector2;

void drawThickLineStrip(const std::vector<Vector3> &p);
void apply(const p3d::Camera &c);

void draw(const std::string &s,const Vector3 &pos);
void draw(const std::string &s, const Vector2 &pos, const Vector4 &color);
void draw(const std::string &s,const Vector2 &pos);
void draw(int v,const Vector3 &pos);

}

