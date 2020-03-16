#include "Hermite.h"

#include <iostream>
#include <sstream>
#include "GLTool.h"

/**
@file
@author F. Aubert
*/


using namespace p3d;
using namespace std;

/**
* Evaluation de la courbe de hermite P(t) :
* - _a,_b,_ta et _tb sont les données géométriques (points extrémités et tangentes).
* - on peut utiliser les opérateurs *, + entre les points et des doubles. Exemple : p=matrix[i]*_a+_b, ...
**/
Vector3 Hermite::eval(double t) {
  // remplacer la matrice 4x4 identité suivante avec les valeurs correctes pour une courbe de Hermite (matrice de hermite => cf cours)

  double matrix[16]={2.0,-2.0,1.0,1.0,
                     -3.0,3.0,-2.0,-1.0,
                     0.0,0.0,1.0,0.0,
                     1.0,0.0,0.0,0.0};

  Vector3 res(0,0,0);
  // A COMPLETER : calculer le point P(t) en utilisant les 16 coefficients de la matrice (i.e. coder explicitement le produit (t^3 t^2 t 1) matrix G : cf cours; les classes du squelette ne permettent pas de le faire directement).

  double t3 = pow(t, 3.0);
  double t2 = pow(t, 2.0);


  double c1 = matrix[0]*t3 + matrix[4]*t2 + matrix[8]*t + matrix[12]*1;
  double c2 = matrix[1]*t3 + matrix[5]*t2 + matrix[9]*t + matrix[13]*1;
  double c3 = matrix[2]*t3 + matrix[6]*t2 + matrix[10]*t + matrix[14]*1;
  double c4 = matrix[3]*t3 + matrix[7]*t2 + matrix[11]*t + matrix[15]*1;

  res = c1*a_ + c2*b_ + c3*ta_ + c4*tb_;
  return res;

  return res;
}

/**
* Trace la courbe de hermite (100 points)
**/
void Hermite::draw() {
    vector<Vector3> lPoints;

    // A COMPLETER : calculer 100 (par exemple) points successifs de la courbe pour décrire la courbe de hermite
    // on insère les points calculés dans le tableau lPoints par des lPoints.push_back(unVector3) (lPoints sera alors tracé à la fin de la méthode par l'appel, déjà présent, à p3d::drawThickLineStrip ).

    for (double i = 1; i < 100; ++i) {
            lPoints.push_back(this->eval(i/100.0));
        }
    p3d::drawThickLineStrip(lPoints);

}


/** **************************************************************************************** */
Hermite::Hermite(const Vector3 &a, const Vector3 &ta, const Vector3 &b, const Vector3 &tb) {
  a_=a;
  b_=b;
  ta_=ta;
  tb_=tb;

  nbInput_=0;
}

p3d::Vector3 *Hermite::interactPoint(size_t i) {
    switch(i) {
    case 0:return &a_;
    case 2:return &b_;
    case 1:return &interactTa_;
    case 3:return &interactTb_;
    default:return nullptr;
    }
}

void Hermite::interactUpdate(size_t i) {
    if (i==1) ta_=(interactTa_-a_)*5;
    if (i==3) tb_=(interactTb_-b_)*5;
    if (i==0) interactTa_=(a_+ta_/5);
    if (i==2) interactTb_=(b_+tb_/5);
}

void Hermite::interactInsert(size_t i, const Vector3 &p) {

  Vector3 p2=p;
    switch(i) {
    case 0:a_=p2;break;
    case 2:b_=p2;break;
    case 1:interactTa_=p2;break;
    case 3:interactTb_=p2;break;
    case 4:nbInput_=0;a_=p2;break;
    default:break;
    }
    nbInput_++;
}

void Hermite::set(const Vector3 &a,const Vector3 &ta,const Vector3 &b,const Vector3 &tb) {
  a_=a;
  b_=b;
  ta_=ta;
  tb_=tb;
}

void Hermite::drawControl() {
  p3d::diffuseColor=Vector3(0,0,1);
  p3d::ambientColor=Vector4(0,0,1,1);
  glPointSize(5);
  switch(nbInput()) {
  case 4:
    p3d::shaderLightPhong();
    p3d::drawArrow(b(),tb()/5,0.01,"","T_B");
    [[fallthrough]];
  case 3:
    p3d::draw("B",b()+Vector3(0.02,0.02,0.0));
    p3d::shaderVertexAmbient();
    p3d::drawPoints(vector<Vector3>{b()});
    [[fallthrough]];
  case 2:
    p3d::shaderLightPhong();
    p3d::drawArrow(a(),ta()/5,0.01,"","T_A");
    [[fallthrough]];
  case 1:
    p3d::draw("A",a()+Vector3(0.01,0.01,0.0));
    p3d::shaderVertexAmbient();
    p3d::drawPoints(vector<Vector3>{a()});
    break;
  }
}



