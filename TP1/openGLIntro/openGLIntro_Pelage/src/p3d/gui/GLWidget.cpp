#include <QMouseEvent>
#include <QKeyEvent>

#include "GLWidget.h"
#include "GLApplication.h"
#include "GLText.h"
#include "Tools.h"

#include <iostream>

#include <QDebug>



/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace std;
using namespace p3d;

GLWidget::GLWidget() :
  QOpenGLWidget(nullptr) {

  setFocusPolicy(Qt::StrongFocus);
  grabKeyboard();


  GLText::initialize();

  connect(this,SIGNAL(updateRequest()),this,SLOT(updateData()));

  firstElapsed_=true;
  snapshot_=false;

}


/// dtor
GLWidget::~GLWidget() {
  makeCurrent();
}



/** ***************************************************************************
 initialize GL context + common set up
 **/
void GLWidget::initializeGL() {
  glewExperimental=GL_TRUE;
  // initialization of all available OpenGL functionalities
  if (glewInit()!=GLEW_OK) {
    cout << "glew error : unable to initialize glew" << endl;
    exit(EXIT_FAILURE);
  }
  while (glGetError()!=GL_NO_ERROR); // there's a bug in GLEW that generates glError => ignores it

  cout << "GL version :" << glGetString(GL_VERSION) << endl;
  cout << "Shading Language  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;



  initData();
}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (includes when the window is shown for the first time)
void GLWidget::resizeGL(int width, int height) {
  dimension(width,height);
  resize(width,height);
}



/** **********************************************************************
  Events
  **/
void GLWidget::mousePressEvent(QMouseEvent *event) {
  mouse(event->x(),event->y());
  resetDeltaMouse();
  if (event->button()==Qt::LeftButton) {
    mouseLeft(true);
  }
  if (event->button()==Qt::RightButton) {
    mouseRight(true);
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  mouse(event->x(),event->y());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  mouse(event->x(),event->y());
  switch(event->button()){
  case Qt::LeftButton:
      mouseLeft(false);
      break;
  case Qt::RightButton:
      mouseRight(false);
    break;
  default:break;
  }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  if (event->delta()<0)
    decelerateWheel(true);
  else
    accelerateWheel(true);
}


void GLWidget::keyPressEvent(QKeyEvent *event) {
  QPoint cursor=mapFromGlobal(QCursor::pos());
  mouse(cursor.x(),cursor.y());
  key(event->key(),true);
  QOpenGLWidget::keyPressEvent(event); // dispatch the event to the parent
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
  key(event->key(),false);
  QOpenGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
}


  /** ***************************************************************************
  init/update data
  **/
void GLWidget::initData() {
  initialize();
}

void GLWidget::updateData() {
  update();
  QWidget::update();
}

void GLWidget::paintGL() {
  draw();
  snapshot();
}


/** ***********************************************************************************
 * Interaction helpers
 **/
bool GLWidget::mouseLeft() {return mouseLeftButton_;}
bool GLWidget::mouseRight() {return mouseRightButton_;}
bool GLWidget::mouseLeftPressed() {bool pressed=mouseLeftPressed_;mouseLeftPressed_=false;return pressed;}
bool GLWidget::mouseRightPressed() {bool pressed=mouseRightPressed_;mouseRightPressed_=false;return pressed;}

int GLWidget::mousePosX() {return this->mapFromGlobal(QCursor::pos()).x();}
int GLWidget::mousePosY() {return this->height()-this->mapFromGlobal(QCursor::pos()).y();}

int GLWidget::mouseX() {return mouseX_;}
int GLWidget::mouseY() {return mouseY_;}
int GLWidget::deltaMouseX() {deltaMouseX_=mouseX_-oldMouseX_;oldMouseX_=mouseX_;return deltaMouseX_;}
int GLWidget::deltaMouseY() {deltaMouseY_=mouseY_-oldMouseY_;oldMouseY_=mouseY_;return deltaMouseY_;}

void GLWidget::mouseLeft(bool v) {mouseLeftButton_=v;mouseLeftPressed_=v;}
void GLWidget::mouseRight(bool v) {mouseRightButton_=v;mouseRightPressed_=v;}

void GLWidget::mouse(int x,int y) {mouseX_=x;mouseY_=height_-y;}
void GLWidget::resetDeltaMouse() {oldMouseX_=mouseX_;oldMouseY_=mouseY_;}

void GLWidget::dimension(int w,int h) {width_=w;height_=h;}
int GLWidget::width() {return width_;}
int GLWidget::height() {return height_;}

bool GLWidget::accelerateWheel() {bool res;res=pressAccelerateWheel_;pressAccelerateWheel_=false;return res;}
bool GLWidget::decelerateWheel() {bool res;res=pressDecelerateWheel_;pressDecelerateWheel_=false;return res;}
void GLWidget::accelerateWheel(bool v) {pressAccelerateWheel_=v;}
void GLWidget::decelerateWheel(bool v) {pressDecelerateWheel_=v;}

bool GLWidget::left() {return key_[Qt::Key_Q] || key_[Qt::Key_Left];}
bool GLWidget::right() {return key_[Qt::Key_D] || key_[Qt::Key_Right];}
bool GLWidget::forward() {return key_[Qt::Key_Z] || key_[Qt::Key_Up];}
bool GLWidget::backward() {return key_[Qt::Key_S] || key_[Qt::Key_Down];}
bool GLWidget::down() {return forward();}
bool GLWidget::up() {return backward();}
bool GLWidget::rotateLeft() {return key_[Qt::Key_A];}
bool GLWidget::rotateRight() {return key_[Qt::Key_E];}

void GLWidget::key(int k,bool v) {key_[k]=v;if (v) {keyPressed_[k]=v;lastKeyPressed_=k;}}
bool GLWidget::key(int k) {return key_[k];}
bool GLWidget::keyPressed(int k) {bool pressed=keyPressed_[k];keyPressed_[k]=false;return pressed;}
int GLWidget::keyPressed() {return ((lastKeyPressed_!=-1 && keyPressed(lastKeyPressed_))?lastKeyPressed_:-1);}





/** i = button number, s = button text
 */
void GLWidget::leftPanel(size_t i, const std::string &s) {
  cout << "button clicked " << i << " " << s << endl;
}

void GLWidget::requestSnapshot() {
  snapshot_=true;
}

void GLWidget::snapshot() {
  if (snapshot_) {
    p3d::captureImage(0,0,width_,height_);
    snapshot_=false;
  }
}








