#include "Revolution.h"

#include "glsupport.h"


#include "GLDraw.h"
#include "GLRender.h"

#include "Vector3.h"


using namespace std;
using namespace p3d;

Revolution::Revolution() {
  // default profile
  profile_.push_back(Vector3(-0.8,0.6,0.0));
  profile_.push_back(Vector3(-0.5,0.0,0.0));
  profile_.push_back(Vector3(-0.9,-0.8,0.0));

}


void Revolution::initSphere() {
  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<int> index;

  int nbSlice=20; // include last slice that closes sphere
  int nbStack=20;

  // *******
  //  TODO


  float teta = (2 * M_PI)/(nbSlice-1);
  float phi = M_PI/(nbSlice-1);
  float si = teta /(2*M_PI);
  float ti = phi/M_PI;
  for (int i = 0; i < nbSlice; ++i) {
      for (int j = 0; j < nbStack; ++j) {
          float x= cos(i*teta) * sin(j*phi);
          float y= cos(j*phi);
          float z=sin(i*teta) * sin(j*phi);
          p.push_back(x);
          p.push_back(y);
          p.push_back(z);

          n.push_back(x);
          n.push_back(y);
          n.push_back(z);


          t.push_back(i*teta/(2*M_PI));
          t.push_back(j*phi/M_PI);


      }



  }
  for (int i = 0; i < nbSlice-1; ++i) {
      for (int j = 0; j < nbStack-1; ++j) {

          index.push_back((i * nbSlice) + j+1);
          index.push_back((i * nbSlice) + j);
          index.push_back((i+1) * nbSlice + j);

          index.push_back((i+1) * nbSlice + j);
          index.push_back(((i+1) * nbSlice) + j+1);
          index.push_back((i * nbSlice) + j+1);


      }
  }

//  index.push_back(6);
//  index.push_back(0);
//  index.push_back(1);
//  index.push_back(1);
//  index.push_back(7);
//  index.push_back(6);

  // *******




  initVAO(index,p,n,t);

  currentTexture_=&earth_;

}


