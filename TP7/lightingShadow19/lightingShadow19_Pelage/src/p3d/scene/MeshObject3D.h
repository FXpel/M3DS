#pragma once
#include "Mesh.h"
#include "MeshGL.h"
#include "Object3D.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

namespace p3d {

class MeshObject3D : public Mesh,public MeshGL, public Object3D {
public:
  virtual ~MeshObject3D();
  MeshObject3D();
};
}


