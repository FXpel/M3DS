#include "GLDraw.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexArray.h"

#include "Tools.h"

using namespace p3d;
using namespace std;

static VertexArray *gridPoint_=nullptr;
static VertexArray *pointsPoint_=nullptr;
static VertexArray *lineStripPoint_=nullptr;
static VertexArray *linesPoint_=nullptr;
static VertexArray *pointsGen_=nullptr;


void p3d::drawLineStrip(const std::vector<Vector3> &point) {
  drawLineStrip(point,point.size());
}


void p3d::drawLineStrip(const std::vector<Vector3> &point,size_t nbVertex) {
  if (true) { // TODO : incompatibility between 2D and 3D lineStrip for update (TODO : better way => distinct lineStrip for 2D and 3D...)
  //if (!_lineStripPoint || _lineStripPoint->nbVertex()!=nbVertex) {
    delete lineStripPoint_;
    lineStripPoint_=new VertexArray(GL_LINE_STRIP);
    lineStripPoint_->add("position",point,0);
  }
  else {
    /*
    lineStripPoint_->update("position",point);
    */
  }
  lineStripPoint_->draw(nbVertex);
}




void p3d::drawLineStrip(const std::vector<Vector2> &point,size_t nbVertex) {
  if (nbVertex==0) nbVertex=point.size();
  if (true) {
  //if (!_lineStripPoint || _lineStripPoint->nbVertex()!=nbVertex) {
    delete lineStripPoint_;
    lineStripPoint_=new VertexArray(GL_LINE_STRIP);
    lineStripPoint_->add("position",point,0);
  }
  else {
    /*
    lineStripPoint_->update("position",point);
    */
  }
  lineStripPoint_->draw(nbVertex);
}



void p3d::fillGrid(const std::vector<Vector3> &point,size_t nbU) {
  if (!gridPoint_ || gridPoint_->nbVertex()!=point.size()) {
    delete gridPoint_;
    gridPoint_=new VertexArray(GL_TRIANGLE_STRIP);
    gridPoint_->add("position",point,0);
    vector<GLuint> element((point.size()-nbU)*2);
    size_t index=0;
    for(size_t i=0;i<point.size()-nbU;++i) {
      element[index++]=GLuint(i);
      element[index++]=GLuint(i+nbU);
    }
    gridPoint_->element(element);
    gridPoint_->restart(2*nbU);
  }
  else {
    gridPoint_->update("position",point);
  }
  gridPoint_->draw();
}

void p3d::fillGrid(const std::vector<Vector3> &point,const std::vector<Vector3> &normal,size_t nbU) {
  if (true) {
  //if (!_gridPoint || _gridPoint->nbVertex()!=int(point.size())) {
    delete gridPoint_;
    gridPoint_=new VertexArray(GL_TRIANGLE_STRIP);
    gridPoint_->add("position",point,0);
    gridPoint_->add("normal",normal,1);
    vector<GLuint> element((point.size()-nbU)*2);
    size_t index=0;
    for(size_t i=0;i<point.size()-nbU;++i) {
      element[index++]=GLuint(i);
      element[index++]=GLuint(i+nbU);
    }
    gridPoint_->element(element);
    gridPoint_->restart(2*nbU);
  }
  else {
    /*
    gridPoint_->update("position",point);
    gridPoint_->update("normal",normal);
    */
  }
  gridPoint_->draw();
}


void p3d::drawGrid(const std::vector<Vector3> &point,size_t nbU) {
  if (!gridPoint_ || gridPoint_->nbVertex()!=point.size()) {
    delete gridPoint_;
    gridPoint_=new VertexArray(GL_LINES);
    gridPoint_->add("position",point,0);
    vector<GLuint> element(point.size()*4-nbU*2);
    size_t index=0;
    for(size_t i=0;i<point.size();++i) {
      if (i<(point.size()-nbU)) {
        element[index++]=GLuint(i);
        element[index++]=GLuint(i+nbU);
      }
      if (i%nbU) {
        element[index++]=GLuint(i);
        element[index++]=GLuint(i-1);
      }
    }
    gridPoint_->element(element);
  }
  else {
    gridPoint_->update("position",point);
  }
  gridPoint_->draw();
}


void p3d::drawLines(const std::vector<Vector3> &point,size_t nbVertex) {
  if (nbVertex==0) nbVertex=point.size();
  if (!linesPoint_ || linesPoint_->nbVertex()!=nbVertex) {
    delete linesPoint_;
    linesPoint_=new VertexArray(GL_LINES);
    linesPoint_->add("position",point,0);
  }
  else {
    linesPoint_->update("position",point);
  }
  linesPoint_->draw(nbVertex);
}

void p3d::drawLines(const std::vector<Vector2> &point,size_t nbVertex) {
  if (nbVertex==0) nbVertex=point.size();
  if (!linesPoint_ || linesPoint_->nbVertex()!=nbVertex) {
    delete linesPoint_;
    linesPoint_=new VertexArray(GL_LINES);
    linesPoint_->add("position",point,0);
  }
  else {
    linesPoint_->update("position",point);
  }
  linesPoint_->draw(nbVertex);
}


