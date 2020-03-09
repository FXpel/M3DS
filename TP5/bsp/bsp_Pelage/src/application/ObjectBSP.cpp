/**
Objet représenté sous forme BSP
(container : liste de faces, sommets, arbre)

@author Fabrice Aubert
*/


#include "ObjectBSP.h"
#include "FaceBSP.h"
#include <iostream>
#include <algorithm>
#include "GLDraw.h"
#include "GLRender.h"
#include "Mesh.h"

using namespace p3d;
using namespace std;


void ObjectBSP::set(const std::vector<FaceBSP> &face) {
  allFace_=face;
}

void ObjectBSP::consBSP() {
  bsp_=consTree(allFace_);
}


/// Construction récursive d'un arbre BSP selon une liste de facettes
NodeBSP *ObjectBSP::consTree(const std::vector<FaceBSP> &face) {
  /// Il s'agit de prendre un pivot dans le paramètre face, et de séparer l'ensemble des autres faces dans des tableaux negative et positive.
  /// puis on récursive sur negative et positive
  /// Les setters du NodeBSP res sont res->face(une_face), res->negative(un_arbre), res->positive(un_arbre)

  NodeBSP *res=nullptr;
  vector<FaceBSP> negative,positive;
  negative.clear();
  positive.clear();

  if (face.empty()) {
    return nullptr;
  } else {
    res=new NodeBSP();
    FaceBSP fPositive, fNegative;
    // TODO : A COMPLETER (construire negative et positive; mettre le pivot comme noeud de l'arbre res)
    FaceBSP pivot = face[0];
    res -> face(pivot);
    for (int i = 1;i<face.size();i++) {
        NodeBSP *f = new NodeBSP();
        FaceBSP fc = face[i];

        face[i].separe(pivot,fPositive,fNegative);

        if(f -> negative() != NULL){
            negative.push_back(fc);
        }
        if(f -> positive() != NULL){
            positive.push_back(fc);
        }
    }
    // à laisser à la fin : appels récursifs
    res->negative(consTree(negative));
    res->positive(consTree(positive));
    return res;
  }
}


/** **************************************************************** */
/** Affichage de l'arbre BSP par le peintre */

/// méthode statique récursive (appel initial fait par ObjetBSP::drawBSP(Const Vector3 &))
void ObjectBSP::drawBSP(NodeBSP *tree,const Vector3 &eye) {
  /// eye est la position de l'observateur
  /// les sélecteurs sont tree->face() (de type FaceBSP), et tree->negative(), tree->positive() (de type TreeBSP *)
  /// pour ajouter le tracé d'une face de type FaceBSP, il sufft de faire addDraw(face)

  // TODO : à compléter
    if(tree != nullptr) {
            switch(tree->face().sign(eye)) {
            case Sign_Minus:
                drawBSP(tree->positive(), eye);
                addDraw(tree->face());
                drawBSP(tree->negative(), eye);
                break;
            case Sign_Plus:
                drawBSP(tree->negative(), eye);
                addDraw(tree->face());
                drawBSP(tree->positive(), eye);
                break;
            default:
                break;
            }
        }

}

/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */



/// Affichage par peintre : init récursivité.
void ObjectBSP::drawBSP(const Vector3 &eye) {
  clearDraw();
  glDepthFunc(GL_ALWAYS);
  p3d::materialBlueGreen();
  drawBSP(bsp_,eye);
  drawArrayFill();
}

/** **************************************************************************** */





ObjectBSP::ObjectBSP() {
}


ObjectBSP::~ObjectBSP() {
  delete bsp_;
}


void ObjectBSP::read(const string &resourceName) {
//  QFileInfo name=resourceFile(resourceName);
  vector<VertexBSP> allVertex;
  Mesh o;
  o.read(resourceName);
  o.scaleInBoxRatio(-1,1,-1,1,-1,1);
  o.computeNormal();

  for(size_t i=0; i<o.nbPosition(); ++i) {
    allVertex.push_back(VertexBSP(o.positionMesh(i),o.normalMesh(i)));
  }
  for(size_t i=0; i<o.nbFace(); ++i) {
    FaceBSP f;
    f.normal(o.normalFace(i));
    for(size_t j=0; j<o.nbVertexFace(i); ++j) {
      f.add(allVertex[o.indexPositionVertexFace(i,int(j))]);
    }
    allFace_.push_back(f);
  }
  cout << "NB FACE READ : " << allFace_.size() << endl;
}

