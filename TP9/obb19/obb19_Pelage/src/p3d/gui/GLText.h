#pragma once

#include <sstream>
#include <iomanip>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/



namespace p3d {

template <class T> std::string toString(const T &v) {std::stringstream s;s<< std::setprecision(2) <<v;return s.str();}


namespace GLText {
  void end();




  unsigned int toTexture(const std::string &s, int *width, int *height);
  void initTextShader();
  void initialize();
  // render text in OpenGL >= 3.2 : slow cause the texture is computed each call
  void draw(const std::string &s, double x, double y, double zz=0, const float *modelview=nullptr, const float *projection=nullptr, const float *color=nullptr);
}

}

