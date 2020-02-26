#include "GLApplication.h"

#include <iostream>
#include <cmath>

using namespace std;

GLApplication::~GLApplication() {
  makeCurrent();
}

GLApplication::GLApplication() {

//  trianglePosition_ = {
//    -0.8f,-0.5f,0.0f, // vertex 0
//    -0.2f,-0.5f,0.0f, // 1
//    -0.5f,0.5f,0.0f,  // 2

//    0.2f,0.5f,0.0f,  // 3
//    0.8f,0.5f,0.0f,  // 4
//    0.5f,-0.5f,0.0f  // 5
//  };
//    trianglePosition_ = {
//      -0.8f,-0.5f, 0.0f, // vertex 0 anciennement vertex 0
//       0.8f, 0.5f, 0.0f, // 1 anciennement 4
//      -0.5f, 0.5f, 0.0f, // 2 anciennement 2
//      -0.2f,-0.5f, 0.0f, // 3 anciennement 1
//       0.5f,-0.5f, 0.0f, // 4 anciennement 5
//       0.2f, 0.5f, 0.0f  // 5 anciennement 3
//    };
//    trianglePosition_ = {
//      -0.8f,-0.8f, 0.0f,
//       0.8f, 0.8f, 0.0f,
//       0.0f, 0.2f, 0.0f,
//      -0.8f, 0.8f, 0.0f,
//       0.8f,-0.8f, 0.0f
//    };

//    trianglePosition_ = {
//      -0.8f,-0.8f, 0.0f,
//      -0.8f, 0.8f, 0.0f,
//      -0.4f,-0.8f, 0.0f,
//      -0.4f, 0.8f, 0.0f,
//       0.0f,-0.8f, 0.0f,
//       0.0f, 0.8f, 0.0f,
//       0.4f,-0.8f, 0.0f,
//       0.4f, 0.8f, 0.0f,
//       0.8f,-0.8f, 0.0f
//    };
//    // tous les sommets à rouge :
//    triangleColor_.clear();
//    for(unsigned int i=0;i<=trianglePosition_.size()/3 ;++i) {
//      triangleColor_.push_back(1);triangleColor_.push_back(0);triangleColor_.push_back(0);triangleColor_.push_back(1);
//    };
//  triangleColor_ = {
//    0.3f,0.0f,0.6f,1.0f,
//    0.3f,0.0f,0.6f,1.0f,
//    0.0f,0.9f,0.0f,1.0f,

//    0.0f,0.5f,0.6f,1.0f,
//    0.0f,0.5f,0.6f,1.0f,
//    0.9f,0.0f,0.0f,1.0f
//  };
//  coeff_ = 0;
    trianglePosition_ = { // rectangle tracé avec TRIANGLE_STRIP
      -0.6f,-0.8f,0.0f,
      -0.6f, 0.8f,0.0f,
       0.6f,-0.8f,0.0f,
       0.6f, 0.8f,0.0f
    };
    triangleColor_ = { // tous les sommets en rouge
      1,0,0,1,
      1,0,0,1,
      1,0,0,1,
      1,0,0,1,
    };
//    triangleTexCoord_ = { // coordonnées de texture en chaque sommet
//      0,1,
//      0,0,
//      1,1,
//      1,0
//    };

    //
//    triangleTexCoord_ = { // coordonnées de texture en chaque sommet
//      0,0,
//      0,1,
//      1,0,
//      1,1
//    };


//  elementData_ = {0,3,2,2,1,4};
  //initStrip(15,-0.8,0.8,-0.8,0.8);
  initRing(30,0.2,0.8);



}




/** ********************************************************************** **/
void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL
  glClearColor(1,1,1,1);

  glLineWidth(2.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


  shader0_=initProgram("simple");


  initTriangleBuffer();
  initTriangleVAO();
  initTexture();


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


    if(f ){
        if(coeff_ >= 1){
            f = false;
        }
        coeff_ += 0.005;
    }
    else{
        if(coeff_ <= 0){
            f = true;
        }
        coeff_ -= 0.005;
    }


}

void GLApplication::draw() {
  // appelée après chaque update
  // => tracer toute l'image
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shader0_);
  glUniform1f(glGetUniformLocation(shader0_,"coeff"),coeff_);
  glBindVertexArray(triangleVAO_);

