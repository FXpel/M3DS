#include "MeshGL.h"
#include "GLDraw.h"
#include "GLRender.h"
#include "Tools.h"
#include "Mesh.h"

#include "glsupport.h"
#include "Vector2.h"


#include <iostream>

using namespace std;


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace p3d;
using namespace std;

MeshGL::~MeshGL() {
  // keep the if !!! (when called outside opengl context... or crashed).
  if (allAttribBuffer_) glDeleteBuffers(1,&allAttribBuffer_);
}

MeshGL::MeshGL(Mesh *mesh) {
  mesh_=mesh;
  allAttribBuffer_=0;
  vao_=0;
  nbVertex_=0;
}


void MeshGL::drawLine(double scale) {
  vector<Vector3> toDraw;
  for(size_t i=0;i<mesh_->nbFace();++i) {
    Vector3 g(0,0,0);
    for(size_t j=0;j<mesh_->nbVertexFace(i);++j) {
      g+=mesh_->positionVertexFace(i,int(j));
    }
    g/=mesh_->nbVertexFace(i);
    for(size_t j=0;j<mesh_->nbVertexFace(i);++j) {
      toDraw.push_back((mesh_->positionVertexFace(i,int(j))-g)*scale+g);
      toDraw.push_back((mesh_->positionVertexFace(i,int(j+1))-g)*scale+g);
    }
  }
  p3d::drawLines(toDraw);
}

void MeshGL::draw() {
  if (requestInitBuffer_) initVAO();
  drawBuffer();
}

void MeshGL::initVAO() {
  glDeleteVertexArrays(1,&vao_);
  glGenVertexArrays(1,&vao_);
  nbVertex_=0;
  for(unsigned int i=0;i<mesh_->nbFace();++i) {
    for(unsigned int j=0;j<mesh_->nbVertexFace(i);++j) {
      nbVertex_++;
    }
  }

  float *allAttrib;
  allAttrib=new float[nbVertex_*8]; // 3 position+3normal+2texCoord

  unsigned int iV=0;
  for(size_t i=0;i<mesh_->nbFace();++i) {
    for(int j=0;j<static_cast<int>(mesh_->nbVertexFace(i));++j) {
      allAttrib[iV*8+0]=static_cast<float>(mesh_->positionVertexFace(i,j).x());
      allAttrib[iV*8+1]=static_cast<float>(mesh_->positionVertexFace(i,j).y());
      allAttrib[iV*8+2]=static_cast<float>(mesh_->positionVertexFace(i,j).z());

      allAttrib[iV*8+3]=static_cast<float>(mesh_->normalVertexFace(i,j).x());
      allAttrib[iV*8+4]=static_cast<float>(mesh_->normalVertexFace(i,j).y());
      allAttrib[iV*8+5]=static_cast<float>(mesh_->normalVertexFace(i,j).z());

      allAttrib[iV*8+6]=static_cast<float>(mesh_->texCoordVertexFace(i,j).x());
      allAttrib[iV*8+7]=static_cast<float>(mesh_->texCoordVertexFace(i,j).y());
      ++iV;
    }
  }

  glDeleteBuffers(1,&allAttribBuffer_);
  glBindVertexArray(vao_);
  glEnableVertexAttribArray(0); // should be position in shader
  glEnableVertexAttribArray(1); // should be normal in shader
  glEnableVertexAttribArray(2); // should be texCoord in shader

  glGenBuffers(1,&allAttribBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,allAttribBuffer_);
  glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(8*nbVertex_*sizeof(float)),allAttrib,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),nullptr); // 0 = position
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<GLvoid *>(3*sizeof(float))); // 1 = normal
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<GLvoid *>(6*sizeof(float))); // 2 = texCoord

  glBindVertexArray(0);
  delete[] allAttrib;
  requestInitBuffer_=false;
}


void MeshGL::drawBuffer() {
  glBindVertexArray(vao_);

  glDrawArrays(GL_TRIANGLES,0,GLsizei(nbVertex_));

  glBindVertexArray(0);
}

void MeshGL::drawNormal(double kLength) {
    vector<Vector3> pts;
    for(size_t i=0;i<mesh_->nbFace();++i) {
        for(size_t j=0;j<mesh_->nbVertexFace(i);++j) {
            pts.push_back(mesh_->positionVertexFace(i,int(j)));
            pts.push_back(mesh_->positionVertexFace(i,int(j))+kLength*mesh_->normalVertexFace(i,int(j)));
        }
    }
    p3d::ambientColor=Vector4(1,0,0,1);
    p3d::shaderVertexAmbient();
    p3d::drawLines(pts);
}


