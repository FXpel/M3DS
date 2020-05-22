#include "DebugTool.h"

#include "GLRender.h"
#include "GLDraw.h"
#include "Line.h"
#include "GLText.h"

#include <vector>

using namespace p3d;
using namespace std;

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

DebugTool::~DebugTool() {

}

DebugTool::DebugTool() {
}

void DebugTool::point(const p3d::Vector3 &p, const std::string &label,const p3d::Vector3 color) {
  position_.push_back(p);
  positionLabel_.push_back(label);
  positionColor_.push_back(color);

}

void DebugTool::line(const p3d::Line &l, const Vector3 &color) {
  line_.push_back(l);
  lineColor_.push_back(color);
}

void DebugTool::segment(const p3d::Line &l, const std::string &labelA, const std::string &labelB,const p3d::Vector3 &color) {
  segment_.push_back(l);
  segmentColor_.push_back(color);

  point(l.point(0),labelA,color);
  point(l.point(1),labelB,color);
}

void DebugTool::direction(const p3d::Vector3 &,const p3d::Vector3 &,const std::string &,const p3d::Vector3 &) {

}

void DebugTool::frame(const p3d::Matrix4 m, const std::string &label) {
  frame_.push_back(m);

  point(m.column(3).toPosition(),label);
  position_.push_back(Vector4(m.column(3)).toPosition());
  positionLabel_.push_back(label);
}


void DebugTool::drawPoint() {
  p3d::shaderVertexColor();
  p3d::draw(GL_POINTS,position_,"position",positionColor_,"color");
  for(unsigned int i=0;i<position_.size();++i) {
      Vector3 pos=position_[i];
      GLText::draw(positionLabel_[i],pos.x(),pos.y(),pos.z(),
                    Mat4f(p3d::projectionMatrix*p3d::modelviewMatrix).v(),nullptr,Vec4f(p3d::ambientColor).v());
  }
}

void DebugTool::drawSegment() {
  vector<Vector3> pts;
  vector<Vector3> color;

  for(unsigned int i=0;i<segment_.size();++i) {
      pts.push_back(segment_[i].point(0));
      pts.push_back(segment_[i].point(1));
      color.push_back(segmentColor_[i]);
      color.push_back(segmentColor_[i]);

  }
  p3d::draw(GL_LINES,pts,"position",color,"color");
}

void DebugTool::draw() {

  drawPoint();
  drawSegment();

}

void DebugTool::clear() {
  position_.clear();
  positionLabel_.clear();
  positionColor_.clear();


  line_.clear();
  lineColor_.clear();

  segment_.clear();
  segmentColor_.clear();

  directionStart_.clear();
  direction_.clear();
  directionLabel_.clear();
  directionColor_.clear();


  frame_.clear();

}


static DebugTool debugTool_;

void p3d::addDebug(const Vector3 &p1,const Vector3 &p2,const std::string &s,const Vector3 &color) {
  debugTool_.segment(Line(p1,p2-p1),s,"",color);
}

void p3d::clearDebug() {
  debugTool_.clear();
}

void p3d::drawDebug() {
  debugTool_.draw();
}


