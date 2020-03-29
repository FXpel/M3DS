#include "Shader.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Tools.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

using namespace p3d;
using namespace std;

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

Shader *Shader::current_=nullptr;

Shader::~Shader() {
  release();
}

void Shader::release() {
  if (isInit_) {
    // openGL context should be active
    glDeleteShader(vertexId_);
    glDeleteShader(fragmentId_);
    glDeleteProgram(programId_);
    isInit_=false;
  }
}


Shader::Shader() {
  // should not acquire openGL resources in constructor : call init() once context is initialized
}

void Shader::init() {

  programId_=glCreateProgram();
  fragmentId_=glCreateShader(GL_FRAGMENT_SHADER);
  vertexId_=glCreateShader(GL_VERTEX_SHADER);


  glAttachShader(programId_,vertexId_);
  glAttachShader(programId_,fragmentId_);


  isInit_=true;
}


void Shader::readFile(const string &resourceName,string *res) {
  QFileInfo resource=p3d::resourceFile(resourceName);

  ifstream f(resource.filePath().toStdString().c_str());
  if (f.fail()) throw ErrorD("fichier shader inexistant");
  char readChar[1000];
  string readString;
  while (f.good()) {
    f.getline(readChar,1000);
    readString=string(readChar);
    *res+=readString+"\n";
  }
  f.close();
}


void Shader::checkCompileError(GLuint id,const std::string &message) {
  int compile_ok;
  GLsizei info_length;

  glGetShaderiv(id,GL_COMPILE_STATUS,&compile_ok);
  glGetShaderiv(id,GL_INFO_LOG_LENGTH,&info_length);


  if (!compile_ok) {
    cout << "=============================" << endl;
    cout << "GLSL Error : " << message << endl;
    char *info=new char[static_cast<size_t>(info_length)];
    glGetShaderInfoLog(id,info_length,nullptr,info);
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Shader compilation error");
  }
}


void Shader::compile(GLuint shader,const string &source) {
  const char *buffer=source.c_str();
  glShaderSource(shader,1,&buffer,nullptr);
  glCompileShader(shader);
}

void Shader::read(const string &name) {
  if (!isInit_) {
    init();
  }
  name_=name;
  readFile(name+".vert",&vertexSource_);
  readFile(name+".frag",&fragmentSource_);
  compile(vertexId_,vertexSource_);
  checkCompileError(vertexId_,"in "+name+".vert");
  compile(fragmentId_,fragmentSource_);
  checkCompileError(fragmentId_,"in "+name+".frag");
  link();
  use();
}

void Shader::link() {
  glLinkProgram(programId_);

  int link_ok;
  GLsizei info_length;

  glGetProgramiv(programId_,GL_LINK_STATUS,&link_ok);
  glGetProgramiv(programId_,GL_INFO_LOG_LENGTH,&info_length);

  if (!link_ok) {
    char *info=new char[static_cast<size_t>(info_length)];
    glGetProgramInfoLog(programId_,info_length,nullptr,info);
    cout << "Info Log :" << endl;
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Link shader program error");
  }
}

void Shader::use() {
  glUseProgram(programId_);
  Shader::current_=this;
}

void Shader::disable() {
  glUseProgram(0);
  Shader::current_=nullptr;
}


int Shader::uniform(string nom) {
  int res=glGetUniformLocation(programId_,nom.c_str());
  return res;
}

void Shader::uniform(const string &nom,float value) {
  int loc=uniform(nom);
  glUniform1f(loc,value);
}

void Shader::uniform(int loc,float value) {
  glUniform1f(loc,value);
}

void Shader::uniform(const string &nom,int value) {
  int loc=uniform(nom);
  glUniform1i(loc,value);
}

void Shader::uniform(int loc,int value) {
  glUniform1i(loc,value);
}


void Shader::uniform(const string &nom,const Vector3 &v) {
  int loc=uniform(nom);
  glUniform3fv(loc,1,Vec3f(v).v());
}

void Shader::uniform(int loc,const Vector3 &v) {
  glUniform3fv(loc,1,Vec3f(v).v());
}

void Shader::uniform(const string &nom,const Matrix4 &m) {
  int loc=uniform(nom);
  glUniformMatrix4fv(loc,1,GL_FALSE,Mat4f(m).v());
}

void Shader::uniform(int loc,const Matrix4 &m) {
  glUniformMatrix4fv(loc,1,GL_FALSE,Mat4f(m).v());
}


void Shader::uniform(const string &nom,const Vector4 &p,int offset) {
  int loc=uniform(nom);
  glUniform4fv(loc+offset,1,Vec4f(p).v());
}

void Shader::uniform(int loc,const Vector4 &p,int offset) {
  glUniform4fv(loc+offset,1,Vec4f(p).v());
}

void Shader::uniform(const string &nom,const vector<int> &t) {
  int loc=uniform(nom+"[0]");
  uniform(loc,t);
}

void Shader::uniform(int loc,const vector<int> &t) {
  glUniform1iv(loc,static_cast<GLsizei>(t.size()),t.data());
}

void Shader::uniform(const string &nom,const Matrix3 &m) {
  int loc=uniform(nom);
  glUniformMatrix3fv(loc,1,GL_FALSE,Mat3f(m).v());
}

void Shader::uniform(int loc,const Matrix3 &m) {
  glUniformMatrix3fv(loc,1,GL_FALSE,Mat3f(m).v());
}

void Shader::uniform(const string &name,const vector<Matrix4> &m) {
  int loc=uniform(name+"[0]");
  uniform(loc,m);
}

void Shader::uniform(const string &name,const vector<Vector4> &v) {
  int loc=uniform(name+"[0]");
  uniform(loc,v);
}


void Shader::uniform(int loc,const vector<Matrix4> &m) {
  vector<float> mf;
  mf.resize(m.size()*16);
  size_t i=0,j=0;
  for(auto &v:mf) {
    v=static_cast<float>(m[i](j));
    if (j==15) {++i;j=0;}
    else ++j;
  }
  glUniformMatrix4fv(loc,static_cast<GLsizei>(m.size()),GL_FALSE,mf.data());
}

void Shader::uniform(int loc,const vector<Vector4> &v) {
  vector<float> mv;
  mv.resize(v.size()*4);
  size_t i=0,j=0;
  for(auto &val:mv) {
    val=static_cast<float>(v[i](j));
    if (j==3) {++i;j=0;}
    else ++j;
  }
  glUniform4fv(loc,static_cast<GLsizei>(v.size()),mv.data());
}



GLuint Shader::id() {
  return programId_;
}

Shader *Shader::current() {
  return Shader::current_;
}






