#include "Texture.h"
#include <iostream>
#include <cstdlib>
#include "Tools.h"
#include "GLRender.h"
#include <QDebug>
#include <QPainter>


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


Texture::~Texture() {
  delete img_;
  release();
}

void Texture::release() {
  if (isInit_) {
    glDeleteTextures(1,&texId_);
    isInit_=false;
  }
}


Texture::Texture() {
  // should not acquire openGL resources in constructor : call init() once context is initialized
}

void Texture::filename(const std::string &s) {
    filename_=s;
}

void Texture::read() {
    read(filename_);
}


void Texture::init() {
  glGenTextures(1,&texId_);
  isInit_=true;
}


void Texture::filter(GLint filter) {
  filterMin(filter);
  filterMag(filter);
}

void Texture::filterMin(GLint filter) {
  if (filter!=0) filterMin_=filter;
  bind();
  glTexParameterf(target_,GL_TEXTURE_MIN_FILTER,filterMin_);
}

void Texture::filterMag(GLint filter) {
  if (filter!=0) filterMag_=filter;
  bind();
  glTexParameterf(target_,GL_TEXTURE_MAG_FILTER,filterMag_);
}

void Texture::wrap(GLenum wrap) {
  bind();
  wrap_=wrap;
  glTexParameterf(target_,GL_TEXTURE_WRAP_S,wrap_);
  glTexParameterf(target_,GL_TEXTURE_WRAP_T,wrap_);
  glTexParameterf(target_,GL_TEXTURE_WRAP_R,wrap_);
}


void Texture::check() {
  cout << "TARGET = ";
  switch(target_) {
    case GL_TEXTURE_1D:cout << "GL_TEXTURE_1D";break;
    case GL_TEXTURE_2D:cout << "GL_TEXTURE_2D";break;
    case GL_TEXTURE_3D:cout << "GL_TEXTURE_3D";break;
    default:cout << "error";
  }
  cout << endl;

  cout << "TEX ID = " << texId_ << endl;
  cout << "INTERNAL FORMAT = ";
  switch(internalFormat_) {
    case GL_RED:cout << "GL_RED";break;
    case GL_RGBA:cout << "GL_RGBA";break;
    case GL_DEPTH_COMPONENT:cout << "GL_DEPTH_COMPONENT";break;
    default:cout << "error";
  }
  cout << endl;

  cout << "WIDTH =" << width_ << " HEIGHT = " << height_ << " DEPTH = " << depth_ << endl;

  cout << "DATA FORMAT = ";
  switch(dataFormat_) {
    case GL_RED:cout << "GL_RED";break;
    case GL_BGRA:cout << "GL_BGRA";break;
    case GL_RGBA:cout << "GL_RGBA";break;
    case GL_DEPTH_COMPONENT:cout << "GL_DEPTH_COMPONENT";break;
    default:cout << "error";
  }
  cout << endl;

  cout << "DATA TYPE = ";
  switch(dataType_) {
    case GL_FLOAT:cout << "GL_FLOAT";break;
    case GL_UNSIGNED_BYTE:cout << "GL_UNSIGNED_BYTE";break;
    default:cout << "unknown";
  }
  cout << endl;

  cout << "MIN FILTER = ";
  switch(filterMin_) {
  case GL_NEAREST:cout << "GL_NEAREST";break;
  case GL_LINEAR:cout << "GL_LINEAR";break;
  case GL_LINEAR_MIPMAP_LINEAR:cout << "GL_LINEAR_MIPMAP_LINEAR";break;
  default:cout << "error";
  }
  cout << endl;

  cout << "MAG FILTER = ";
  switch(filterMag_) {
  case GL_NEAREST:cout << "GL_NEAREST";break;
  case GL_LINEAR:cout << "GL_LINEAR";break;
  case GL_LINEAR_MIPMAP_LINEAR:cout << "GL_LINEAR_MIPMAP_LINEAR";break;
  default:cout << "error";
  }
  cout << endl;

  cout << "WRAP = ";
  switch(wrap_) {
  case GL_REPEAT:cout << "GL_REPEAT";break;
  case GL_CLAMP:cout << "GL_CLAMP";break;
  default:cout << "error";
  }
  cout << endl;
  checkGLErrorD("check");

}

