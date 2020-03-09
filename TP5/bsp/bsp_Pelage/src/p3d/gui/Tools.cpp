#include "Tools.h"

#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QImage>

#include <ctime>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


#ifdef _WIN32
#define SEARCH_CHAR "\\"
#else
#define SEARCH_CHAR "/"
#endif

using namespace std;

using namespace p3d;

static QDir mediaPath_(QString(MEDIA_PATH));


static bool firstElapsed_;
static long int lastStart_;
static long int lastElapsed_;

static QString figureName_="";
static int figureStep_=-1;

Error::Error(string mesg, int line,string fichier) {
  std::ostringstream oss;

  string base;
  size_t index;


  index=fichier.rfind(SEARCH_CHAR);
  index=fichier.rfind(SEARCH_CHAR,index-1);
  base=fichier.substr(index+1,fichier.length()-index-1);


  oss << "Error in "<< base << " line " << line << " : " << mesg;
  this->message_ = oss.str();
}

const char * Error::what() const noexcept {
     return this->message_.c_str();
}


void Error::show() {
    cout << message_ << endl;
}


/** UTILS **/
std::string p3d::errorToString(GLenum error) {
  switch(error) {
    case GL_INVALID_ENUM:return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:return "GL_OUT_OF_MEMORY";
    case GL_STACK_UNDERFLOW:return "GL_STACK_UNDERFLOW";
    case GL_STACK_OVERFLOW:return "GL_STACK_OVERFLOW";
    default:return "UNKNOWN ERROR";
  }
}


void p3d::checkError(const std::string &mesg,int line,const std::string &file) {

  std::cout << "==============================" << std::endl;
  GLenum err=glGetError();
  if (err!=GL_NO_ERROR) {
    std::cout << "GL error : " << errorToString(err) << " from " << file << " at line " << line << " ("+mesg+")" << std::endl;
    throw ErrorD("GL error");
  } else {
    std::cout << "no GL error : " << " from " << file << " at line " << line << " ("+mesg+")" << std::endl;
    std::cout << "==============================" << std::endl;
    }
}


std::string p3d::mediaPath() {
  return mediaPath_.absolutePath().toStdString();
}

void p3d::checkFile(const QFileInfo &file) {
  if (!file.exists())
    throw ErrorD("cant locate file "+file.absoluteFilePath().toStdString()+" (media path = "+mediaPath_.absolutePath().toStdString()+")");
}

QFileInfo p3d::resourceFile(const std::string &filename) {
  QFileInfo res;
  res.setFile(mediaPath_,QString::fromStdString(filename));
  checkFile(res);
  return res;
}

void p3d::mediaPath(const std::string &dir) {
  mediaPath_.setPath(QString::fromStdString(dir));
}

void mediaDirectoryRelativeFile(const std::string &filename,const std::string &dir) {
  mediaPath_.setPath(QFileInfo(QString::fromStdString(filename)).absolutePath()+"/"+QString::fromStdString(dir));
}

void p3d::figureName(const string &filename) {
  figureName_=QString::fromStdString(filename);
}

void p3d::figureStep(int step) {
  figureStep_=step;
}

void p3d::captureImage(int x,int y,int w,int h) {
  QString name;
  QString fullname;
  QFileInfo file;
  int index=figureStep_;
  if (index==-1) {
    name=QCoreApplication::applicationName();
    name+="_%1.png";
    do {
      index++;
      fullname=name.arg(index,4,10,QChar('0'));
      file.setFile(mediaPath_,"snapshot/"+fullname);
    }
    while(file.exists());
  }
  else {
    name=figureName_;
    name+="%1.png";
    fullname=name.arg(index,4,10,QChar('0'));
    file.setFile(mediaPath_,"snapshot/"+fullname);
  }
  unsigned char *pixels=new unsigned char[size_t(w)*size_t(h)*4];
  glReadPixels(x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,pixels);
  QImage img=QImage(pixels,w,h,QImage::Format_ARGB32);
  img=img.mirrored(false,true);
  bool ok=img.save(file.absoluteFilePath(),"PNG");
  if (!ok) throw ErrorD("cant save snapshot :"+file.absoluteFilePath().toStdString());
  cout << file.absoluteFilePath().toStdString() << " saved" << endl;
  delete[] pixels;
}

std::string p3d::readTextFile(const std::string &filename) {
  QFileInfo finfo=resourceFile(filename);
  QFile f(finfo.absoluteFilePath());
  f.open(QIODevice::ReadOnly);
  QTextStream t(&f);
  QString s=t.readAll();
  return s.toStdString();
}


void p3d::startTime() {lastStart_=clock();}
double p3d::elapsedStartTime() {return double(clock()-lastStart_)/CLOCKS_PER_SEC;}
double p3d::elapsedTime() {
  double res;
  time_t t=clock();
  if (firstElapsed_) {
    lastElapsed_=t;
    firstElapsed_=false;
  }
  res=t-lastElapsed_;
  lastElapsed_=t;
  return res/double(CLOCKS_PER_SEC);
}




