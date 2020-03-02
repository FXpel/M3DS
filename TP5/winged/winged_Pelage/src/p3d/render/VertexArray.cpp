#include "VertexArray.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Tools.h"

#include <string>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace std;
using namespace p3d;



/** VERTEX ATTRIB **/
VertexArray::~VertexArray() {
  release();
}


VertexArray::VertexArray(GLenum mode) {
  mode_=mode;
  init();
}

void VertexArray::release() {
  for(auto b:buffer_) {
    delete b;
  }
  delete element_;
  delete[] first_;
  delete[] count_;
  glDeleteVertexArrays(1,&vaoId_);
  init();
}

void VertexArray::init() {
  nbElement_=0;
  nbVertex_=0;
  buffer_.clear();index_.clear();enable_.clear();
  bufferIndex_.clear();
  bufferName_.clear();
  element_=nullptr;
  restart_=0;
  first_=nullptr;count_=nullptr;
  vaoId_=0;
}


void VertexArray::add(size_t nbVertex,const std::string &name,GLuint index) {
  if (vaoId_==0) {
    glGenVertexArrays(1,&vaoId_);
  }
  if (index==0) {
    index=GLuint(buffer_.size()-1);
  }
  if (nbVertex_==0) {
    nbVertex_=nbVertex;
  }
  else if (nbVertex_!=nbVertex) throw ErrorD("inconsistent nbVertex");
  if (nbElement_==0) {
    nbElement_=nbVertex;
  }
  if (bufferIndex_.find(index)!=bufferIndex_.end()) {
    throw ErrorD("attribute index is already mapped");
  }
  if (bufferName_.find(name)!=bufferName_.end()) {
    throw ErrorD(string("the name \"")+name+"\" already exists in VertexAttrib (use update ?)");
  }
  bufferName_[name]=GLuint(buffer_.size()-1);
  bufferIndex_[index]=GLuint(buffer_.size()-1);
  index_.push_back(index);
  enable_.push_back(true);

  glBindVertexArray(vaoId_);
  size_t i=buffer_.size()-1;
  glEnableVertexAttribArray(index_[i]);
  glBindBuffer(GL_ARRAY_BUFFER,buffer_[i]->id_);
  glVertexAttribPointer(index_[i],GLint(buffer_[i]->nbComponent_),buffer_[i]->type_,GL_FALSE,0,nullptr);
  glBindVertexArray(0);
}

void VertexArray::add(const std::string &name,const std::vector<Vector2> &data,GLuint index) {
  buffer_.push_back(new ArrayBuffer(data,GL_ARRAY_BUFFER));
  add(data.size(),name,index);
}

void VertexArray::add(const std::string &name,const std::vector<Vector3> &data,GLuint index) {
  buffer_.push_back(new ArrayBuffer(data,GL_ARRAY_BUFFER));
  add(data.size(),name,index);
}

void VertexArray::add(const std::string &name,const std::vector<Vector4> &data,GLuint index) {
  buffer_.push_back(new ArrayBuffer(data,GL_ARRAY_BUFFER));
  add(data.size(),name,index);
}


void VertexArray::add(const std::string &name,const std::vector<float> &data,GLuint nbComponent,GLuint index) {
  buffer_.push_back(new ArrayBuffer(data,GL_ARRAY_BUFFER,nbComponent));
  add(data.size()/nbComponent,name,index);
}

void VertexArray::update(const std::string &name,const std::vector<Vector2> &data) {
  if (bufferName_.find(name)==bufferName_.end()) {
    throw ErrorD(string("cant update the buffer \"")+name+"\" : not in VertexAttrib (use add before ?)");
  }
  ArrayBuffer *b=buffer_[bufferName_[name]];
  b->update(data);
}

void VertexArray::update(const std::string &name,const std::vector<Vector3> &data) {
  if (bufferName_.find(name)==bufferName_.end()) {
    throw ErrorD(string("cant update the buffer \"")+name+"\" : not in VertexAttrib (use add before ?)");
  }
  ArrayBuffer *b=buffer_[bufferName_[name]];
  b->update(data);
}

