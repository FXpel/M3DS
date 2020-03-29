#pragma once

#include "glsupport.h"

/*!
*
* @file
*
* @brief FrameBuffer
* @author F. Aubert
*
*/

namespace p3d {
class Texture;

class FrameBuffer {
public:
  virtual ~FrameBuffer();
  FrameBuffer(const FrameBuffer &)=delete;
  FrameBuffer &operator=(const FrameBuffer &)=delete;

  FrameBuffer();

  int width() {return width_;}
  int height() {return height_;}

  void create(int w,int h);
  void attachDepth();
  void attachColor();
  void attachColor(const p3d::Texture &texture);
  void attachDepth(const p3d::Texture &texture);

  void begin();
  void end();

  void rtt(p3d::Texture *colorTexture,p3d::Texture *depthTexture=nullptr);

private:
  GLuint idFrame_,idRenderColor_,idRenderDepth_;
  int width_,height_;

  GLint previousFBO_; // TODO : stack for hierarchical Render To Texture (rtt that uses rtt)


};
} // namespace p3d

