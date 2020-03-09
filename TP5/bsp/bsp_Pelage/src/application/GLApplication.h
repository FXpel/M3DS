#pragma once


/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#include "GLWidget.h"

#include "ObjectBSP.h"

#include "Camera.h"


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
  QSize sizeHint() const override {return QSize(500,500);}


  void display1();
  void display2();
  void display3();
  void display4();

  void updateCamera();



  // TP :

private:
  double zCam_;

  size_t modeDisplay_,solution_,nbSolution_;

  ObjectBSP bsp1_,bsp2_,bsp3_;
  p3d::Vector3 obs_;


  p3d::Camera camera_; // contrôle du placement de la scène à la souris
  double frustum_=0.01;
  bool withBlend_=false;


signals:

public slots:

};


