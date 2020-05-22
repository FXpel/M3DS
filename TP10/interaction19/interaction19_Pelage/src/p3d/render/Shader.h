#pragma once

#include "glsupport.h"
#include <string>
#include <map>
#include <vector>

/**
@file
@author F. Aubert
@brief Encapsulates common shader operations
*/

namespace p3d {
  class Vector3;
  class Vector4;
  class Matrix3;
  class Matrix4;


  class Shader {

  public:
    ~Shader();
    void release();
    Shader();

    void init();

    void read(const std::string &name);
    void link();

    void use();
    void disable();

    void uniform(const std::string &nom,float value);
    void uniform(int loc,float value);
    void uniform(const std::string &nom,int value);
    void uniform(int loc,int value);
    void uniform(const std::string &nom,const Vector3 &v);
    void uniform(int loc,const Vector3 &v);
    void uniform(const std::string &nom,const Matrix4 &m);
    void uniform(int loc,const Matrix4 &m);
    void uniform(const std::string &nom,const Vector4 &p,int offset=0);
    void uniform(int loc,const Vector4 &p,int offset=0);
    void uniform(const std::string &nom,const Matrix3 &m);
    void uniform(int loc,const Matrix3 &m);
    GLint uniform(std::string nom);

    GLuint id();

    static Shader *current();

    void uniform(const std::string &nom, const std::vector<int> &t);
    void uniform(int loc, const std::vector<int> &t);
    void uniform(const std::string &name, const std::vector<Matrix4> &m);
    void uniform(int loc, const std::vector<Matrix4> &m);
    void uniform(int loc, const std::vector<Vector4> &v);
    void uniform(const std::string &name, const std::vector<Vector4> &v);
    std::string vertexSource_{""},fragmentSource_{""}; // sources are kept for merging


  private:
    void readSource(GLuint id,std::string nom);
    void compile(GLuint shader, const std::string &source);
    void readFile(const std::string &nom,std::string *res);
    void checkCompileError(GLuint id, const std::string &message);


    std::string name_ {""};
    GLuint programId_{0},vertexId_{0},fragmentId_{0};
    bool isInit_{false};
    static Shader *current_;
  };

}




