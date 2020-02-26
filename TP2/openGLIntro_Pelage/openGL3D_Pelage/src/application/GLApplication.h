#pragma once

#include "Mesh.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "GLWidget.h"
#include "BasicMesh.h"
#include "Shader.h"

#include <string>

namespace p3d {
class Shader;
}

class GLApplication : public GLWidget {
  Q_OBJECT  
public:
  GLApplication();
  ~GLApplication() override;


  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/
  void leftPanel(size_t i, const std::string &s) override;
  /** ***** **/
  QSize sizeHint() const override {return QSize(512,512);}


private:
  p3d::Shader shader_;

  p3d::Mesh obj_;
  BasicMesh basicMesh_;
  p3d::Matrix4 projection_;
  p3d::Matrix4 transform_;
  p3d::Vector3 lightPosition_;

  double angle_;



};


