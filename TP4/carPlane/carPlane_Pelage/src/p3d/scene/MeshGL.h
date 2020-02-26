#pragma once

#include <string>
#include "glsupport.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


namespace p3d {
class Mesh;

class MeshGL {

public:
  MeshGL(Mesh *mesh);
  virtual ~MeshGL();
  void draw();
  void initVAO();
  void drawBuffer();
  void drawNormal(double kLength=1.0);
  bool requestInitBuffer_=false;
  void drawLine(double scale);
private:
  GLuint allAttribBuffer_{0};
  GLuint vao_{0};
  Mesh *mesh_{nullptr};
  size_t nbVertex_{0};
};

}

