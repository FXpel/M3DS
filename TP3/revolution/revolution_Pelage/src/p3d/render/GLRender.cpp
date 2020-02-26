#include "GLRender.h"
#include "Shader.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Tools.h"

#include <iostream>

/**

  @author F. Aubert
  @brief offers a (very) simple gl renderer for common operations (global projection, modelview, material, common shaders)
  **/

using namespace std;

namespace p3d {

static bool _isRenderInit=false;

const unsigned int nbLight=4;
static Shader *shader_;
Matrix4 projectionMatrix;
Matrix4 modelviewMatrix;
Matrix4 textureMatrix;
Matrix4 textureTransform;
Vector4 ambientColor;
Vector4 ambientBackColor;
Vector3 diffuseColor; // = front
Vector3 diffuseBackColor;
Vector3 specularColor;
vector<Vector4> lightPosition(nbLight);
vector<float> lightIntensity(nbLight);
float shininess;
float fragOffset;

static Shader *shaderLightPhong_=nullptr;
static Shader *shaderTextureLight_=nullptr;
static Shader *shaderVertexAmbient_=nullptr;
static Shader *shaderVertexColor_=nullptr;
static Shader *shaderTextureAmbient_=nullptr;
static Shader *shaderTextureTransform_=nullptr;
static Shader *shaderTexture3DAmbient_=nullptr;
static Shader *shaderTexture4DAmbient_=nullptr;
static Shader *shaderTextureDepth_=nullptr;
static Shader *textureTransform_=nullptr;

static vector<Shader*> shaderPredefined_;


void releaseGLRender() {
  for(auto i=shaderPredefined_.begin();i!=shaderPredefined_.end();++i) {
    delete *i;
  }
}

void initGLRender() {
  if (!_isRenderInit) {
    shaderPredefined_.push_back(shaderLightPhong_=new Shader());
    shaderPredefined_.push_back(shaderTextureLight_=new Shader());
    shaderPredefined_.push_back(shaderVertexAmbient_=new Shader());
    shaderPredefined_.push_back(shaderVertexColor_=new Shader());
    shaderPredefined_.push_back(shaderTextureAmbient_=new Shader());
    shaderPredefined_.push_back(shaderTextureTransform_=new Shader());
    shaderPredefined_.push_back(shaderTexture3DAmbient_=new Shader());
    shaderPredefined_.push_back(shaderTexture4DAmbient_=new Shader());
    shaderPredefined_.push_back(shaderTextureDepth_=new Shader());
    shaderPredefined_.push_back(textureTransform_=new Shader());



    for(unsigned int i=0;i<lightPosition.size();++i) {
      lightPosition[i]=Vector4(0,0,1,0);
      lightIntensity[i]=0;
    }
    lightIntensity[0]=1;

    string savePath=p3d::mediaPath();
    p3d::mediaPath(string(RENDER_SHADER_PATH));
    shaderLightPhong_->read("light_phong");

    shaderTextureLight_->read("texture_light");

    shaderVertexAmbient_->read("vertex_ambient");

    shaderVertexColor_->read("vertex_color");

    shaderTextureAmbient_->read("texture_ambient");

    shaderTexture3DAmbient_->read("texture3D_ambient");

    shaderTexture4DAmbient_->read("texture4D_ambient");

    shaderTextureDepth_->read("texture_depth");

    shaderTextureTransform_->read("texture_transform");


    modelviewMatrix=Matrix4::identity();
    projectionMatrix=Matrix4::identity();
    textureMatrix=Matrix4::identity();
    textureTransform=Matrix4::identity();

    lightPosition[0]=Vector4(0,0,0,1);
    _isRenderInit=true;
    p3d::mediaPath(savePath);
    p3d::fragOffset=0;

    ambientColor=Vector4(0.1,0.1,0.1);
    ambientBackColor=Vector4(0.1,0.1,0.1);
    diffuseColor=Vector3(0.8,0,0);
    diffuseBackColor=Vector3(0.2,0.8,0.0);
    specularColor=Vector3(0.7,0.1,0);
    shininess=100;
  }
}

void shaderLightPhong() {
  shader(shaderLightPhong_);

  uniformTransformation();
  uniformMaterial();
  uniformLight();
}

void shaderVertexAmbient() {
  shader(shaderVertexAmbient_);

  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("ambientBack",ambientBackColor);
  shader_->uniform("offset",fragOffset);
}

void shaderVertexColor() {
  shader(shaderVertexColor_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("offset",fragOffset);
}

void shaderTextureAmbient() {
  shader(shaderTextureAmbient_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("textureMatrix",textureMatrix);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("texture0",0);
}

void shaderTextureTransform() {
  shader(shaderTextureTransform_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("textureMatrix",textureMatrix);
  shader_->uniform("textureTransform",textureTransform);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("texture0",0);
}

void shaderTexture3DAmbient() {
  shader(shaderTexture3DAmbient_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("texture0",0);
}

void shaderTexture4DAmbient() {
  shader(shaderTexture4DAmbient_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("texture0",0);
}


void shaderTextureDepth() {
  shader(shaderTextureDepth_);
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("texture0",0);
}

void shaderTextureLight() {
  shader(shaderTextureLight_);
  uniformTransformation();
  uniformLight();
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("textureMatrix",textureMatrix);
  shader_->uniform("texture0",0);
}

void uniformProjection() {
  shader_->uniform("projectionMatrix",projectionMatrix);
}

void uniformModelview() {
  shader_->uniform("modelviewMatrix",modelviewMatrix);
  shader_->uniform("normalMatrix",modelviewMatrix.normalMatrix());
}

void uniformTransformation() {
  uniformProjection();
  uniformModelview();
  uniformMVP();
}

void uniformMVP() {
  shader_->uniform("mvp",projectionMatrix*modelviewMatrix);
}

void uniformAmbient() {
  shader_->uniform("ambient",ambientColor);
}

void uniformAmbient(const Vector4 &c) {
  ambientColor=c;
  uniformAmbient();
}

void uniformAmbient(const Vector3 &c) {
  ambientColor=Vector4(c,1);
  uniformAmbient();
}

void uniformAmbient(double x,double y,double z) {
  ambientColor=Vector4(x,y,z,1);
  uniformAmbient();
}

void uniformMaterial() {
  shader_->uniform("ambient",ambientColor);
  shader_->uniform("materialFrontDiffuse",diffuseColor);
  shader_->uniform("materialBackDiffuse",diffuseBackColor);
  shader_->uniform("materialSpecular",specularColor);
  shader_->uniform("materialShininess",shininess);
}

void uniformLight() {
  for(unsigned int i=0;i<lightPosition.size();++i) {
    QString is=QString::number(i);
    // TODO : template this kind of thing
    shader_->uniform((QString("lightPosition[")+is+"]").toStdString(),lightPosition[i]);
    shader_->uniform((QString("lightIntensity[")+is+"]").toStdString(),lightIntensity[i]);
  }
}

void material(const p3d::Vector4 &ambient,const p3d::Vector3 &diffuse,const p3d::Vector3 &specular,int shininess) {
  p3d::ambientColor=ambient;p3d::diffuseColor=diffuse;p3d::diffuseBackColor=diffuse;p3d::specularColor=specular;p3d::shininess=shininess;
}

void materialBlueGreen() {
  material(Vector4(0,0.1,0.1,1),Vector3(0,0.3,0.7),Vector3(0,0.8,0.2),200);
}

void materialFrontBack() {
  material(Vector4(0.1,0.1,0.1,1),Vector3(0.8,0.1,0.0),Vector3(0.6,0.6,0.6),200);
  diffuseBackColor=Vector3(0,0.8,0);
}


void shader(Shader *s) {
  shader_=s;
  shader_->use();
}

Shader *currentShader() {
  return shader_;
}





} // namespace p3d

