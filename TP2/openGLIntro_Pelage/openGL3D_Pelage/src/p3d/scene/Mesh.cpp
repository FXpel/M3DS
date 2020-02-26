#include "Mesh.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Tools.h"
#include "Matrix4.h"
#include <fstream>

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

// modulo operator (% gives the remainder)
static size_t cycle(int i,size_t nb) {
  int nbi=static_cast<int>(nb);
  int ii=static_cast<int>(i);
  return static_cast<size_t>((ii%nbi+nbi)%nbi);
}


const Vector3 &Mesh::positionMesh(size_t i) const {return positionMesh_[i];}
const Vector3 &Mesh::normalMesh(size_t i) const {return normalMesh_[i];}
const Vector2 &Mesh::texCoordMesh(size_t i) const {return texCoordMesh_[i];}

const Vector3 &Mesh::normalFace(size_t i) const {return normalFace_[i];}

const VertexAttrib &Mesh::vertexAttrib(size_t i,int j) const {return vertexAttrib_[i][cycle(j,nbVertexFace(i))];}
size_t Mesh::indexPositionVertexFace(size_t i,int j) const {return vertexAttrib(i,j).positionIndex_;}
const Vector3 &Mesh::positionVertexFace(size_t i,int j) const {return positionMesh(indexPositionVertexFace(i,j));}
const Vector3 &Mesh::normalVertexFace(size_t i,int j) const {return normalMesh(vertexAttrib(i,j).normalIndex_);}
const Vector2 &Mesh::texCoordVertexFace(size_t i,int j) const {return texCoordMesh(vertexAttrib(i,j).texCoordIndex_);}

size_t Mesh::nbPosition() const {return positionMesh_.size();}
size_t Mesh::nbNormal() const {return normalMesh_.size();}
size_t Mesh::nbTexCoord() const {return texCoordMesh_.size();}
size_t Mesh::nbVertexFace(size_t i) const {return vertexAttrib_[i].size();}
size_t Mesh::nbFace() const {return vertexAttrib_.size();}



void Mesh::check() {
  cout << "nb position = " << positionMesh_.size() << endl;
  cout << "nb faces = " << vertexAttrib_.size() << endl;
  for(auto i=vertexAttrib_.begin();i!=vertexAttrib_.end();i++) {
    cout << "(";
    VertexAttribFace::iterator j=(*i).begin();
    cout << "v " << j->positionIndex_ << "n " << j->normalIndex_;
    j++;
    cout << ")" << endl;
  }
}

void Mesh::readInit(const string &resourceName,bool triangle) {
  read(resourceName);
  if (triangle) triangulate();
  scaleInBoxRatio(-1,1,-1,1,-1,1);
  if (normalMesh_.empty()) {cout << "no normal in OBJ file => per vertex average is computed" << endl;computeNormal();}
  if (texCoordMesh_.empty()) {cout << "no texCoord in OBJ file => set to (0,0)" << endl;computeTexCoord();}
}