void Revolution::initCube() {
  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<int> index;

  //Q1
//  p={
//      -1,-1,-1,//s0
//      1,-1,-1,//s1
//      -1,1,-1,//s2
//      1,1,-1, //s3
//      -1,-1,1,//s4
//      1,-1,1,//s5
//      -1,1,1,//s6
//      1,1,1 //s7


//  };

//  index={
//      2,0,1, //face arrière
//      1,3,2,

//      3,1,5, //face droite
//      5,7,3,

  //      2,0,4, // face gauche
  //      4,6,2,

  //      2,6,7, // face haute
  //      7,3,2,

  //      0,4,5, // face basse
  //      5,1,0,

  //      6,4,5,//face avant
  //      5,7,6

//};

    p={
        -1,-1,-1,//s0 = 0
        -1,-1,-1,//s0 = 1
        -1,-1,-1,//s0 = 2
        1,-1,-1,//s1 = 3
        1,-1,-1,//s1 = 4
        1,-1,-1,//s1 = 5
        -1,1,-1,//s2 = 6
        -1,1,-1,//s2 = 7
        -1,1,-1,//s2 = 8
        1,1,-1, //s3 =9
        1,1,-1, //s3 = 10
        1,1,-1, //s3 = 11
        -1,-1,1,//s4 = 12
        -1,-1,1,//s4 = 13
        -1,-1,1,//s4 = 14
        1,-1,1,//s5 = 15
        1,-1,1,//s5 = 16
        1,-1,1,//s5 = 17
        -1,1,1,//s6 = 18
        -1,1,1,//s6 = 19
        -1,1,1,//s6 = 20
        1,1,1, //s7 =21
        1,1,1, //s7 = 22
        1,1,1 //s7 = 23


    };

    index={
        3,0,6, // face arrière
        6,9,3,

        10,15,4,// face droite
        21,15,10,

        7,1,12, // face gauche
        12,18,7,

        8,19,22, // face haute
        22,11,8,

        2,16,13, // face basse
        5,16,2,

//        14,20,17,//face avant
//        23,17,20
        17,20,14,//face avant
        20,17,23

    };
    n={
        0, 0, -1,//s0 = 0 => face arrière
        -1, 0, 0,//s0 = 1 => face gauche
        0, -1, 0,//s0 = 2 => face basse

        0, 0, -1,//s1 = 3 => face arrière
        1, 0, 0,//s1 = 4 => face droite
        0, -1, 0,//s1 = 5 => face basse
        0, 0, -1,//s2 = 6 => face arrière
        -1, 0, 0,//s2 = 7 => face gauche
        0, 1, 0,//s2 = 8 => face haute
        0, 0, -1, //s3 =9 => face arrière
        1, 0, 0, //s3 = 10 => face droite
        0, 1, 0, //s3 = 11 => face haute
        -1, 0, 0,//s4 = 12 => face gauche
        0, -1, 0,//s4 = 13 => face basse
        0, 0, 1,//s4 = 14 => face avant
        1, 0, 0,//s5 = 15 => face droite
        0, -1, 0,//s5 = 16 => face basse
        0, 0, 1,//s5 = 17 => face avant
        -1, 0, 0,//s6 = 18 => face gauche
        0, 1, 0,//s6 = 19 => face haute
        0, 0, 1,//s6 = 20 => face avant
        1, 0, 0, //s7 =21 => face droite
        0, 1, 0, //s7 = 22 => face haute
        0, 0, 1 //s7 = 23 => face avant


    };

    t = {
        2.0/3.0, 0,//s0 = 0 => face arrière
        1.0/3.0, 1.0/2.0,//s0 = 1 => face gauche
        0, 1.0/2.0,//s0 = 2 => face basse
        1, 0,//s1 = 3 => face arrière
        1.0/3.0, 0,//s1 = 4 => face droite
        1.0/3.0, 1.0/2.0,//s1 = 5 => face basse
        2.0/3.0, 1.0/2.0,//s2 = 6 => face arrière
        1.0/3.0, 1,//s2 = 7 => face gauche
        2.0/3.0, 1,//s2 = 8 => face haute
        1, 1.0/2.0, //s3 =9 => face arrière
        1.0/3.0, 1.0/2.0, //s3 = 10 => face droite
        1, 1, //s3 = 11 => face haute
        2.0/3.0, 1.0/2.0,//s4 = 12 => face gauche
        0, 0,//s4 = 13 => face basse
        0, 1.0/2.0,//s4 = 14 => face avant
        2.0/3.0, 0,//s5 = 15 => face droite
        1.0/3.0, 0,//s5 = 16 => face basse
        1.0/3.0, 1.0/2.0,//s5 = 17 => face avant
        2.0/3.0, 1,//s6 = 18 => face gauche
        2.0/3.0,1.0/2.0,//s6 = 19 => face haute
        0, 1.0,//s6 = 20 => face avant
        2.0/3.0, 1.0/2.0, //s7 =21 => face droite
        1, 1.0/2.0, //s7 = 22 => face haute
        1.0/3.0, 1 //s7 = 23 => face avant
    };




  initVAO(index,p,n,t);

  currentTexture_=&dice_;
}


void Revolution::initRevolution() {
  if (profile_.size()<2) return; // more than 2 vertices in the profile

  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<int> index;

  size_t nbSlice=40;
  size_t nbStack=profile_.size();
  float teta = (2 * M_PI)/(nbSlice-1);
  std::vector<Vector3> normalProfile; // to set normal profile
  Vector3 A;
  for (int var = 0; var < nbStack-1; ++var) {
      Vector3 dir = profile_[var+1] - profile_[var];
      Vector3 ortho;
      ortho.x(dir.y());
      ortho.y(-dir.x());
      ortho.z(0);
      normalProfile.push_back(ortho);
  }
  // *******
  //  TODO
  float pi_slice = (2*M_PI)/(nbSlice-1);
  for (size_t i = 0; i < nbStack; ++i) {
      Vector3 vec1,vec2;
      if (i == nbStack-1) {
        vec1 = normalProfile[nbStack-1];
        vec2 = normalProfile[nbStack-1];
      }
      if (i == 0) {
        vec1 = normalProfile[0];
        vec2 = normalProfile[0];
      }
      else {
        vec1 = normalProfile[i];
        vec2 = normalProfile[i-1];
      }
      Vector3 normal = (vec1 + vec2)/2;
      for (size_t j = 0; j < nbSlice; ++j) {

          A = profile_[i].rotationY(j*pi_slice);
          Vector3 moy= normal.rotationY(j*pi_slice);
          float x = A.x();
          float y = A.y();
          float z = A.z();
          p.push_back(x);
          p.push_back(y);
          p.push_back(z);

          n.push_back(moy.x());
          n.push_back(moy.y());
          n.push_back(moy.z());
          float fnbStack = (float)(nbStack-1);
          float fi = (float)i;
          t.push_back((j*teta)/(2*M_PI));
          t.push_back((fnbStack-fi)/fnbStack);


      }

  }
  for (size_t i = 0; i < nbStack-1; ++i) {
      for (size_t j = 0; j < nbSlice-1; ++j) {

          index.push_back((i * nbSlice) + j+1);
          index.push_back((i * nbSlice) + j);
          index.push_back((i+1) * nbSlice + j);

          index.push_back((i+1) * nbSlice + j);
          index.push_back(((i+1) * nbSlice) + j+1);
          index.push_back((i * nbSlice) + j+1);


      }
  }

  // *******





  initVAO(index,p,n,t);
  currentTexture_=&logoFST_;
}


