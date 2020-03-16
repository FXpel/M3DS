#pragma once

#include <vector>
#include <string>

#include "glsupport.h"


namespace p3d {

class Vector3;
class Vector2;


void drawLineStrip(const std::vector<p3d::Vector3> &point);
void drawLineStrip(const std::vector<p3d::Vector3> &point, size_t nbVertex);
void drawLineStrip(const std::vector<p3d::Vector2> &point);
void drawLineStrip(const std::vector<p3d::Vector2> &point, size_t nbVertex);

void drawLines(const std::vector<p3d::Vector3> &point, size_t nbVertex=0);
void drawLines(const std::vector<p3d::Vector2> &point, size_t nbVertex=0);
void drawPoints(const std::vector<p3d::Vector3> &point);
void drawPoints(const std::vector<p3d::Vector2> &point);
void drawPoints(const std::vector<Vector3> &point,const std::vector<Vector3> &color);
void drawPoints(const std::vector<Vector2> &point,const std::vector<Vector3> &color);
void drawGrid(const std::vector<Vector3> &point,size_t nbU);
void fillGrid(const std::vector<Vector3> &point,size_t nbU);
void fillGrid(const std::vector<Vector3> &point, const std::vector<Vector3> &normal, size_t nbU);
void drawPolygon(const std::vector<p3d::Vector3> &position, const std::vector<Vector3> &normal, bool fill=false, float scale=1.0f);
void drawPolygon(const std::vector<p3d::Vector3> &position, bool fill=false, float scale=1.0);
void draw(GLenum prim,const std::vector<Vector3> &point);
void draw(GLenum prim, const std::vector<Vector3> &a0, const std::string &na0, const std::vector<Vector3> &a1, const std::string &na1);
}

