#ifndef GLAPPLICATION_H
#define GLAPPLICATION_H

#include "GLWidget.h"

#include "Texture.h"
#include "Camera.h"

#include "Revolution.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert - Univ Lille 1
*
*/

class GLApplication : public GLWidget
{

  typedef enum {Init_Cube,Init_Sphere,Init_Revolution,Init_None} ERequestInit;
  typedef enum {View_Input,View_3D} EView;

public:
  ~GLApplication() override;
  GLApplication();

  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/
  void leftPanel(size_t i, const std::string &s) override;
  QSize sizeHint() const override {return QSize(500,500);}

  void updateCamera();
  void apply(const p3d::Camera &c);

  // ****************************
  // TP :



  void drawScene();

private:
  void drawGroundTool(const p3d::Texture &texture);



private:
  p3d::Camera camera_,cameraInput_;

  Revolution revolution_;

  ERequestInit requestInit_=Init_Sphere; // Init_Cube, Init_Revolution
  Revolution::EShader shaderMode_=Revolution::Shader_Ambient; // Shader_Light, Shader_Texture
  EView viewMode_=View_3D; // View_2D for profile input

};

#endif // GLAPPLICATION_H

