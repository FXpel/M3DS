#include <cmath>
#include "GLTool.h"
#include "GLDrawCamera.h"

#include "GLText.h"
#include "GLDraw.h"
#include "Texture.h"

#include "Shader.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexArray.h"
#include "Camera.h"
#include "AngleAxis.h"

#include <vector>

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


void initGLTool() {
  initGLRender();
  initGLPrimitive();
}



void drawArrow(const p3d::Vector3 &a,const p3d::Vector3 &u,double radius,const std::string &s1,const std::string &s2,double arrowRatio) {
  if (fabs(u.length2())<0.000001) return;
  p3d::modelviewMatrix.push();

  Quaternion q;
  q=Quaternion::fromRotation(Vector3(0,0,1),u);
  //AngleAxis aa=q.toAngleAxis();
  p3d::modelviewMatrix.translate(a);
  p3d::modelviewMatrix.rotate(q);

  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.scale(radius,radius,u.length()*(1-arrowRatio));
  p3d::shaderVertexAmbient();
  p3d::drawCylinder();
  p3d::modelviewMatrix.pop();



  p3d::modelviewMatrix.translate(0,0,u.length()*1);
  p3d::modelviewMatrix.rotate(180,1,0,0);
  p3d::modelviewMatrix.scale(radius*2,radius*2,u.length()*arrowRatio);
  p3d::shaderLightPhong();
  p3d::drawCone();
  p3d::modelviewMatrix.pop();

  p3d::modelviewMatrix.push();

  if (s1!="")
  GLText::draw(s1,a.x(),a.y(),a.z(),nullptr,Mat4f(p3d::projectionMatrix*p3d::modelviewMatrix).v(),Vec4f(p3d::ambientColor).v());
  if (s2!="") {
    Vector3 end=a+u;
    GLText::draw(s2,end.x(),end.y(),end.z(),nullptr,Mat4f(p3d::projectionMatrix*p3d::modelviewMatrix).v(),Vec4f(p3d::ambientColor).v());
  }


  p3d::modelviewMatrix.pop();


}



void drawFrame(const p3d::Matrix4 &frame, const std::string &name, double size,int pos) {
  p3d::ambientColor=Vector4(1,0,0,1);
  p3d::drawArrow(frame.column(3).xyz(),frame.column(0).xyz()*size,0.01*size);
  p3d::ambientColor=Vector4(0,1,0,1);
  p3d::drawArrow(frame.column(3).xyz(),frame.column(1).xyz()*size,0.01*size);
  p3d::ambientColor=Vector4(0,0,1,1);
  p3d::drawArrow(frame.column(3).xyz(),frame.column(2).xyz()*size,0.01*size);
  Vector3 posString=frame.column(3).xyz()+frame.column(0).xyz();
  if (pos==1) posString+=Vector3(0,-0.15,0);
  p3d::draw(name,posString);
}

void drawTexture(p3d::Texture &texture,double x,double y,double w,double h,bool depthTexture) {
  texture.bind(0);
  p3d::modelviewMatrix.push();
  p3d::projectionMatrix.push();
  p3d::projectionMatrix=Matrix4::fromOrtho(0,1,0,1,0,1);
  // square is drawn in [-1,1]
  p3d::modelviewMatrix=Matrix4::identity();
  p3d::modelviewMatrix.translate(x,y,-0.00001);
  p3d::modelviewMatrix.scale(w/2.0,h/2.0,1);
  p3d::modelviewMatrix.translate(1,1,0);
  if (depthTexture)
    p3d::shaderTextureDepth();
  else
    p3d::shaderTextureAmbient();

  p3d::drawSquare();
  p3d::projectionMatrix.pop();
  p3d::modelviewMatrix.pop();
}

void drawGround(p3d::Texture &texture) {
  texture.bind(0);
  Vector4 saveLight=p3d::lightPosition[0];
  p3d::modelviewMatrix.push();
  p3d::ambientColor=Vector4(0,0,0,1);
  p3d::modelviewMatrix.rotate(-90,Vector3(1,0,0));
  p3d::lightPosition[0]=p3d::modelviewMatrix*Vector4(0,0,1);
  p3d::modelviewMatrix.scale(Vector3(50,50,1));
  p3d::shaderTextureLight();
  p3d::uniformTransformation();
  p3d::uniformLight();
  p3d::uniformMaterial();
  p3d::drawSquare();

  p3d::modelviewMatrix.pop();
  p3d::lightPosition[0]=saveLight;
}



} // namespace p3d