void VertexArray::update(const std::string &name,const std::vector<Vector4> &data) {
  if (bufferName_.find(name)==bufferName_.end()) {
    throw ErrorD(string("cant update the buffer \"")+name+"\" : not in VertexAttrib (use add before ?)");
  }
  ArrayBuffer *b=buffer_[bufferName_[name]];
  b->update(data);
}


void VertexArray::update(const std::string &name,const std::vector<float> &data) {
  if (bufferName_.find(name)==bufferName_.end()) {
    throw ErrorD(string("cant update the buffer \"")+name+"\" : not in VertexAttrib (use add before ?)");
  }
  ArrayBuffer *b=buffer_[bufferName_[name]];
  b->update(data);
}


void VertexArray::element(const std::vector<GLuint> &element) {
  if (vaoId_==0) {
    glGenVertexArrays(1,&vaoId_);
  }
  element_=new ArrayBuffer(element,GL_ELEMENT_ARRAY_BUFFER);
  nbElement_=element.size();
  glBindVertexArray(vaoId_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,element_->id_);
  glBindVertexArray(0);
}

void VertexArray::disable(GLuint index) {
  enable_[bufferIndex_[index]]=false;
}

void VertexArray::enable(GLuint index) {
  enable_[bufferIndex_[index]]=true;
}

void VertexArray::restart(size_t nbElem) {
  if (nbElement_==0) {
    throw ErrorD("call restart **after** an attribute or element is set");
  }
  restart_=GLuint(nbElem);
  intptr_t offset; // offset for restart (different for Arrays (=vertex count) and Elements (=byte offset)
  primCount_=nbElement_/nbElem;
  if (element_) {
    offset=restart_*sizeof(GLuint);
  }
  else {
    offset=restart_;
  }
  delete[] first_;
  delete[] count_;
  first_=new GLint[primCount_];
  count_=new GLsizei[primCount_];
  first_[0]=0;
  count_[0]=restart_;

  for(size_t i=1;i<primCount_;i++) {
    first_[i]=first_[i-1]+offset;
    count_[i]=count_[i-1];
  }
}

/// use parameter with care ! (perticularly when restart is set)
void VertexArray::draw(size_t nbElement) {
  if (nbElement!=0) {
    // if (_restart && nbElement!=_nbElement) throw ErrorD("cant draw nbVertex with a restart set (all elements should be drawn)");
  }
  else nbElement=nbElement_;
  glBindVertexArray(vaoId_);
  if (!element_) { // draw with arrays
    if (restart_==0) {
      glDrawArrays(mode_,0,GLsizei(nbElement));
    }
    else {
      // TODO : cant use multi with same type for glMultiDrawElements and glMultiDrawArrays (64 bits platform : GLint * not compatible with GLvoid *; use intptr_t)
      for(size_t i=0;i<primCount_;i++) {
        glDrawArrays(mode_,first_[i],count_[i]);
      }
//      glMultiDrawArrays(_mode,(GLint *)_first,_count,_primCount);
    }
  }
  else {// draw with elements
    if (restart_==0) {
      glDrawElements(mode_,GLsizei(nbElement),GL_UNSIGNED_INT,nullptr);
    }
    else {
      // TODO : cant use multi with same type for Elements and Array (64 bits : GLuint/GLint not compatible with GLvoid *)

      for(size_t i=0;i<primCount_;i++) {
        glDrawElements(mode_,count_[i],GL_UNSIGNED_INT,reinterpret_cast<GLvoid*>(first_[i]));
      }

 //     glMultiDrawElements(_mode,_count,GL_UNSIGNED_INT,(const GLvoid **)(_first),_primCount);
    }
  }
  glBindVertexArray(0);
}


/** ARRAY BUFFER **/
ArrayBuffer::~ArrayBuffer() {
  if (id_!=0) glDeleteBuffers(1,&id_); // ArrayBuffer must not share gl buffer !
}


ArrayBuffer::ArrayBuffer(const std::vector<float> &data, GLenum target, GLuint nbComponent) {
  id_=0;
  set(data,target,nbComponent);
}

ArrayBuffer::ArrayBuffer(const std::vector<Vector3> &data, GLenum target) {
  id_=0;
  set(data,target);
}

