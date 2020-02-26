#include "GLApplication.h"

#include "Shader.h"

#include <iostream>

using namespace std;
using namespace p3d;

GLApplication::~GLApplication() {
}

GLApplication::GLApplication() {
  // _projection de classe Matrix4 (déclaré dans GLApplication.h)
  //projection_=Matrix4::fromOrtho(-20,20,-20,20,5,100); // cf calcul de la matrice dans le cours
  //projection_=Matrix4::fromOrtho(-18,22,-10,30,5,100);
  //projection_=Matrix4::fromOrtho(-5,2,-10,10,5,100); //xmin,xmax, ymin,ymax ,zmin,zmax
  //projection_=Matrix4::fromOrtho(-20,20,-20,20,5,1e8);
  projection_=Matrix4::fromOrtho(-20,20,-20,20,5,100);
  lightPosition_=Vector3(0,0,0);
//  obj_.readInit("cube.obj");
   obj_.readInit("cow.obj");
  obj_.scaleInBoxRatio(-10,10,-10,10,-20,-10);  // reporte l'objet dans la boite d'extémités (-10,-10,-20) et (10,10,-10)
  basicMesh_.initObj(obj_);
  printf("rotiroetieropt");
  basicMesh_.initTetrahedron();

//  angle_=0.0;






}




/** ********************************************************************** **/
void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL
  glClearColor(1,1,1,1);

//  glLineWidth(2.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST); // activation Depth Buffer (opérations écriture/tests)
    glDepthFunc(GL_LESS);  // le test passe si depth(src) < depth(dst)
    glClearDepth(1); // valeur d'initialisation du depth destination de tous les pixels lors d'un glClear

  shader_.read("openGL3D");



  basicMesh_.initDraw();
}

void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  glViewport(0,0,width,height);
  // ...
}

void GLApplication::update() {
  // appelée toutes les 20ms (60Hz)
  // => mettre à jour les données de l'application
  // avant l'affichage de la prochaine image (animation)
  // ...

  angle_+=2;


  transform_=Matrix4::identity();
  transform_.translate(0,0,-15);
  transform_.rotate(angle_,Vector3(1,0.2,0));
  transform_.translate(0,0,15);
  //angle_ =0.0;
}

void GLApplication::draw() {
  // appelée après chaque update
  // => tracer toute l'image

  glClear(GL_COLOR_BUFFER_BIT);


  glUseProgram(shader_.id());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // affecte tous les pixels du color buffer et du depth buffer avec les valeurs d'initialisation
  shader_.uniform("projection",projection_); // utilisation de la classe shader
                                                // qui permet d'alléger la syntaxe OpenGL
  shader_.uniform("transform",transform_);
  shader_.uniform("diffuseColor",Vector3(0.2,0.8,0.2));
  shader_.uniform("lightPosition",lightPosition_);
  basicMesh_.draw();
  glUseProgram(0);

  snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(size_t i, const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  /*
  switch (i) {
    case 0:menu0();break;
    case 1:menu1();break;
  }
  */
}

/** *********************************************************** **/




