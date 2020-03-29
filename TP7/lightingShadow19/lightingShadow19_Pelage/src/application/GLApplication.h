#ifndef GLAPPLICATION_H
#define GLAPPLICATION_H


#include "GLWidget.h"
#include "Camera.h"
#include "MeshObject3D.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Vector4.h"

#include "DrawScene.h"

#include <string>


typedef enum {Overlay_None=0,Overlay_Color,Overlay_Depth} EOverlay;

class GLApplication : public GLWidget {
  Q_OBJECT
public:


  ~GLApplication() override;
  GLApplication();
  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/
  void leftPanel(size_t i, const std::string &s) override;
  QSize sizeHint() const override {return QSize(512,512);}
  /** ***** **/

  void updateCamera();

  void renderToTexture();

  void drawOverlay();

private:
  p3d::Camera camera_;
  double cameraFrame_;
  bool orbitalCamera_;

  bool animate_;

  p3d::Texture depthTexture_;
  p3d::Texture colorTexture_;
  p3d::Texture diapoTexture_;


  p3d::FrameBuffer rtt_;


  p3d::Matrix4 worldProjectorMatrix_,textureEyeMatrix_;

  EOverlay overlay_{Overlay_None};

  DrawScene drawScene_;
  double moveAngle_{0.0};


};

#endif // GLAPPLICATION_H



