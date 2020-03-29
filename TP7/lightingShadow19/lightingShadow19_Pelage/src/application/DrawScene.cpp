#include "DrawScene.h"

#include "GLTool.h"

using namespace p3d;

DrawScene::DrawScene() {
  car_.readInit("porsche.obj");
  car_.translate(0,-1.8,-2);
  car_.rotate(30,0,1,0);
  car_.scale(4,4,4);

  airplane_.readInit("cessna.obj");
  airplane_.translate(0,3,1);
  airplane_.scale(1.5);

  al_.readInit("al.obj");
  al_.translate(2,-1,1);
  al_.rotate(45,0,1,0);
  al_.scale(2,2,2);

  moveAngle_=0;
  angleStep_=0.1;
  moveZ_=0;
  zStep_=0.1;
  planeAngle_=0;

  depthMap_=nullptr;


}

void DrawScene::initialize() {
  initGLTool();
  glPointSize(4.0);


  glPixelStorei(GL_UNPACK_ALIGNMENT,4);
  glPixelStorei(GL_PACK_ALIGNMENT,4);



  p3d::initSphere(12,12);
  earthMatrix_=Matrix4::identity();
  earthMatrix_.translate(-3,1,-3);
  earthMatrix_.rotate(-73,1,0,0);
  earthMatrix_.scale(1.5);

  groundTexture_.read("mosaic_pierre.jpg");
  groundTexture_.generateMipmap();


  earthTexture_.read("EarthDay512.jpg");
  earthTexture_.generateMipmap();




  al_.initVAO();
  car_.initVAO();
  airplane_.initVAO();



  ambientTexture_.read("shader/ambientTexture");
  perFragmentLighting_.read("shader/perFragmentLighting");
  perVertexLighting_.read("shader/perVertexLighting");
  shadowMapShader_.read("shader/shadowMap");
  textureTransform_.read("shader/textureTransform");

  constantColor_.read("shader/constantColor");


}

void DrawScene::animateStep() {
  moveAngle_+=fabs(angleStep_)*3.0;
  moveZ_+=zStep_;
  planeAngle_+=angleStep_;
  if (moveZ_>4.0 || moveZ_<-4.0) {
    zStep_=-zStep_;
    angleStep_=-angleStep_;
  }
  car_.translate(zStep_/2.0,0,0,Coordinate_Local);
  airplane_.translate(0,0,zStep_/3);
  airplane_.rotate(-angleStep_*10,1,0,0);
  earthMatrix_.rotate(-angleStep_*10,0,0,1);

}


void DrawScene::enablePass(unsigned int p) {
  pass_=p;
}

void DrawScene::enablePass(EPass p) {
  pass_=p;
}


void DrawScene::draw() {

  // first pass : lighting
  if (pass_ & Pass_PerVertexLighting) {
    currentShader_=&perVertexLighting_;
    drawSimple();
  }
  else if (pass_ & Pass_PerFragmentLighting) {
    currentShader_=&perFragmentLighting_;
    drawSimple();
  }
  else if (pass_ & Pass_NoLighting) {
    currentShader_=&constantColor_;
    drawSimple();
  }


  // second pass : shadow and texturing (color blending with first pass)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_EQUAL);
  if (pass_ & Pass_Shadow) {
    currentShader_=&shadowMapShader_;
    drawSimple();
  }
  else if (pass_ & Pass_TexProj) {
    currentShader_=&textureTransform_;
    drawSimple();
  }
  materialAmbient_.a(0.5);
  currentShader_=&ambientTexture_;
  groundTexture_.bind(0);
  drawGround();
  earthTexture_.bind(0);
  drawEarth();
  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);

  drawLight();


  /*
  // post process : all frag dst have an alpha value set to 1
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_ZERO,GL_ONE,GL_ONE,GL_ZERO);
  p3d::modelviewMatrix=Matrix4::identity();
  p3d::projectionMatrix=Matrix4::identity();
  p3d::shaderVertexAmbient();
  p3d::drawSquare();
  glDisable(GL_BLEND);
*/

  glUseProgram(0);

}


/** ********************************************************************************
 *
 *
**/



void DrawScene::useShaderPerVertexLighting() {
  currentShader(&perVertexLighting_);
}



void DrawScene::useConstantColor() {
  currentShader(&constantColor_);
}




/** ******************************************************************* **/
/**
  Drawings
**/


void DrawScene::currentShader(Shader *shader) {
  currentShader_=shader;
}

void DrawScene::drawSimple() {
  setLightPositionEye();
  drawGround();
  drawEarth();
  drawObject();
}

