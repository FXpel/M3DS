#ifndef BASICMESH_H
#define BASICMESH_H

#include "glsupport.h"

#include <vector>

namespace p3d {
class Mesh;
}

class BasicMesh
{
public:
  virtual ~BasicMesh();
  BasicMesh();


  void initTetrahedron();
  void initObj(const p3d::Mesh &mesh);

  void initDraw();
  void draw();

private:
  void initBuffer();
  void updateBuffer();
  void initVAO();

  std::vector<float> attribute_{};
  std::vector<unsigned int> element_{};

  GLuint vao_{0};
  GLuint attributeBuffer_{0};
  GLuint elementBuffer_{0};
};

#endif // BASICMESH_H