void Texture::toGL(const GLvoid *data,int level) {
  if (texId_==0) throw ErrorD("Texture::init should be called before Texture::toGL");
  bind();
  switch(target_) {
    case GL_TEXTURE_1D:glTexImage1D(target_,level,internalFormat_,width_,0,dataFormat_,dataType_,data);
      break;
    case GL_TEXTURE_2D:glTexImage2D(target_,level,internalFormat_,width_,height_,0,dataFormat_,dataType_,data);
      break;
    case GL_TEXTURE_3D:glTexImage3D(target_,level,internalFormat_,width_,height_,depth_,0,dataFormat_,dataType_,data);
      break;
    default:throw ErrorD("_target undefined");
  }
}

void Texture::toGL(GLenum dataFormat,GLenum dataType,const GLvoid *data,int level) {
  dataFormat_=dataFormat;
  dataType_=dataType;
  toGL(data,level);
}


void Texture::init(const QImage &img) {
  if (!img_) img_=new QImage(img.width(),img.height(),QImage::Format_ARGB32);
  (*img_)=img.convertToFormat(QImage::Format_ARGB32);

  if (img_->isNull()) {
    throw ErrorD("QImage error in init(img)");
  }
  dataType_=GL_UNSIGNED_BYTE;
  dataFormat_=GL_BGRA;
  init(img_->width(),img_->height(),0,img_->mirrored().bits());
}


void Texture::toSubGL(const QImage &img) {
  dataType_=GL_UNSIGNED_BYTE;
  dataFormat_=GL_BGRA;
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,img.width(),img.height(),dataFormat_, GL_UNSIGNED_BYTE, img.convertToFormat(QImage::Format_ARGB32).mirrored().bits());
}


void Texture::init(int w,int h,int d,GLint internalFormat) {
  if (!isInit_) {
    init();
  }
  if (internalFormat!=0) internalFormat_=internalFormat;
  width_=w;height_=h;depth_=d;
  if (filterMin_==0) filterMin_=GL_NEAREST;
  if (filterMag_==0) filterMag_=GL_NEAREST;
  if (wrap_==0) wrap_=GL_REPEAT;
  if (depth_) target_=GL_TEXTURE_3D;
  else if (height_) target_=GL_TEXTURE_2D;
  else target_=GL_TEXTURE_1D;
  wrap();
  filter();
  ////////////////////////////////////////////////
}

void Texture::init(int w,int h,int d,const GLvoid *data,GLenum dataFormat,GLenum dataType,GLint internalFormat) {
  init(w,h,d,internalFormat);
  if (dataFormat!=0) dataFormat_=dataFormat;
  if (dataType!=0) dataType_=dataType;
  toGL(data);
  if (generateMipMap_) generateMipmap();
}

void Texture::set(const QImage &img) {
  init(img);
}

void Texture::init(int w,int h,int d,const vector<float> &data,GLenum dataFormat,GLint internalFormat) {
  // TODO : check coherence
  const GLvoid *ddata=nullptr;
  if (!data.empty()) ddata=data.data();
  init(w,h,d,ddata,dataFormat,GL_FLOAT,internalFormat);
}

void Texture::set(const std::vector<float> &data,int level) {
  // TODO : check coherence
  if (texId_==0) throw ErrorD("texture should be initialized before set direct data : use set(w,h,d,format) or set(QImage) or read(filename) before");
//  _dataFormat=format;
  dataType_=GL_FLOAT;
  toGL(data.data(),level);
}


void Texture::bind(unsigned int unit) {
  GLenum unitTarget=GL_TEXTURE0+unit;
  glActiveTexture(unitTarget);
  glBindTexture(target_,texId_);
}