//  glDrawArrays(GL_TRIANGLES,0,6);
  //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
  //glDrawArrays(GL_TRIANGLES,0,9);
  //glDrawArrays(GL_TRIANGLE_STRIP,0,trianglePosition_.size()/3);
  glActiveTexture(GL_TEXTURE0); // on travaille avec l'unité de texture 0
    // dans l'instruction suivante, textureId_ correspond à l'image "lagoon.jpg"; cf GLApplication::initTexture pour l'initialisation de textureId_
    glBindTexture(GL_TEXTURE_2D,textureId_); // l'unité de texture 0 correspond à la texture _textureId
    // (le fragment shader manipule des unités de textures et non les identifiants de texture directement)
    glUniform1f(glGetUniformLocation(shader0_,"textureUnit"),0); // on affecte la valeur du sampler2D du fragment shader à l'unité de texture 0.

    glDrawArrays(GL_TRIANGLE_STRIP,0,trianglePosition_.size()/3);
  glBindVertexArray(0);
  glUseProgram(0);


  snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  /*
  switch (i) {
    case 0:menu0();break;
    case 1:menu1();break;
  }
  */
}




/** *********************************************************** **/
GLuint GLApplication::initProgram(const std::string &filename) {
  GLuint program;
  int infoLength; // for message error

  string vertexSource=p3d::readTextFile(filename+".vert");
  string fragmentSource=p3d::readTextFile(filename+".frag");

  program=glCreateProgram();
  GLuint vertexShader=glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
  glAttachShader(program,vertexShader);
  glAttachShader(program,fragmentShader);

  const char *source;
  source=vertexSource.c_str();
  glShaderSource(vertexShader,1,&source,nullptr);
  source=fragmentSource.c_str();
  glShaderSource(fragmentShader,1,&source,nullptr);

  GLint ok;
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&ok);
  if (!ok) {
    glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&infoLength);
    cout << "=============================" << endl;
    cout << "GLSL Error : " << endl;
    char *info=new char[size_t(infoLength)];
    glGetShaderInfoLog(vertexShader,infoLength,nullptr,info);
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Vertex Shader compilation error in "+filename+".vert");
  }

  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&ok);
  if (!ok) {
    glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&infoLength);
    cout << "=============================" << endl;
    cout << "GLSL Error : " << endl;
    char *info=new char[size_t(infoLength)];
    glGetShaderInfoLog(fragmentShader,infoLength,nullptr,info);
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Fragment Shader compilation error in "+filename+".frag");
  }

  glLinkProgram(program);
  glGetProgramiv(program,GL_LINK_STATUS,&ok);
  if (!ok) {
    glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLength);
    char *info=new char[size_t(infoLength)];
    glGetProgramInfoLog(program,infoLength,nullptr,info);
    cout << "Info Log :" << endl;
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Program Shader : link error (varyings ok ?)");
  }

  return program;
}


void GLApplication::initTexture() {
    QImage img;
    img.load("../media/lagoon.jpg");
    img=img.convertToFormat(QImage::Format_ARGB32).mirrored();

    glGenTextures(1,&textureId_);
    glBindTexture(GL_TEXTURE_2D,textureId_);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width(),img.height(),0,GL_BGRA,GL_UNSIGNED_BYTE,img.bits());



    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}


void GLApplication::initTriangleBuffer() {

  glGenBuffers(1,&trianglePositionBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,trianglePositionBuffer_);
  glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(trianglePosition_.size()*sizeof(float)),trianglePosition_.data(),GL_STATIC_DRAW);
  glGenBuffers(1,&triangleColorBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,triangleColorBuffer_);
  glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(triangleColor_.size()*sizeof(float)),triangleColor_.data(),GL_STATIC_DRAW);
  glGenBuffers(1,&elementBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,elementData_.size()*sizeof(unsigned int),elementData_.data(),GL_STATIC_DRAW);

  glGenBuffers(1,&triangleTexCoordBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,triangleTexCoordBuffer_);
  glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(triangleTexCoord_.size()*sizeof(float)),triangleTexCoord_.data(),GL_STATIC_DRAW);

}


