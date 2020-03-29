#include "GLDrawCamera.h"

#include "GLDraw.h"
#include "GLRender.h"
#include "Camera.h"
#include "GLText.h"

#include "Vector2.h"

using namespace std;

namespace p3d {
void apply(const p3d::Camera &c) {
  p3d::projectionMatrix=c.projectionMatrix();
  p3d::modelviewMatrix=c.cameraWorld();
  glViewport(c.viewX(),c.viewY(),c.viewWidth(),c.viewHeight());
}


void drawThickLineStrip(const std::vector<Vector3> &p) {
  if (p.size()<2) return;
    Vector3 dir;
    Matrix4 t=projectionMatrix*modelviewMatrix;
    Vector3 p0,p1;
    Vector4 p0h,p1h;
    vector<Vector3> pts;

    Vector4 vv(0,0,0,1);
    //Vector4 vvh=projectionMatrix*vv;

    pts.clear();
    double bias=0.9999; // bias for clipping close to the near plane (debug purpose)
    for(unsigned int i=0;i<p.size()-1;++i) {
      p0h=t*Vector4(p[i],1);
      p1h=t*Vector4(p[i+1],1);

      /* own clipping with near plane (the case w close to 0 is impossible to treat for 3D operation to make the thick line */
      if (p0h.z()<-bias*p0h.w() && p1h.z()<-bias*p1h.w()) continue;
      else if (p0h.z()<-bias*p0h.w() || p1h.z()<-bias*p1h.w()) {
        double lambda=(bias*p0h.w()+p0h.z())/((bias*p0h.w()+p0h.z())-(bias*p1h.w()+p1h.z()));
        if (lambda<0 || lambda>1) continue;
        Vector4 pih=(1-lambda)*p0h+lambda*p1h;
        if (p0h.z()<-fabs(bias*p0h.w())) {
//          cout << "before p0 : " << p0h.w() << " ";
          p0h=pih;

        }
        else {
//          cout << "before p1 : " << p1h.w() << " ";
          p1h=pih;
        }
//        cout << pih.z() << " et " << pih.w() << " div =" << pih.z()/pih.w() << endl;
      }
      /* clipping done */

      p0=p0h.toPosition();
      p1=p1h.toPosition();

      dir=p1-p0;

      dir=normalize(Vector3(-dir.y(),dir.x(),0)); // TODO : except for null direction
      dir*=0.01;
      pts.push_back(p0-dir);
      pts.push_back(p1-dir);
      pts.push_back(p0+dir);

      pts.push_back(p0+dir);
      pts.push_back(p1-dir);
      pts.push_back(p1+dir);
    }
    modelviewMatrix.push();
    projectionMatrix.push();
    modelviewMatrix=Matrix4::identity();
    projectionMatrix=Matrix4::identity();
    p3d::shaderVertexAmbient();
    p3d::draw(GL_TRIANGLES,pts);
    projectionMatrix.pop();
    modelviewMatrix.pop();
}


void draw(const std::string &s,const Vector3 &pos) {
  glDepthMask(GL_FALSE);
  GLText::draw(s,pos.x(),pos.y(),pos.z(),Mat4f(p3d::projectionMatrix*p3d::modelviewMatrix).v(),nullptr,Vec4f(p3d::ambientColor).v());
  glDepthMask(GL_TRUE);
}

void draw(const std::string &s,const Vector2 &pos) {
  glDepthFunc(GL_ALWAYS);
  GLText::draw(s,pos.x(),pos.y(),0,nullptr,nullptr,Vec4f(p3d::ambientColor).v());
  glDepthFunc(GL_LESS);
}


void draw(const std::string &s,const Vector2 &pos,const Vector4 &color) {
  glDepthFunc(GL_ALWAYS);
  GLText::draw(s,pos.x(),pos.y(),0,nullptr,nullptr,Vec4f(color).v());
  glDepthFunc(GL_LESS);
}


void draw(int v,const Vector3 &pos) {draw(toString(v),pos);}




}