void Texture::read(const string &resourceName) {
  QFileInfo resource=p3d::resourceFile(resourceName);
  if (!img_) {
    img_=new QImage();
  }

  QImage load;
  load.load(resource.filePath());
  if (load.isNull()) throw ErrorD("cant load the format image : " +resource.absoluteFilePath().toStdString());
  set(load);
}

void Texture::readMipmap(const string &resourceName) {
  read(resourceName);
  generateMipmap();
}

void Texture::read(const string &resourceName,int level) {
  QFileInfo resource=p3d::resourceFile(resourceName);
  if (texId_==0) {
    throw Error("read with mipmap level only with an already created mipmap texture",__LINE__,__FILE__);
  }
  QImage img;
  img.load(resource.filePath());

  if (img.isNull()) {
    throw ErrorD("problem with QImage in read(\""+resource.absoluteFilePath().toStdString()+"\")");
  }
  dataType_=GL_UNSIGNED_BYTE;
  dataFormat_=GL_BGRA;
  // TODO : check coherence with requested level
  //init(_img->width(),_img->height(),0,_img->bits(),GL_RGBA);
  toGL(img_->mirrored().bits(),level);
}


void Texture::mipmap(bool v) {
  generateMipMap_=v;
}

void Texture::generateMipmap() {
  glGenerateMipmap(GL_TEXTURE_2D); // TODO : check target and see if data=0 is not a problem
  filterMipmap();
}

void Texture::parameter(unsigned int para,int mode) {
  glBindTexture(target_,texId_);
  glTexParameteri(target_,para,mode);
}

void Texture::transparency(bool active) {
  if (active) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  }
  else
    glDisable(GL_BLEND);
}

void Texture::setAlpha(const Vector4 &c, double v, double radius) {
  if (img_) {
    for(int i=0;i<img_->width();i++) {
      for(int j=0;j<img_->height();j++) {
        QColor cq=img_->pixel(i,j);
        double dist=Vector3(cq.redF()-c.r(),cq.greenF()-c.g(),cq.blueF()-c.b()).length();
        if (dist<radius)
          cq.setAlphaF(v);//1.0-(radius-dist)/radius);
        img_->setPixel(i,j,cq.rgba());
      }
    }
    init(*img_);
  }
}

void Texture::setAlpha(double v) {
  if (img_) {
    for(int i=0;i<img_->width();i++) {
      for(int j=0;j<img_->height();j++) {
        QColor cq=QColor::fromRgba(img_->pixel(i,j));
        cq.setAlphaF(v);
        img_->setPixel(i,j,cq.rgba());
      }
    }
    init(*img_);
  }
}

void Texture::makeChecker(int w, int h, int nbLine, int nbColumn, const Vector4 &c1, const Vector4 &c2) {
  double stepLine=double(h)/nbLine;
  double stepColumn=double(w)/nbColumn;
  double x=0;
  double y=0;
  bool squareC1=false;
  QBrush b1=QBrush(QColor::fromRgbF(c1.r(),c1.g(),c1.b(),c1.a()),Qt::SolidPattern);
  QBrush b2=QBrush(QColor::fromRgbF(c2.r(),c2.g(),c2.b(),c2.a()),Qt::SolidPattern);


  QPainter p;
  QImage img(w,h,QImage::Format_ARGB32);
  img.fill(Qt::red);
  p.begin(&img);

  for(int i=0;i<nbColumn;++i) {
    squareC1=(i%2==0);
    y=0;
    for(int j=0;j<nbLine;++j) {
      if (squareC1) {
        p.setBrush(b1);
      }
      else {
        p.setBrush(b2);
      }
      p.setPen(p.brush().color());
      p.drawRect(int(x),int(y),int(stepColumn),int(stepLine));
      squareC1=!squareC1;
      y+=stepLine;
    }
    x+=stepColumn;
  }

  p.end();
  set(img);
}