void ObjectBSP::drawNormal() {
    vector<Vector3> pts;
    for(auto &f:allFace_) {
      for(size_t i=0;i<f.nbVertex();++i) {
        pts.push_back(f.position(i));
        pts.push_back(f.position(i)+f.normal(i));
      }
    }
    p3d::ambientColor=Vector4(1,0,0,1);
    p3d::shaderVertexAmbient();
    p3d::drawLines(pts);

}


void ObjectBSP::drawDepth() {
  p3d::materialFrontBack();
  glDepthFunc(GL_LESS);
  p3d::shaderLightPhong();
  drawGrid();

}


void ObjectBSP::clearDraw() {
  drawNormalFill_.clear();
  drawPtsFill_.clear();
  drawPtsWire_.clear();
}

void ObjectBSP::drawArrayGrid() {
  drawArrayFill();
  p3d::fragOffset=0.01f;
  p3d::shaderVertexAmbient();
  p3d::uniformAmbient(Vector4(0,0,0,1));
  glLineWidth(2);
  p3d::drawLines(drawPtsWire_);
  p3d::fragOffset=0;


}

void ObjectBSP::drawArrayFill() {
  p3d::materialFrontBack();
  p3d::ambientColor=Vector4(0.1,0.1,0.1,0.5);
  p3d::shaderLightPhong();
  p3d::draw(GL_TRIANGLES,drawPtsFill_,"position",drawNormalFill_,"normal");
}



void ObjectBSP::drawGrid() {
  drawArrayGrid();
}


void ObjectBSP::drawBrut() {
  p3d::materialFrontBack();
  glDepthFunc(GL_ALWAYS);
  drawGrid();
}


void ObjectBSP::addDrawFill(const FaceBSP &f) {
  if (f.size()!=0) {
    for(size_t i=1; i<f.size()-1; ++i) {
      drawPtsFill_.push_back(f.position(0));
      drawNormalFill_.push_back(f.normal(0));
      drawPtsFill_.push_back(f.position(i));
      drawNormalFill_.push_back(f.normal(i));
      drawPtsFill_.push_back(f.position(i+1));
      drawNormalFill_.push_back(f.normal(i+1));
    }
  }
}

void ObjectBSP::addDrawWire(const FaceBSP &f) {
  if (f.size()!=0) {
    drawPtsWire_.push_back(f.position(0));
    for(size_t i=1; i<f.size(); ++i) {
      drawPtsWire_.push_back(f.position(i));
      drawPtsWire_.push_back(f.position(i));
    }
    drawPtsWire_.push_back(f.position(0));
  }
}


void ObjectBSP::addDrawAllInit() {
  for(auto &f:allFace_) {
    addDraw(f);
  }
}

void ObjectBSP::addDrawAllTree() {
  addDraw(bsp_);
}


void ObjectBSP::addDraw(const FaceBSP &f) {
  addDrawWire(f);
  addDrawFill(f);

}

void ObjectBSP::addDraw(NodeBSP *n) {
  addDrawFill(n);
  addDrawWire(n);
}

void ObjectBSP::addDrawFill(NodeBSP *n) {
  if (n) {
    addDrawFill(n->face());
    addDrawFill(n->negative());
    addDrawFill(n->positive());
  }
}

void ObjectBSP::addDrawWire(NodeBSP *n) {
  if (n) {
    addDrawWire(n->face());
    addDrawWire(n->negative());
    addDrawWire(n->positive());
  }
}


size_t ObjectBSP::nbFaceInit() const {
  return allFace_.size();
}

size_t ObjectBSP::nbFaceBSP() const {
  return bsp_->nbFace();
}