void Revolution::init() {
  glGenBuffers(1,&vbo_);
  glGenBuffers(1,&elementBuffer_);
  glGenVertexArrays(1,&vao_);

  earth_.readMipmap("EarthDay512.jpg");
  dice_.readMipmap("dice.png");
  logoFST_.readMipmap("logoFSTnot.png");

}


void Revolution::draw(EShader mode) {
//  return;
  glBindVertexArray(vao_);


  p3d::material(Vector4(0,0,0,1),Vector3(0,0.8,0.2),Vector3(0,0.2,0.8),100);
  p3d::diffuseBackColor=Vector3(0.8,0,0);
  if (mode==Shader_Ambient) {
    p3d::ambientColor=Vector4(0.0,0.8,0,1);
    p3d::ambientBackColor=Vector4(0.8,0.0,0,1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  currentTexture_->bind(0);

  switch(mode) {
  case Shader_Ambient:p3d::shaderVertexAmbient();break;
  case Shader_Light:p3d::shaderLightPhong();break;
  case Shader_Texture:p3d::shaderTextureLight();break;
  }


  glDrawElements(GL_TRIANGLES,nbElement_,GL_UNSIGNED_INT,nullptr);



  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBindVertexArray(0);

}


// Caution : position should be a (x,y,z) array, normal (x,y,z) array, texCoord (x,y) array. lengths should be coherent
void Revolution::initVAO(const std::vector<int> &index,const std::vector<float> &position,const std::vector<float> &normal,const std::vector<float> &texCoord) {

  vector<float> attrib;
  attrib.resize(position.size()/3*8);
  for(unsigned int i=0;i<position.size()/3;++i) {
    attrib[i*8+0]=position[i*3+0];
    attrib[i*8+1]=position[i*3+1];
    attrib[i*8+2]=position[i*3+2];

    if (!normal.empty()) {
      attrib[i*8+3]=normal[i*3+0];
      attrib[i*8+4]=normal[i*3+1];
      attrib[i*8+5]=normal[i*3+2];
    }

    if (!texCoord.empty()) {
      attrib[i*8+6]=texCoord[i*2+0];
      attrib[i*8+7]=texCoord[i*2+1];
    }
  }



  glBindBuffer(GL_ARRAY_BUFFER,vbo_);
  glBufferData(GL_ARRAY_BUFFER,int(attrib.size()*sizeof(float)),attrib.data(),GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,int(index.size()*sizeof(int)),index.data(),GL_STATIC_DRAW);
  nbElement_=int(index.size());

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER,vbo_);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),nullptr);                                   // position
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<const GLvoid *>(3*sizeof(float)));   // normal
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<const GLvoid *>(6*sizeof(float)));   // texCoord

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glBindVertexArray(0);

}

void Revolution::drawProfile() {
  p3d::ambientColor=Vector4(1,0,0,1);
  p3d::shaderVertexAmbient();

  p3d::drawLineStrip(profile_);
  p3d::drawPoints(profile_);

  vector<Vector3> mirror;
  for(auto &p:profile_) {
    mirror.push_back(Vector3(-p.x(),p.y(),p.z()));
  }

  p3d::uniformAmbient(Vector4(0,1,0,1));
  p3d::drawLineStrip(mirror);
  p3d::drawPoints(mirror);

  p3d::uniformAmbient(Vector4(0,1,0,1));
  p3d::drawLines({Vector3(0,-1,0),Vector3(0,1,0)});



}


void Revolution::interactInsert(size_t i,const p3d::Vector3 &p) {
  if (profile_.size()>0) {
    Vector3 pred=*(profile_.begin()+int(i-1));
    if ((pred-p).length()<=0.1) return;
  }
  profile_.insert(profile_.begin()+int(i),p);
}

