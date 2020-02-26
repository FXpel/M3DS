#pragma once

#include "GLWidget.h"
#include "Tools.h"
#include "Matrix4.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Car.h"
#include "Mesh.h"
#include "Airplane.h"

#include <string>


class GLApplication : public GLWidget {
  Q_OBJECT  
public:
  typedef enum {Camera_Follow_Car,Camera_Car_Setup,Camera_Follow_Plane} ECameraMode;

  virtual ~GLApplication() override;
  GLApplication();
  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/
  void leftPanel(size_t i, const std::string &s) override;
  /** ***** **/

  void drawGround();
  void updateCamera();


  QSize sizeHint() const override {return QSize(512,512);}


private:
  Car car_;
  Airplane airplane_;
  p3d::Camera camera_,cameraStart_,cameraStop_;
  p3d::Texture ground_;
  ECameraMode cameraMode_;

  double lambda_=0.0;

  double angle_=0.0;


};


