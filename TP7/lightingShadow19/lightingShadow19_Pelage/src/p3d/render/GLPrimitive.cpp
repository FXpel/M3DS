#include "GLPrimitive.h"

#include "Tools.h"

#include "Vector2.h"

#include "VertexArray.h"
#include "GLDraw.h"

#include "Vector3.h"


#include <cmath>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace std;

namespace p3d {
static bool _isPrimitiveInit=false;

void initGLPrimitive() {
  if (!_isPrimitiveInit) {
    initSquare();
    initCylinder(true,20);
    initSphere(20,20);
    initCube();
    initCone(true,30);
    initTorus(10,40,5.0,2.0);
    initSegment();
    _isPrimitiveInit=true;
  }
}




static VertexArray square_(GL_TRIANGLE_STRIP);
static VertexArray cylinder_(GL_TRIANGLE_STRIP);
static VertexArray cylinderCap_(GL_TRIANGLE_FAN);
static VertexArray cube_(GL_TRIANGLE_STRIP);
static VertexArray sphere_(GL_TRIANGLE_STRIP);
static VertexArray cone_(GL_TRIANGLES);
static VertexArray coneCap_(GL_TRIANGLE_FAN);
static VertexArray torus_(GL_TRIANGLE_STRIP);
static VertexArray grid_(GL_TRIANGLES);



static std::vector<float> position_;
static std::vector<float> normal_;
static std::vector<float> texCoord_;

static Vector3 currentNormal_;
static Vector2 currentTexCoord_;

void resetAttrib() {
  position_.clear();normal_.clear();texCoord_.clear();
}

void normalAttrib(const p3d::Vector3 &n) {
  currentNormal_=normalize(n);
}

void normalAttrib(double x,double y,double z) {
  normalAttrib(Vector3(x,y,z));
}

void texCoordAttrib(const p3d::Vector2 &t) {
  currentTexCoord_=t;
}

void texCoordAttrib(double s,double t) {
  texCoordAttrib(Vector2(s,t));
}

void vertexAttrib(const p3d::Vector3 &v) {
  Vec3f vf(v);
  Vec3f nf(currentNormal_);
  Vec2f tf(currentTexCoord_);
  position_.push_back(v.xf());
  position_.push_back(v.yf());
  position_.push_back(v.zf());
  normal_.push_back(currentNormal_.xf());
  normal_.push_back(currentNormal_.yf());
  normal_.push_back(currentNormal_.zf());
  texCoord_.push_back(currentTexCoord_.xf());
  texCoord_.push_back(currentTexCoord_.yf());
}

void addCommonAttrib(VertexArray *vao) {
  vao->add("position",position_,3,0);
  vao->add("normal",normal_,3,1);
  vao->add("texCoord",texCoord_,2,2);
}

void vertexAttrib(double x,double y,double z) {
  vertexAttrib(Vector3(x,y,z));
}

void initSquare() {
  square_.release();
  std::vector<float> vertex{
    -1.0,1.0,0.0,
    -1.0,-1.0,0.0,
    1.0,1.0,0.0,
    1.0,-1.0,0.0
  };
  std::vector<float> normal{
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0,
    0.0,0.0,1.0
  };
  std::vector<float> texCoord{
    0.0,1.0,
    0.0,0.0,
    1.0,1.0,
    1.0,0.0
  };

  square_.add("position",vertex,3,0);
  square_.add("normal",normal,3,1);
  square_.add("texCoord",texCoord,2,2);
}

void initConeCap(size_t nbSlice) {
  coneCap_.release();
  resetAttrib();
  double theta=0.0;
  double stepTheta=2.0*M_PI/nbSlice;
  double x,y;
  normalAttrib(Vector3(0,0,1));
  texCoordAttrib(Vector2(0.5,0.5));
  vertexAttrib(Vector3(0,0,1));
  for(size_t i=0;i<nbSlice;++i) {
    x=cos(theta);y=sin(theta);
    texCoordAttrib(Vector2(x/2.0+0.5,y/2.0+0.5));
    vertexAttrib(Vector3(x,y,1.0));
    theta+=stepTheta;
  }
  texCoordAttrib(1.0,0.5);
  vertexAttrib(1,0,1);
  addCommonAttrib(&coneCap_);
}

void initCone(bool cap,size_t nbSlice) {
  cone_.release();
  resetAttrib();
  double step=2.0*M_PI/nbSlice;
  double theta=0.0;
  double x,y;
  for(size_t i=0;i<nbSlice+1;++i) {
    x=cos(theta);
    y=sin(theta);
    normalAttrib(Vector3(x/sqrt(2),y/sqrt(2),-1.0/sqrt(2)));
    texCoordAttrib(Vector2(theta/2.0/M_PI,0));
    vertexAttrib(Vector3(x,y,1.0));

    theta+=step/2.0;
    x=cos(theta);
    y=sin(theta);
    normalAttrib(Vector3(x/sqrt(2),y/sqrt(2),-1.0/sqrt(2)));
    texCoordAttrib(Vector2(theta/2.0/M_PI,1));
    vertexAttrib(Vector3(0,0,0));

    theta+=step/2.0;
    x=cos(theta);
    y=sin(theta);
    normalAttrib(Vector3(x/sqrt(2),y/sqrt(2),-1.0/sqrt(2)));
    texCoordAttrib(Vector2(theta/2.0/M_PI,0));
    vertexAttrib(Vector3(x,y,1.0));

  }
  addCommonAttrib(&cone_);
  if (cap) {
    initConeCap(nbSlice);
  }

}

void initGrid(int ,int ) {
  /*
  double stepX=2.0/nbSlice,stepY=2.0/nbStack;
  for(int i=0;i<nbStack;++i) {
    for(int j=0;j<nbSlice;++j) {

    }
  }
  */
}

void initTorus(size_t nbSlice,size_t nbStack,double rg,double rp) {
  torus_.release();

  double theta=0.0; // angle for external radius
  double phi=0.0;   // angle for internal radius
  double stepTheta=2.0*M_PI/nbStack;
  double stepPhi=2.0*M_PI/nbSlice;

  double nx0,ny0,nz0,nx1,ny1,nz1;
  double x0,y0,z0;
  double x1,y1,z1;
  resetAttrib();
  for(size_t i=0;i<nbStack+1;++i) {
    phi=0.0;
    for(size_t j=0;j<nbSlice+1;++j) {
      if (j==nbSlice) phi=0.0; // lazy-junction
      nx1=cos(phi);
      ny1=0.0;
      nz1=sin(phi);
      x1=rp*nx1+(rg-rp);
      y1=0.0;
      z1=rp*nz1;

      nx0=(nx1*cos(theta)-ny1*sin(theta));
      ny0=(nx1*sin(theta)+ny1*cos(theta));
      nz0=nz1;

      x0=(x1*cos(theta)-y1*sin(theta));
      y0=(x1*sin(theta)+y1*cos(theta));
      z0=z1;
      normalAttrib(nx0,ny0,nz0);
      texCoordAttrib(theta/(2.0*M_PI),phi/(2.0*M_PI));
      vertexAttrib(x0,y0,z0);
      phi+=stepPhi;
    }
    theta+=stepTheta;
  }
  addCommonAttrib(&torus_);
  vector<GLuint> elem;
  elem.clear();
  for(size_t i=0;i<nbStack;++i) {
    for(size_t j=0;j<nbSlice+1;++j) {
      elem.push_back(GLuint(i*(nbSlice+1)+j));
      elem.push_back(GLuint(i*(nbSlice+1)+nbSlice+1+j));
    }
  }
  torus_.element(elem);
  torus_.restart((nbSlice+1)*2);

}





void initCylinderCap(size_t nbSlice) {
  resetAttrib();
  double theta=0.0;
  double stepTheta=2.0*M_PI/nbSlice;
  double x,y,z;
  for(size_t cap=0;cap<2;++cap) {
    theta=0.0;
    if (cap==0) z=1.0; else {z=0.0;stepTheta=-stepTheta;}
    normalAttrib(Vector3(0,0,z*2-1));
    texCoordAttrib(Vector2(0.5,0.5));
    vertexAttrib(Vector3(0,0,z));
    for(size_t i=0;i<nbSlice;++i) {
      x=cos(theta);y=sin(theta);
      texCoordAttrib(Vector2(x/2.0+0.5,y/2.0+0.5));
      vertexAttrib(Vector3(x,y,z));
      theta+=stepTheta;
    }
    texCoordAttrib(1.0,0.5);
    vertexAttrib(1,0,z);
  }
  addCommonAttrib(&cylinderCap_);
  cylinderCap_.restart(nbSlice+2);
}

void initCylinder(bool cap,size_t nbSlice) {
  resetAttrib();
  double step=2.0*M_PI/nbSlice;
  double theta=0.0;
  double x,y;
  for(size_t i=0;i<nbSlice;i++) {
    x=cos(theta);
    y=sin(theta);
    normalAttrib(Vector3(x,y,0.0));
    texCoordAttrib(Vector2(theta/2.0/M_PI,0));
    vertexAttrib(Vector3(x,y,1.0));

    texCoordAttrib(Vector2(theta/2.0/M_PI,1.0));
    vertexAttrib(Vector3(x,y,0.0));

    theta+=step;
  }
  normalAttrib(Vector3(1.0,0.0,0.0));
  texCoordAttrib(Vector2(1.0,0.0));
  vertexAttrib(Vector3(1.0,0.0,1.0));

  texCoordAttrib(Vector2(1.0,1.0));
  vertexAttrib(Vector3(1.0,0.0,0.0));
  addCommonAttrib(&cylinder_);
  if (cap) {
    initCylinderCap(nbSlice);
  }
}


void initSphere(size_t nbSlice,size_t nbStack) {
  sphere_.release();
  resetAttrib();
  double stepTheta=2.0*M_PI/nbSlice;
  double stepPhi=M_PI/nbStack;
  double phi=-M_PI/2.0;
  for(size_t i=0;i<nbStack+1;++i) {
    double theta=0.0;
    double x,y,z;
    z=sin(phi);
    for(size_t j=0;j<nbSlice+1;++j) {
      x=cos(theta)*cos(phi);
      y=sin(theta)*cos(phi);
      normalAttrib(Vector3(x,y,z));
      texCoordAttrib(Vector2(theta/2.0/M_PI,phi/M_PI+0.5));
      vertexAttrib(Vector3(x,y,z));
      theta+=stepTheta;
    }
    phi+=stepPhi;
  }
  vector<GLuint> elem;
  elem.clear();
  for(size_t i=0;i<nbStack;++i) {
    for(size_t j=0;j<nbSlice+1;++j) {
      elem.push_back(GLuint((i+1)*(nbSlice+1)+j));
      elem.push_back(GLuint(i*(nbSlice+1)+j));
    }
  }
  sphere_.element(elem);
  addCommonAttrib(&sphere_);
  sphere_.restart((nbSlice+1)*2);
}

void initCube() {
  std::vector<float> vertex{
    -1.0,1.0,1.0, // face z=1
    -1.0,-1.0,1.0,
    1.0,1.0,1.0,
    1.0,-1.0,1.0,
    -1.0,-1.0,-1.0, // face z=-1
    -1.0,1.0,-1.0,
    1.0,-1.0,-1.0,
    1.0,1.0,-1.0,
    1.0,-1.0,1.0, // face x=1
    1.0,-1.0,-1.0,
    1.0,1.0,1.0,
    1.0,1.0,-1.0,
    -1.0,-1.0,-1.0, // face x=-1
    -1.0,-1.0,1.0,
    -1.0,1.0,-1.0,
    -1.0,1.0,1.0,

    -1.0,1.0,-1.0, // face y=1
    -1.0,1.0,1.0,
    1.0,1.0,-1.0,
    1.0,1.0,1.0,

    -1.0,-1.0,1.0,  // face y=-1
    -1.0,-1.0,-1.0,
    1.0,-1.0,1.0,
    1.0,-1.0,-1.0
  };
  std::vector<float> normal {
    0,0,1, // face z=1
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,-1, // face z=-1
    0,0,-1,
    0,0,-1,
    0,0,-1,
    1,0,0, // face x=1
    1,0,0,
    1,0,0,
    1,0,0,
    -1,0,0, // face x=-1
    -1,0,0,
    -1,0,0,
    -1,0,0,
    0,1,0, // face y=1
    0,1,0,
    0,1,0,
    0,1,0,
    0,-1,0, // face y=-1
    0,-1,0,
    0,-1,0,
    0,-1,0
  };
  std::vector<float> texCoord {
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,
    0.0,1.0, // face
    0.0,0.0,
    1.0,1.0,
    1.0,0.0,

  };
  cube_.add("position",vertex,3,0);
  cube_.add("normal",normal,3,1);
  cube_.add("texCoord",texCoord,2,2);
  cube_.restart(4);

}

void initSegment() {
  // TODO : GL memory leak (change GLRender for empty buffer
}


void drawSquare(double) {
  square_.draw();
}

void drawCylinder(double) {
  cylinder_.draw();
  cylinderCap_.draw();
}

void drawCube(double) {
  cube_.draw();
}

void drawSphere(double) {
  sphere_.draw();
}

void drawCone(double) {
  cone_.draw();
  coneCap_.draw();
}

void drawTorus(double) {
  torus_.draw();
}








}



