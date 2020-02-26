#pragma once

#include "GLWidget.h"
#include "Tools.h"

#include <string>

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
  QSize sizeHint() const override {return QSize(1000,1000);}
  /** ***** **/
  GLuint initProgram(const std::string &filename);
  void initTriangleBuffer();
  void initTriangleVAO();
  void initTexture();
  void initStrip(int nbSlice,float xmin,float xmax,float ymin,float ymax);
  void initRing(int nbSlice,float r0,float r1);
private:
  std::vector<float> trianglePosition_;
  std::vector<float> triangleColor_;
  std::vector<float> triangleTexCoord_;
  float coeff_;
  bool f;

  GLuint trianglePositionBuffer_;
  GLuint triangleColorBuffer_;
  GLuint triangleTexCoordBuffer_;
  std::vector<unsigned int> elementData_;
  GLuint elementBuffer_;
  GLuint triangleVAO_;
  GLuint shader0_;
  GLuint textureId_;



};


