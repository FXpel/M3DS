#include "FrameBuffer.h"
#include "glsupport.h"

#include "Texture.h"

/*!
*
* @file
*
* @brief FrameBuffer
* @author F. Aubert
*
*/

using namespace std;
using namespace p3d;


FrameBuffer::~FrameBuffer() {
  glDeleteRenderbuffers(1,&idRenderColor_);
  glDeleteRenderbuffers(1,&idRenderDepth_);
  glDeleteFramebuffers(1,&idFrame_);
}

FrameBuffer::FrameBuffer()
{
  idFrame_=0;
  idRenderColor_=0;
  idRenderDepth_=0;
  width_=0;
  height_=0;
}


void FrameBuffer::attachDepth() {
  // attach a depth buffer
  if (!idRenderDepth_) glGenRenderbuffers(1,&idRenderDepth_);
  glBindRenderbuffer(GL_RENDERBUFFER,idRenderDepth_);
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width_,height_);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,idRenderDepth_);
}

void FrameBuffer::attachColor() {
  if (!idRenderColor_) glGenRenderbuffers(1,&idRenderColor_);
  glBindRenderbuffer(GL_RENDERBUFFER,idRenderColor_);
  glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,width_,height_);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,idRenderColor_);
  // TODO : bind glReadPixel to the current buffer :
  //glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,_idRender);
}

void FrameBuffer::attachColor(const p3d::Texture &texture) {
  // TODO : check texture format to be compatible with color attachment
  // TODO : check texture width and height
  // 0 = level
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture.id(),0);
}

void FrameBuffer::attachDepth(const p3d::Texture &texture) {
  // TODO : check texture format to be compatible with depth attachment
  // TODO : check texture width and height
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,texture.id(),0);
}



void FrameBuffer::create(int w,int h) {
  width_=w;height_=h;
  // generate fbo
  glGenFramebuffers(1,&idFrame_);
  glBindFramebuffer(GL_FRAMEBUFFER,idFrame_);


  glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void FrameBuffer::begin() {
  // saves previous Frame Buffer
  glGetIntegerv(GL_FRAMEBUFFER_BINDING,&previousFBO_);

  glBindFramebuffer(GL_FRAMEBUFFER,idFrame_);
}

void FrameBuffer::end() {
  // restores previous Frame Buffer
  glBindFramebuffer(GL_FRAMEBUFFER,GLuint(previousFBO_));
}


void FrameBuffer::rtt(Texture *colorTexture,Texture *depthTexture) {
  glBindFramebuffer(GL_FRAMEBUFFER,idFrame_);
  if (colorTexture) {
    colorTexture->init(width_,height_,0,GL_RGBA);
    this->attachColor(*colorTexture);
  }
  else this->attachColor();
  if (depthTexture) {
    depthTexture->init(width_,height_,0,GL_DEPTH_COMPONENT);
    this->attachDepth(*depthTexture);

  }
  else this->attachDepth();
  glBindFramebuffer(GL_FRAMEBUFFER,0);
}