void Mesh::read(const string &resourceName) {
  QFileInfo resource=p3d::resourceFile(resourceName);
  fstream file;
  file.open(resource.filePath().toStdString().c_str(),ios::in);
  file.width(20);
  if (!file.is_open()) {
    throw Error("cant load file "+resource.filePath().toStdString(),__LINE__,__FILE__);
  }

  char s[200]; // dummy to read non-interpreted line
  char read[30];
  char c;
  double x,y,z;
  unsigned int indexPosition,indexTexture,indexNormal;
  VertexAttribFace face;
  VertexAttrib vertexAttrib;

  bool readingFace=false;
  while (!file.eof()) {
    file.clear();
    file >> read;
    if (string(read).compare("v")==0) {
      file >> x >> y >> z;
      positionMesh_.push_back(Vector3(x,y,z));
      continue;
    }
    if (string(read).compare("vn")==0) {
      file >> x >> y >> z;
      normalMesh_.push_back(Vector3(x,y,z));
      continue;
    }
    if (string(read).compare("vt")==0) {
      file >> x >> y;
      texCoordMesh_.push_back(Vector2(x,y));
      continue;
    }

    if (string(read).compare("f")==0) {
      face.clear();
      readingFace=true;
      while (readingFace) {
        file >> indexPosition;
        if (!file.fail()) {
          vertexAttrib.positionIndex_=indexPosition-1; // starts at index 1 in obj file so -1 for internal arrays
          file >> c;
          if (!file.fail()) {
            if (c=='/') {
              file >> indexTexture;
              if (file.fail()) {
                file.clear();
              }
              else vertexAttrib.texCoordIndex_=indexTexture-1;
              file >> c;
              if (!file.fail()) {
                if (c=='/') {
                  file >> indexNormal;
                  vertexAttrib.normalIndex_=indexNormal-1;
                }
                else file.putback(c);
              }
            }
            else file.putback(c);
          }
          face.push_back(vertexAttrib);
        }
        else {
          readingFace=false;
        }
      }
      vertexAttrib_.push_back(face);
      continue;
    }
    file.getline(s,200);
  }
  file.close();
}

void Mesh::addPositionMesh(const p3d::Vector3 &p) {
  positionMesh_.push_back(p);
}

void Mesh::addFaceMesh(const std::vector<int> &indexList) {
  VertexAttribFace f;
  for(int i:indexList) {
    VertexAttrib v;
    v.positionIndex_=i;
    f.push_back(v);
  }
  vertexAttrib_.push_back(f);
}


void Mesh::computeNormalFace(size_t i) {
  Vector3 s1;
  Vector3 s2;
  Vector3 s3;
  Vector3 n=Vector3(0,0,0);

  double dist=0;

  size_t v1=0;
  size_t v2=1;
  size_t v3=2;
  bool stop=false;
  bool normalOk=false;
  while (!normalOk && !stop) {
    s1=positionVertexFace(i,v1);
    s2=positionVertexFace(i,v2);
    s3=positionVertexFace(i,v3);
    Vector3 u1=s2-s1;
    Vector3 u2=s3-s2;
    n=cross(u1,u2);
    dist=n.length();
    if (dist>1e-05) {
      normalOk=true;
    }
    else {
      v3++;
      if (v3==nbVertexFace(i)) {
        v2++;
        v3=v2+1;
        if (v3==nbVertexFace(i)) {
          v1++;
          v2=v1+1;
          v3=v2+1;
        }
      }
      if (static_cast<size_t>(v3)==nbVertexFace(i)) {
        stop=true;
//        throw Error("Normal problem",__LINE__,__FILE__);
      }
    }
  }

  if (stop) {
    n=Vector3(0,0,0);
  }
  else  n/=dist;
  normalFace_[i]=n;
}

void Mesh::computeNormalFace() {
  normalFace_.resize(nbFace());
  for(unsigned int i=0;i<nbFace();++i) {
    computeNormalFace(i);
  }
}


void Mesh::computeNormal() {
  computeNormalFace();
  normalMesh_.resize(nbPosition());
  vector<unsigned int> nbFaceVertex; //nbFaceVertex[i] = nbFace around vertex i
  nbFaceVertex.resize(nbPosition());
  for(unsigned int i=0;i<nbPosition();++i) {
    normalMesh_[i]=Vector3(0,0,0);
    nbFaceVertex[i]=0;
  }
  for(unsigned int i=0;i<nbFace();++i) {
    for(unsigned int j=0;j<nbVertexFace(i);++j) {
      vertexAttrib_[i][j].normalIndex_=vertexAttrib_[i][j].positionIndex_;
    }
  }
  for(unsigned int i=0;i<nbFace();++i) {
    for(unsigned int j=0;j<nbVertexFace(i);++j) {
//      cout << "normal face = " << _normalFace[i] << endl;
      normalMesh_[vertexAttrib_[i][j].normalIndex_]+=normalFace_[i];
      nbFaceVertex[vertexAttrib_[i][j].normalIndex_]++;
    }
  }
  for(unsigned int i=0;i<nbNormal();++i) {
    normalMesh_[i]/=nbFaceVertex[i];
//    cout << "normal[i] = " << _normal[i] << endl;
  }
}