void p3d::drawPoints(const std::vector<Vector3> &point) {
  /*
  if (point.empty()) {
    if (!_pointsPoint) {
      return;
      throw ErrorD("_pointsPoint VAO is empty (should be initialized before)");
    }
  }
  else {
  */
  {
    if (true) {//!_pointsPoint || _pointsPoint->nbVertex()!=point.size()) {
      delete pointsPoint_;
      pointsPoint_=new VertexArray(GL_POINTS);
      pointsPoint_->add("position",point,0);
    }
    else {
      /*
      pointsPoint_->update("position",point);
      */
    }
  }
  pointsPoint_->draw();
}

// TODO : incompatibility 2D <=> 3D (when updates VAO)
void p3d::drawPoints(const std::vector<Vector2> &point) {
  if (!pointsPoint_ || size_t(pointsPoint_->nbVertex()) !=point.size()) {
      delete pointsPoint_;
      pointsPoint_=new VertexArray(GL_POINTS);
      pointsPoint_->add("position",point,0);
  }
  else {
    pointsPoint_->update("position",point);
  }
  pointsPoint_->draw();
}


void p3d::draw(GLenum prim,const std::vector<Vector3> &point) {
  if (point.empty()) {
    if (!pointsGen_) {
      return;
      throw ErrorD("_pointsPoint VAO is empty (should be initialized before)");
    }
  }
  else {
    if (true) {//!_pointsPoint || _pointsPoint->nbVertex()!=point.size()) {
      delete pointsGen_;
      pointsGen_=new VertexArray(prim);
      pointsGen_->add("position",point,0);
    }
    else {
      /*
      pointsGen_->update("position",point);
      */
    }
  }
  pointsGen_->draw();
}

void p3d::draw(GLenum prim,const std::vector<Vector3> &a0,const std::string &na0,const std::vector<Vector3> &a1,const std::string &na1) {
  if (true) {//!_pointsPoint || _pointsPoint->nbVertex()!=point.size()) { // TODO : should verify attribute names also
    delete pointsGen_;
    pointsGen_=new VertexArray(prim);
    pointsGen_->add(na0,a0,0);
    pointsGen_->add(na1,a1,1);
  }
  else {
    /*
    pointsGen_->update(na0,a0);
    pointsGen_->update(na1,a1);
    */
  }
  pointsGen_->draw();
}

void p3d::drawPoints(const std::vector<Vector3> &point,const std::vector<Vector3> &color) {
  if (point.empty()) {
    if (!pointsPoint_) {
      return;
      throw ErrorD("_pointsPoint VAO is empty (should be initialized before)");
    }
  }
  else {
    if (true) {//!_pointsPoint || _pointsPoint->nbVertex()!=point.size()) {
      delete pointsPoint_;
      pointsPoint_=new VertexArray(GL_POINTS);
      pointsPoint_->add("position",point,0);
      pointsPoint_->add("color",color,1);
    }
    else {
      /*
      pointsPoint_->update("position",point);
      pointsPoint_->update("color",color);
      */
    }
  }
  pointsPoint_->draw();
}


/* very slow : triangulation, works only for *one* polygon, vao is generated at each call */
void p3d::drawPolygon(const std::vector<p3d::Vector3> &position, bool fill, float scale) {
  vector<Vector3> normal;
  normal.clear();
  drawPolygon(position,normal,fill,scale);
}

/* very slow : triangulation, works only for *one* polygon, vao is generated at each call */
void p3d::drawPolygon(const std::vector<p3d::Vector3> &position,const std::vector<p3d::Vector3> &normal,bool fill,float scale) {
  Vector3 bary(0,0,0);
  bool setNormal=normal.size()>0;
  if (setNormal && (normal.size()!=position.size())) {
    throw ErrorD("incoherent : normal.size() should be = pts.size()");
  }
  vector<float> pts(position.size()*3);
  vector<float> n;
  if (setNormal) n.resize(normal.size()*3);
  vector<GLuint> elem(position.size());
  if (scale!=1.0f) {
    for(auto &p:position) {
      bary+=p;
    }
    bary/=position.size();
  }
  for(size_t i=0;i<position.size();++i) {
    pts[i*3]=(position[i].xf()-bary.xf())*scale+bary.xf();
    pts[i*3+1]=(position[i].yf()-bary.yf())*scale+bary.yf();
    pts[i*3+2]=(position[i].zf()-bary.zf())*scale+bary.zf();
    if (setNormal) {
      n[i*3]=normal[i].xf();
      n[i*3+1]=normal[i].yf();
      n[i*3+2]=normal[i].zf();
    }
    elem[i]=GLuint(i);
  }
  VertexArray vao(GL_LINE_LOOP);
  if (fill) {
    elem.push_back(0);
    vao.mode(GL_TRIANGLE_FAN);
  }
  vao.add("position",pts,3,0);
  if (setNormal) vao.add("normal",n,3,1);
  vao.element(elem);
  vao.draw();
}


