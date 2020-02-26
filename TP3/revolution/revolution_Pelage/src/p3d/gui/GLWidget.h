#pragma once

#include "glsupport.h"
#include <QOpenGLWidget>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

class GLWidget : public QOpenGLWidget {
  Q_OBJECT

protected:

public:
  explicit GLWidget();
  virtual ~GLWidget();

/** interface for child class (typically named GLApplication) **/
  virtual void initialize()=0;  // OpenGL resources acquisition
  virtual void resize(int width,int height)=0; // Window server is resized
  virtual void update()=0; // update application data
  virtual void draw()=0;   // draw data

/** Interaction helpers **/
  bool mouseLeft();
  bool mouseRight();
  bool mouseLeftPressed();
  bool mouseRightPressed();

  void mouseLeft(bool v);
  void mouseRight(bool v);

  void dimension(int w,int h);
  int width();
  int height();

  void mouse(int x,int y);

  int mouseX();
  int mouseY();
  int deltaMouseX();
  int deltaMouseY();

  bool left();
  bool right();
  bool forward();
  bool backward();
  bool down();
  bool up();
  bool rotateLeft();
  bool rotateRight();

  bool accelerateWheel();
  bool decelerateWheel();
  void accelerateWheel(bool v);
  void decelerateWheel(bool v);

  void key(int k,bool v);
  bool key(int k);
  bool keyPressed(int k);
  int keyPressed();

  void startTime();
  double elapsedStartTime(); ///! since last startTime
  double elapsedTime(); ///! since last elapsedTime

  void resetDeltaMouse();

  void snapshot();
  void requestSnapshot();

  // public members
  QStringList leftPanelMenu_;

  //! default call when a button in left panel is clicked
  virtual void leftPanel(size_t i,const std::string &s);

  QSize sizeHint() const {return QSize(512,512);}

  int mousePosX();
  int mousePosY();
protected:
  //! initialize data application
  void initData();
  //! initialize the OpenGL context
  void initializeGL();
  //! OpenGL drawing (called when updateGL() )
  void paintGL();
  //! should update projection (called when the widget is resized)
  void resizeGL(int width,int height);

  // events
  //! called when a mouse button is pressed in the widget
  void mousePressEvent(QMouseEvent *event);
  //! called when the mouse is moved in the widget
  void mouseMoveEvent(QMouseEvent *event);
  //! called when a mouse button is released in the widget
  void mouseReleaseEvent(QMouseEvent *event);
  //! called when a key is pressed in the widget
  void keyPressEvent( QKeyEvent *event );
  //! called when a key is released in the widget
  void keyReleaseEvent( QKeyEvent *event );
  //! called when the mouse wheel is turned
  void wheelEvent(QWheelEvent *event);



signals:
  void updateRequest();
public slots:
  //! called every 20ms by the MainWindow
  void updateData();

private:
  // Interaction
  bool mouseLeftButton_=false; //! to detect when mouse left button is down (true until the button is released).
  bool mouseRightButton_=false;
  bool mouseLeftPressed_=false; //! to detect when mouse left button is pressed (set back to false when mouseLeftPressed() is called)
  bool mouseRightPressed_=false;

  bool pressAccelerateWheel_=false;
  bool pressDecelerateWheel_=false;
  int  width_=0, height_=0;
  int  mouseX_=0;
  int  mouseY_=0;
  int  oldMouseX_=0;
  int  oldMouseY_=0;
  int  deltaMouseX_=0;
  int  deltaMouseY_=0;

  std::map<int,bool> key_;
  std::map<int,bool> keyPressed_;
  int lastKeyPressed_;

  bool firstElapsed_=true;
  time_t lastElapsed_;
  time_t lastStart_;
  bool snapshot_;


};