void GLApplication::initTriangleVAO() {
  glGenVertexArrays(1,&triangleVAO_);

  glBindVertexArray(triangleVAO_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glBindBuffer(GL_ARRAY_BUFFER,trianglePositionBuffer_);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);

  glBindBuffer(GL_ARRAY_BUFFER,triangleColorBuffer_);
  glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,nullptr);

  glBindBuffer(GL_ARRAY_BUFFER,triangleTexCoordBuffer_);
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,nullptr);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  glBindVertexArray(0);
}

void GLApplication::initStrip(int nbSlice,float xmin,float xmax,float ymin,float ymax){
    float dist_x = (xmax - xmin)/nbSlice;
    float col = (float) 1 / nbSlice;
    trianglePosition_.clear();
    triangleColor_.clear();
    for(unsigned int i=0;i < nbSlice;i++){
        trianglePosition_.push_back(xmin + i*dist_x);trianglePosition_.push_back(ymin);trianglePosition_.push_back(0);
        trianglePosition_.push_back(xmin + i*dist_x);trianglePosition_.push_back(ymax);trianglePosition_.push_back(0);
        triangleColor_.push_back(0);triangleColor_.push_back(i*col);triangleColor_.push_back(0);triangleColor_.push_back(1);
        triangleColor_.push_back(0);triangleColor_.push_back(0);triangleColor_.push_back(1-i*col);triangleColor_.push_back(1);
    }
}
//question 31 cos = -1 et 1
// on veut le mettre entre 0 et 1
void GLApplication::initRing(int nbSlice,float r0,float r1){
    float pi = 3.14159;
    float pi_slice = (2* pi)/nbSlice;
    float col = (float) 1 / nbSlice;
    float img = (float) 1 / nbSlice;
    trianglePosition_.clear();
    triangleColor_.clear();
    for(unsigned int i=0;i < nbSlice;i++){
        trianglePosition_.push_back(r0 * cos(i*pi_slice));trianglePosition_.push_back(r0 * sin(i*pi_slice));trianglePosition_.push_back(0);
        trianglePosition_.push_back(r1 * cos(i*pi_slice));trianglePosition_.push_back(r1 * sin(i * pi_slice));trianglePosition_.push_back(0);
        triangleColor_.push_back(0);triangleColor_.push_back(i*col);triangleColor_.push_back(0);triangleColor_.push_back(1);


        triangleColor_.push_back(0);triangleColor_.push_back(0);triangleColor_.push_back(1-i*col);triangleColor_.push_back(1);
//        triangleTexCoord_.push_back(i*pi_slice/(2*pi));triangleTexCoord_.push_back(1);
//        triangleTexCoord_.push_back(i*pi_slice/(2*pi));triangleTexCoord_.push_back(0);


        triangleTexCoord_.push_back((r0 * cos(i*pi_slice))/2+0.5);triangleTexCoord_.push_back((r0 * sin(i*pi_slice))/2 + 0.5);
        triangleTexCoord_.push_back((r1 * cos(i*pi_slice))/2 + 0.5);triangleTexCoord_.push_back((r1 * sin(i*pi_slice))/2 + 0.5);



    }
    trianglePosition_.push_back(r0 * cos(0));trianglePosition_.push_back(r0 * sin(0));trianglePosition_.push_back(0);
    trianglePosition_.push_back(r1 * cos(0));trianglePosition_.push_back(r1 * sin(0));trianglePosition_.push_back(0);
    triangleColor_.push_back(0);triangleColor_.push_back(1);triangleColor_.push_back(0);triangleColor_.push_back(1);
    triangleColor_.push_back(0);triangleColor_.push_back(0);triangleColor_.push_back(0);triangleColor_.push_back(1);

//    triangleTexCoord_.push_back(1);triangleTexCoord_.push_back(1);
//    triangleTexCoord_.push_back(1);triangleTexCoord_.push_back(0);

    triangleTexCoord_.push_back(r0 * cos(0)/2 + 0.5);triangleTexCoord_.push_back(r0 * sin(0)/2 + 0.5);
    triangleTexCoord_.push_back(r1 * cos(0)/2 + 0.5);triangleTexCoord_.push_back(r1 * sin(0)/2 + 0.5);

}