void Mesh::computeTexCoord() {
  texCoordMesh_.push_back(Vector2(0,0));
  for(unsigned int i=0;i<nbFace();++i) {
    for(unsigned int j=0;j<nbVertexFace(i);++j) {
      vertexAttrib_[i][j].texCoordIndex_=0;
    }
  }
}

void Mesh::scaleInBox(double left,double right,double bottom,double top,double znear,double zfar) {
  Vector3 mini(positionMesh_[0]);
  Vector3 maxi(positionMesh_[0]);

  for(unsigned int i=1;i<nbPosition();++i) {
    mini.setMinCoordinate(positionMesh_[i]);
    maxi.setMaxCoordinate(positionMesh_[i]);
  }
  for(unsigned int i=0;i<nbPosition();++i) {
    positionMesh_[i].x((positionMesh_[i].x()-mini.x())/(maxi.x()-mini.x())*(right-left)+left);
    positionMesh_[i].y((positionMesh_[i].y()-mini.y())/(maxi.y()-mini.y())*(top-bottom)+bottom);
    positionMesh_[i].z((positionMesh_[i].z()-mini.z())/(maxi.z()-mini.z())*(zfar-znear)+znear);
  }
}

void Mesh::scaleInBoxRatio(double left,double right,double bottom,double top,double znear,double zfar) {
  Vector3 mini(positionMesh_[0]);
  Vector3 maxi(positionMesh_[0]);

  for(unsigned int i=1;i<positionMesh_.size();i++) {
    mini.setMinCoordinate(positionMesh_[i]);
    maxi.setMaxCoordinate(positionMesh_[i]);
  }

  Vector3 diag=maxi-mini;
  unsigned int which;
  double scale=diag.max(&which);
  scale=Vector3(right-left,top-bottom,zfar-znear)(which)/scale;


  for(unsigned int i=0;i<nbPosition();i++) {
    positionMesh_[i].x((positionMesh_[i].x()-mini.x())*scale+left+((right-left)-(maxi.x()-mini.x())*scale)/2.0);
    positionMesh_[i].y((positionMesh_[i].y()-mini.y())*scale+bottom+((top-bottom)-(maxi.y()-mini.y())*scale)/2.0);
    positionMesh_[i].z((positionMesh_[i].z()-mini.z())*scale+znear+((zfar-znear)-(maxi.z()-mini.z())*scale)/2.0);
  }
}

void Mesh::triangulate() {
  auto nb=nbFace();
  for(unsigned int i=0;i<nb;i++) {
    if (nbVertexFace(i)>3) {
      VertexAttribFace add;
      for(unsigned int j=0;j<nbVertexFace(i)-3;++j) {
        add.clear();
        add.push_back(vertexAttrib_[i][0]);
        add.push_back(vertexAttrib_[i][j+2]);
        add.push_back(vertexAttrib_[i][j+3]);
        vertexAttrib_.push_back(add);
      }

      vertexAttrib_[i].erase(vertexAttrib_[i].begin()+3,vertexAttrib_[i].end());
    }
  }
}

void Mesh::rotateY(double angle) {
  Matrix4 trans;
  trans=Matrix4::identity();
//  trans.translate((_aabbMax+_aabbMin)/2.0);
  trans.rotate(angle,Vector3(0,1,0));
//  trans.translate(-(_aabbMax+_aabbMin)/2.0);
  for(unsigned int i=0;i<positionMesh_.size();++i) {
    positionMesh_[i]=trans.transformPoint(positionMesh_[i]);
  }

  for(unsigned int i=0;i<nbNormal();++i) {
    normalMesh_[i]=trans.transformDirection(normalMesh_[i]); // incorrect in general case (eg scale).
  }
}


