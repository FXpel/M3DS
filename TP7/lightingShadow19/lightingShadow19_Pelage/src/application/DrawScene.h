#pragma once

#include "Vector4.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshObject3D.h"

namespace p3d {
class Shader;
}

typedef enum {Pass_NoLighting=0x10000,Pass_PerVertexLighting=0x001,Pass_PerFragmentLighting=0x1000,Pass_Shadow=0x010,Pass_TexProj=0x100} EPass;

class DrawScene
{
public:
  DrawScene();

  void initialize();
  void animateStep();
  void draw();

  void currentShader(p3d::Shader *shader);
  void drawSimple();
  void depthMap(p3d::Texture *t);
  void diapoTexture(p3d::Texture *t);
  void textureEye(const p3d::Matrix4 &m);
  void worldProjectorMatrix(const p3d::Matrix4 &m);

  void useShaderPerVertexLighting();
  void useConstantColor();
  void enablePass(unsigned int p);
  void enablePass(EPass p);
private:
  p3d::Vector4 materialAmbient_;
  p3d::Vector3 materialDiffuse_,materialSpecular_;

  p3d::Texture groundTexture_,earthTexture_;
  p3d::Texture *depthMap_,*diapoTexture_;


  p3d::Shader perFragmentLighting_,perVertexLighting_,shadowMapShader_,textureTransform_,ambientTexture_,constantColor_;

  p3d::Shader *currentShader_;

  p3d::MeshObject3D airplane_,al_,car_;

  p3d::Matrix4 earthMatrix_,textureEyeMatrix_,worldProjectorMatrix_;

  double moveAngle_,angleStep_,moveZ_,zStep_,planeAngle_;

  p3d::Vector3 lightPositionEye_;

  unsigned int pass_{Pass_PerVertexLighting};



  void drawEarth();
  void drawGround();
  void drawObject();
  void useCurrentShader();
  void useAmbientTextureShader();
  void drawLight();
  void setLightPositionEye();
};


