/**
*
*  @author F. Aubert
*  @file
*
*/

#pragma once


#include "GLWidget.h"
#include "Camera.h"
#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"
#include "Texture.h"

/**
  @class GLView

  The widget to draw with OpenGL.
  */
class GLApplication : public GLWidget {
  typedef enum {Draw_Bezier,Draw_Catmull,Draw_Hermite} EDraw;
  typedef enum {Control_Bezier,Control_Catmull,Control_Hermite} EControl;
  typedef enum {Mouse_Interact,Mouse_Nothing} EMouse;

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


  // ****************************
  // TP :

  void initProjection();
  void drawHermite();
  void drawBezier();
  void drawCatmullRom();

  void printMode();

private:

  p3d::Camera camera2D_;


  // ****************************
  // TP :
  EDraw modeDraw_{Draw_Hermite};

  Hermite hermite_{};
  Bezier bezier_{};
  CatmullRom catmullRom_{};

  p3d::InteractPosition *whichInteraction_{nullptr};

  unsigned int viewControlPoint_{0};
  double tAnimation_{0.0};
  bool animateTBN_{false};

signals:

public slots:

};