ArrayBuffer::ArrayBuffer(const std::vector<Vector4> &data, GLenum target) {
  id_=0;
  set(data,target);
}

ArrayBuffer::ArrayBuffer(const std::vector<Vector2> &data, GLenum target) {
  id_=0;
  set(data,target);
}

ArrayBuffer::ArrayBuffer(const std::vector<GLuint> &data, GLenum target) {
  id_=0;
  set(data,target);
}

void ArrayBuffer::update(const std::vector<float> &data) {
  glBindBuffer(target_,id_);
  glBufferData(target_,size_,data.data(),GL_STATIC_DRAW);
}


void ArrayBuffer::update(const std::vector<Vector2> &data) {
  glBindBuffer(target_,id_);
  float *buffer=reinterpret_cast<float*>(glMapBuffer(target_,GL_WRITE_ONLY));
  for(auto &v:data) {
    *(buffer)=v.xf();
    *(++buffer)=v.yf();
    ++buffer;
  }
  glUnmapBuffer(target_);
}

void ArrayBuffer::update(const std::vector<Vector3> &data) {
  glBindBuffer(target_,id_);
  float *buffer=reinterpret_cast<float*>(glMapBuffer(target_,GL_WRITE_ONLY));
  for(auto &v:data) {
    *(buffer)=v.xf();
    *(++buffer)=v.yf();
    *(++buffer)=v.zf();
    ++buffer;
   }
  glUnmapBuffer(target_);
}

void ArrayBuffer::update(const std::vector<Vector4> &data) {
  glBindBuffer(target_,id_);
  float *buffer=reinterpret_cast<float *>(glMapBuffer(target_,GL_WRITE_ONLY));
  for(auto &v:data) {
    *(buffer)=v.xf();
    *(++buffer)=v.yf();
    *(++buffer)=v.zf();
    *(++buffer)=v.wf();
    ++buffer;
   }
  glUnmapBuffer(target_);
}


void ArrayBuffer::set(const std::vector<float> &data, GLenum target, GLuint nbComponent) {
  nbComponent_=nbComponent;
  target_=target;
  size_=GLsizeiptr(sizeof(float)*data.size());
  type_=GL_FLOAT;
  if (id_==0) glGenBuffers(1,&id_);
  glBindBuffer(target_,id_);
  update(data);
}

void ArrayBuffer::set(const std::vector<Vector4> &data,GLenum target) {
  nbComponent_=4;
  target_=target;
  size_=GLsizeiptr(sizeof(GLfloat)*4*data.size());
  type_=GL_FLOAT;
  if (id_==0)
    glGenBuffers(1,&id_);
  glBindBuffer(target_,id_);
  glBufferData(target_,size_,nullptr,GL_STATIC_DRAW);
  update(data);
}

void ArrayBuffer::set(const std::vector<Vector3> &data,GLenum target) {
  nbComponent_=3;
  target_=target;
  size_=GLsizeiptr(sizeof(GLfloat)*3*data.size());
  type_=GL_FLOAT;
  if (id_==0)
    glGenBuffers(1,&id_);
  glBindBuffer(target_,id_);
  glBufferData(target_,size_,nullptr,GL_STATIC_DRAW);
  update(data);
}




void ArrayBuffer::set(const std::vector<Vector2> &data,GLenum target) {
  nbComponent_=2;
  target_=target;
  size_=GLsizeiptr(sizeof(GLfloat)*2*data.size());
  type_=GL_FLOAT;
  if (id_==0) glGenBuffers(1,&id_);
  glBindBuffer(target_,id_);
  glBufferData(target_,size_,nullptr,GL_STATIC_DRAW);
  update(data);
}


void ArrayBuffer::set(const std::vector<GLuint> &data,GLenum target) {
  nbComponent_=1;
  target_=target;
  size_=GLsizeiptr(sizeof(int)*data.size());
  type_=GL_UNSIGNED_INT;
  if (id_==0) glGenBuffers(1,&id_);
  glBindBuffer(target_,id_);
  glBufferData(target_,size_,data.data(),GL_STATIC_DRAW);
}


void ArrayBuffer::erase() {
  glDeleteBuffers(1,&id_);
  id_=0;
}