void DrawScene::drawEarth() {
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix*=earthMatrix_;
  materialAmbient_.rgb(0.1,0.1,0.1);
  materialDiffuse_=Vector3(0.4,0.4,0.4);
  materialSpecular_=Vector3(0.8,0.8,0.8);
  useCurrentShader();
  currentShader_->uniform("repeat",1.0f);
  p3d::drawSphere();
  p3d::modelviewMatrix.pop();
}

void DrawScene::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.rotate(-90,1,0,0);
  p3d::modelviewMatrix.scale(20,20,1);
  materialAmbient_.rgb(0.1,0.1,0.1);
  materialDiffuse_=Vector3(0.5,0.5,0.5);
  materialSpecular_=Vector3(1,1,1);
  useCurrentShader();
  currentShader_->uniform("repeat",5.0f);
  p3d::drawSquare();
  p3d::modelviewMatrix.pop();
}



void DrawScene::drawObject() {
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix*=car_.worldLocal();
  materialAmbient_.rgb(0.1,0.1,0.1);
  materialDiffuse_=Vector3(0.4,0.7,0.4);
  materialSpecular_=Vector3(0.0,0.4,0.4);
  useCurrentShader();
  car_.draw();
  p3d::modelviewMatrix.pop();

  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix*=airplane_.worldLocal();
  materialAmbient_.rgb(0.1,0.1,0.1);
  materialDiffuse_=Vector3(0.3,0.2,0.9);
  materialSpecular_=Vector3(0.0,0.4,0.4);
  useCurrentShader();
  airplane_.draw();
  p3d::modelviewMatrix.pop();

  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix*=al_.worldLocal();
  materialAmbient_.rgb(0.1,0.1,0.1);
  materialDiffuse_=Vector3(0.3,0.7,0.6);
  materialSpecular_=Vector3(0.0,0.4,0.4);
  useCurrentShader();
  al_.draw();
  p3d::modelviewMatrix.pop();
}






void DrawScene::useCurrentShader() {
  currentShader_->use();
  currentShader_->uniform("mvp",p3d::projectionMatrix*p3d::modelviewMatrix);
  currentShader_->uniform("normalMatrix",p3d::modelviewMatrix.normalMatrix());
  currentShader_->uniform("modelviewMatrix",p3d::modelviewMatrix);
  currentShader_->uniform("lightPosition",lightPositionEye_);
  currentShader_->uniform("materialAmbient",materialAmbient_);
  currentShader_->uniform("materialDiffuse",materialDiffuse_);
  currentShader_->uniform("materialSpecular",materialSpecular_);
  currentShader_->uniform("image0",0);
  currentShader_->uniform("image1",1);

  glActiveTexture(GL_TEXTURE2);
  if (depthMap_) glBindTexture(GL_TEXTURE_2D,depthMap_->id());

  currentShader_->uniform("depthTexture",2);
  currentShader_->uniform("textureEyeMatrix",textureEyeMatrix_);
}

void DrawScene::useAmbientTextureShader() {
  ambientTexture_.use();
  ambientTexture_.uniform("mvp",p3d::projectionMatrix*p3d::modelviewMatrix);
  ambientTexture_.uniform("materialAmbient",materialAmbient_);
  ambientTexture_.uniform("image0",0);
  groundTexture_.bind(0);
}


void DrawScene::drawLight() {
  p3d::material(Vector4(0.1,0.1,0.1,1.0),Vector3(0.1,0.1,0.8),Vector3(0.0,0.4,0.4),40);
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix*=worldProjectorMatrix_;
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.scale(0.2);
  p3d::shaderLightPhong();
  p3d::drawSphere();
  p3d::modelviewMatrix.pop();
  p3d::modelviewMatrix.translate(0,0,-0.1);
  p3d::modelviewMatrix.scale(0.5,0.5,0.1);
  p3d::shaderLightPhong();
  p3d::drawCube();
  p3d::modelviewMatrix.pop();
}

void DrawScene::setLightPositionEye() { // should be called after camera setup in modelview
  lightPositionEye_=(p3d::modelviewMatrix*worldProjectorMatrix_).transformPoint(Vector3(0,0,0)); // the shaders require the lightPosition in Eye Coordinates
}

void DrawScene::depthMap(Texture *t) {
  depthMap_=t;
}

void DrawScene::diapoTexture(Texture *t) {
  diapoTexture_=t;
}

void DrawScene::textureEye(const Matrix4 &m) {
  textureEyeMatrix_=m;
}

void DrawScene::worldProjectorMatrix(const Matrix4 &m) {
  worldProjectorMatrix_=m;
}

