/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#pragma once
#include "GLWidget.h"

#include "Camera.h"
#include "Texture.h"

#include "BoxList.h"
#include "EngineBox.h"




/**
  @class GLView

  The widget to draw with OpenGL.
  */
class GLApplication : public GLWidget {

  Q_OBJECT
public:
  //! Ctor
  GLApplication();
  //! Dtor
  ~GLApplication() override;

  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/

  void leftPanel(size_t i, const std::string &s) override;
  QSize sizeHint() const override  {return QSize(500,500);}



  // ****************************
  // TP :

private:

  p3d::Camera _camera;


  // ****************************
  // TP :

  bool applyForce_{false};
  p3d::Vector3 pointForce_{};
  EngineBox engineBox_{};
  bool animate_{false};
  bool visualDebug_{true};


signals:

public slots:

};








