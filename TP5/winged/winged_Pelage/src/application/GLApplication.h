#pragma once

/**

  @author F. Aubert
  **/


#include "GLWidget.h"
#include "Texture.h"
#include "Camera.h"

#include <string>
#include <QSize>

#include "Winged.h"

class WEdge;

class GLApplication : public GLWidget {
  Q_OBJECT
public:
  GLApplication();
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

private:
  void updateCamera();
  void drawWinged();
  void drawGround();


  size_t nbObject_;
  std::array<Winged,3> wingedObject_;

  bool visuPoint_;
  bool visuBoundary_;
  bool visuNormal_;
  size_t visuHide_;
  bool computeVertexNormal_;
  bool drawLineCatmull_;

  std::vector<WEdge *> boundary_;

  size_t active_;

  double frustum_=0.01;
  p3d::Texture ground_;
  p3d::Camera camera_;


};


