#pragma once

#include "GLWidget.h"

#include "MeshObject3D.h"
#include "Texture.h"
#include "CameraInteract.h"
#include "SceneIntersection.h"
#include "DebugTool.h"


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

  void drawGround();
  void updateCamera();

  // ****************************
  // TP :



  void drawScene();

  void drawCow();
  void drawTriceratops();
  void drawTriangle();

  void drawCursor();

  void testIntersection();
  void selectObject();
  void moveSelectedObject();

private:
  typedef enum {Manipulation_Translation,Manipulation_Orientation} EManipulation;

  p3d::MeshObject3D cow_, triceratops_,triangle_;

  CameraInteract camera_;
  p3d::Line pickingRay_;
  p3d::Texture groundTexture_;

  p3d::DebugTool debug_;

  SceneIntersection sceneIntersection_;
  bool isSelectedIntersection_{false};
  Intersection selectedIntersection_;
  p3d::Vector3 attachPointWorld_;

  EManipulation controlMouse_;
};


