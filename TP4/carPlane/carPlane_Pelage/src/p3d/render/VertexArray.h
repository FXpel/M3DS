#pragma once

#include "glsupport.h"

#include <map>
#include <vector>

namespace p3d {
class Vector4;
class Vector3;
class Vector2;

/**
@file
@author F. Aubert
@brief Encapsulates common buffer operations (class ArrayBuffer for VBO and class VertexArray for VAO)
*/

/** VERTEX ARRAY (ARRAY BUFFER is defined at the end) **/
class ArrayBuffer;
class VertexArray
{
  GLuint vaoId_{0}; // opengl VAO id
  std::vector<ArrayBuffer *> buffer_{}; // GL buffer for each attribute (no shared/interleaved buffer for simplicity)
  ArrayBuffer *element_{nullptr}; // for glDrawElements
  std::vector<GLuint> index_{}; // map each buffer to the index of the generic attribute for the shader (by default _index[i]=i)
  std::vector<bool> enable_{}; // enable the vertex attribute or not (useless)
  std::map<GLuint,GLuint> bufferIndex_{}; // to know which buffer is bind to the given shader index
  std::map<std::string,GLuint> bufferName_{}; // to know which buffer has the given name
  size_t nbVertex_{0}; // corresponds to number of vertex in VAO (to check errors)
  size_t nbElement_{0}; // corresponds to number of element to draw, i.e. count in glDrawArrays, glDrawElements
  GLenum mode_{0}; // GL_TRIANGLES, etc

  GLuint restart_{0}; // for primitive restart : here it is interpreted as a cycle (actually implemented with glMultiDraw* not with the opengl restart for compatibility reason)
  GLint *first_{nullptr}; // for primitive restart
  GLsizei *count_{nullptr}; // for primitive restart (actually implemented with glMultiDrawArrays,glMultDrawElements, *not* with restart index from openGL 4)
  size_t primCount_{0}; // for primitive restart

  void init();
  void add(size_t nbVertex, const std::string &name, GLuint index);
public:
  virtual ~VertexArray();

  void release();

  /// do not copy (no semantic due to opengl resources)
  VertexArray(const VertexArray &src)=delete;
  /// do not copy (no semantic due to opengl resources)
  VertexArray &operator=(const VertexArray &src)=delete;

  VertexArray(GLenum mode=GL_TRIANGLES);

  void mode(GLenum mode) {mode_=mode;}
  size_t nbVertex() {return nbVertex_;}
  void add(const std::string &name,const std::vector<Vector4> &data, GLuint index);
  void add(const std::string &name,const std::vector<Vector3> &data, GLuint index);
  void add(const std::string &name,const std::vector<Vector2> &data, GLuint index);
  void add(const std::string &name,const std::vector<float> &data, GLuint nbComponent,GLuint index);

  void update(const std::string &name,const std::vector<Vector4> &data);
  void update(const std::string &name,const std::vector<Vector3> &data);
  void update(const std::string &name,const std::vector<Vector2> &data);
  void update(const std::string &name,const std::vector<float> &data);


  void enable(GLuint index);
  void disable(GLuint index);
  void draw(size_t nbElement=0);
  void element(const std::vector<GLuint> &element);
  void use() {glBindVertexArray(vaoId_);}
  /// attention : if the VAO has elements, make sure that elements are set **before** restart (i.e. with VertexArray::element(indices) )
  void restart(size_t nbIndex);
};

/** ARRAY BUFFER **/
class ArrayBuffer {
public:
  GLenum target_{0}; // GL_ARRAY_BUFFER, etc
  GLuint id_{0}; // openGL name
  GLuint nbComponent_{0};  // corresponds to size of glVertexAttribPointer; must be 1,2,3 or 4
  GLsizeiptr size_{0}; // size in bytes
  GLenum type_{0}; // GL_FLOAT, etc

  virtual ~ArrayBuffer();
  ArrayBuffer(const ArrayBuffer &src)=delete;
  ArrayBuffer &operator=(const ArrayBuffer &src)=delete;
  ArrayBuffer(const std::vector<GLfloat> &data,GLenum target=GL_ARRAY_BUFFER,uint nbComponent=1);
  ArrayBuffer(const std::vector<GLuint> &data,GLenum target=GL_ELEMENT_ARRAY_BUFFER);
  ArrayBuffer(const std::vector<Vector4> &data, GLenum target);
  ArrayBuffer(const std::vector<Vector3> &data, GLenum target);
  ArrayBuffer(const std::vector<Vector2> &data, GLenum target);



  void erase();
  void set(const std::vector<float> &data, GLenum target, GLuint nbComponent=1);

  // TODO : templatize all these things
  void set(const std::vector<GLuint> &data,GLenum target);
  void set(const std::vector<Vector4> &data, GLenum target);
  void set(const std::vector<Vector3> &data, GLenum target);
  void set(const std::vector<Vector2> &data, GLenum target);

  void update(const std::vector<float> &data);
  void update(const std::vector<Vector4> &data);
  void update(const std::vector<Vector3> &data);
  void update(const std::vector<Vector2> &data);

};




} // namespace p3d


