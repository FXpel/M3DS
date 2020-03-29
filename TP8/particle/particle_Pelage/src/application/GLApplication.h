/**
*
*  @author F. Aubert
*
*/



#pragma once

#include "GLWidget.h"

#include "Camera.h"
#include "Texture.h"

#include "Particle.h"
#include "ParticleList.h"
#include "Engine.h"



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


  void drawGround();
  void updateCamera();

  // ****************************
  // TP :

  void initParticle(EParticleMode mode);
private:

  p3d::Camera camera_;
  double frustum_;


  // ****************************
  // TP :

  Engine engine_;

  bool animate_;

  p3d::Texture particleTexture_,groundTexture_;
  p3d::Line windRay_;



signals:

public slots:

};



