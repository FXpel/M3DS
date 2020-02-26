#ifndef REVOLUTION_H
#define REVOLUTION_H

#include <vector>

#include "glsupport.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "InteractPosition.h"

class Revolution : public p3d::InteractPosition
{


public:
  typedef enum {Shader_Ambient,Shader_Light,Shader_Texture} EShader;


  Revolution();

  void init();

  void initCube();
  void initRevolution();
  void initSphere();

  void draw(EShader mode);

  void drawProfile();
  void clearProfile() {profile_.clear();}


  p3d::Vector3 *interactPoint(size_t i) override {return &(profile_[i]);}
  void interactUpdate(size_t) override {}
  size_t interactSize() override {return profile_.size();}
  void interactInsert(size_t i,const p3d::Vector3 &p) override;

private:
  void initVAO(const std::vector<int> &index, const std::vector<float> &position, const std::vector<float> &normal={}, const std::vector<float> &texCoord={});



  GLuint vao_{0},elementBuffer_{0},vbo_{0};
  int nbElement_{0};

  p3d::Texture dice_,earth_,logoFST_;
  p3d::Texture *currentTexture_{nullptr};

  std::vector<p3d::Vector3> profile_{};
};

#endif // REVOLUTION_H

