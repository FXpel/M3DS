#pragma once

#include "glsupport.h"
#include <string>
#include "Vector2.h"
#include "Matrix4.h"
#include <QImage>

/**
@file
@author F. Aubert
@brief Encapsulates common texture operations
*/


namespace p3d {
/**
@class Texture
@brief Encapsulates common texture operations
*/
class Texture {

public:
  ~Texture();
  void release();

  /// !caution : do not clone texture (clone _img (RAM) or/and _texId (GL) ? + require resource manager for GL memory)
  Texture(const Texture &)=delete;
  /// !caution : do not clone texture (clone _img (RAM) or/and _texId (GL) ? + require resource manager for GL memory)
  Texture &operator=(const Texture &)=delete;
  /// ctor
  Texture();
  void check();
  /// sets a filename to texture (useful to load later when opengl context is not valid)
  void filename(const std::string &n);
  /// reads from filename
  void read();
  /// set the texture with img (img is saved in _img property)
  void set(const QImage &img);
  /// set data of the texture !caution : data should be coherent with format, w, h,...
  void init(int w, int h, int d, const std::vector<float> &data,GLenum dataFormat=GL_BGRA,GLint internalFormat=GL_RGBA);
  void init(int w, int h, const std::vector<float> &data,GLenum dataFormat=GL_BGRA) {init(w,h,0,data,dataFormat);}
  void init(int w, const std::vector<float> &data=std::vector<float>(0),GLenum dataFormat=GL_BGRA) {init(w,0,0,data,dataFormat);}
  /// empty texture of size wxhxd (d=0 => texture 2D)
  void init(int w, int h, int d, GLint internalFormat=GL_RGBA);
  /// set direct data to the texture
  void set(const std::vector<float> &data, int level=0);

  /// read and build mip map texture
  void readMipmap(const std::string &filename);
	/// read image file
	void read(const std::string &filename);
	/// read image file for the given mip map level
	void read(const std::string &filename,int level);

	/// get width
  int width() const {return width_;}
	/// get height
  int height() const {return height_;}
  /// get depth (i.e. size of the texture; is not relative to "depth map")
  int depth() const {return depth_;}

  /// this is the current opengl texture for the current texture unit
  void bind(unsigned int unit=0) const;
	/// get the texture id
  inline GLuint id() const {return texId_;}
  /// set GL texture parameter
  void parameter(unsigned int para, int mode);

  void filter(GLint filter=0);
  void filterMin(GLint filter=0);
  void filterMag(GLint filter=0);
  void filterLinear() {filter(GL_LINEAR);}
  void filterMipmap() {filterMin(GL_LINEAR_MIPMAP_LINEAR);filterMag(GL_LINEAR);}
  void filterNearest() {filter(GL_NEAREST);}
  /// set wrap (wrap = GL_CLAMP, GL_REPEAT, etc)
  void wrap(GLenum wrap=GL_REPEAT);
  void wrapRepeat() {wrap(GL_REPEAT);}
  void wrapClamp() {wrap(GL_CLAMP);}
  void transparency(bool active);
  void setLuminance(bool ok);
  void setAlpha(const p3d::Vector4 &c, double v,double radius);
  void setAlpha(double v);
  void generateMipmap();

  void makeChecker(int w, int h, int nbLine, int nbColumn, const Vector4 &c1, const Vector4 &c2);
  void mipmap(bool v);
  void toSubGL(const QImage &img);
  void toGL(GLenum dataFormat, GLenum dataType, const GLvoid *data, int level);


private:

  /// init from QImage
  void init(const QImage &img);
  /// init data of the GL texture with size wxhxd (d=0 => texture 2D) (!TODO : may be private...)
  void init(int w, int h, int d, const GLvoid *data, GLenum dataFormat=0, GLenum dataType=0,GLint internalFormat=0);
  /// set GL texels with data. The QImage _img is not modified (!! caution : coherence of data with format, width, height, etc is not checked !)
  void toGL(const GLvoid *data,int level=0);

  void init();

  GLsizei width_{0},height_{0},depth_{0};
  GLuint texId_{0};
  GLint filterMin_{GL_NEAREST},filterMag_{GL_NEAREST};
  GLenum target_{0};
  GLenum internalFormat_{GL_RGBA};
  GLenum dataFormat_{0};
  GLenum dataType_{GL_UNSIGNED_BYTE};
  GLenum wrap_{GL_REPEAT}; /// same wrap in 3 directions
  bool generateMipMap_{false};
  QImage *img_{nullptr};
  std::string filename_{""};
  bool isInit_{false};

};


}


