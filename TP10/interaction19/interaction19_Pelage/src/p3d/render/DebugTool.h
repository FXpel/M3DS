#pragma once

#include <vector>

#include "Vector3.h"

namespace p3d {

class Line;
class Matrix4;

class DebugTool {
public:
  virtual ~DebugTool();
  DebugTool();

  void point(const p3d::Vector3 &p, const std::string &label, const p3d::Vector3 color=p3d::Vector3(0,0,0));
  void line(const p3d::Line &l,const p3d::Vector3 &color);
  void segment(const p3d::Line &l,const std::string &labelA,const std::string &labelB,const p3d::Vector3 &color=p3d::Vector3(0,0,0));
  void frame(const p3d::Matrix4 m,const std::string &label);
  void direction(const p3d::Vector3 &p, const p3d::Vector3 &u, const std::string &label, const p3d::Vector3 &color);

  void drawPoint();
  void drawSegment();
  void draw();
  void clear();


private:
  std::vector<p3d::Vector3> position_;
  std::vector<std::string> positionLabel_;
  std::vector<p3d::Vector3> positionColor_;

  std::vector<p3d::Line> line_;
  std::vector<p3d::Vector3> lineColor_;

  std::vector<p3d::Line> segment_;
  std::vector<p3d::Vector3> segmentColor_;

  std::vector<p3d::Vector3> directionStart_;
  std::vector<p3d::Vector3> direction_;
  std::vector<std::string> directionLabel_;
  std::vector<p3d::Vector3> directionColor_;

  std::vector<p3d::Matrix4> frame_;
};

void addDebug(const Vector3 &p1,const Vector3 &p2,const std::string &s,const Vector3 &color);
void clearDebug();
void drawDebug();
}


