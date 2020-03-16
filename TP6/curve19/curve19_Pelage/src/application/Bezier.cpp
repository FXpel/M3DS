#include "Bezier.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "Tools.h"

#include <vector>

#include "GLDrawCamera.h"

using namespace p3d;
using namespace std;


Bezier::Bezier()
{

}


/**
* Evaluation de la Curve de Bézier en $t$
**/
Vector3 Bezier::eval(double t) {
    Vector3 result;
    if (nbPoint()>1) {
      // on recopie les points de controles dans le tableau castel (castel est donc initialisé avec la première ligne de l'algo triangulaire).
      vector<Vector3> castel=controlPts();      // tableau de points sur lesquels on applique deCasteljau

        // A COMPLETER : appliquer la méthode de De Casteljau (évolution du tableau castel)

      for(int i = 0; i < castel.size(); i++) {
                  for(int j = 0; j < castel.size() - i - 1; j++) {
                      castel[j] = (1 - t) * castel[j] + t * castel[j+1];
                  }
              }
        // le point de la courbe en t doit se trouver dans castel[0] à la fin de l'algo
        result=castel[0];
    }
    return result;
}


/**
* evalCubic est une alternative à eval mais uniquement pour les cubiques (calcul explicite plutôt qu'avec De Casteljau)
**/
Vector3 Bezier::evalCubic(double t) {
    Vector3 res=Vector3(0,0,0);
    res=t*t*t*(-point(0)+3*point(1)-3*point(2)+point(3))+
        t*t*(3*point(0)-6*point(1)+3*point(2))+
        t*(-3*point(0)+3*point(1))+
        point(0);
    return res;
}

Vector3 Bezier::evalCubicVelocity(double t) {
//  Vector3 res=Vector3(0,0,0);
  Vector3 res =3*(-point(0)+3*point(1)-3*point(2)+point(3))*pow(t,2) + 2*(3*point(0)-6*point(1)+3*point(2))*t + (-3*point(0)+3*point(1));
  return res;

}

Vector3 Bezier::evalCubicAcceleration(double t) {
  //Vector3 res=Vector3(0,0,0);
  Vector3 res = 2 * 3*(-point(0)+3*point(1)-3*point(2)+point(3))* t + 2*(3*point(0)-6*point(1)+3*point(2));
  return res;

}

Matrix4 Bezier::tbn(double t) {
  Vector3 p=evalCubic(t);
  Vector3 v=evalCubicVelocity(t);
  Vector3 a=evalCubicAcceleration(t);

  Vector3 T=v;
  T.normalize();
  Vector3 B=cross(v,a);
  B.normalize();
  Vector3 N=cross(T,B);
  N.normalize();

  return Matrix4::fromFrame(p,T,B,N);
}


Vector3 Bezier::rN(double t) {
  Vector3 tt=evalCubicVelocity(t);
  Vector3 acc=evalCubicAcceleration(t);

  Vector3 B=cross(tt,acc);


  double r=1.0/(B.length()/(tt.length()*tt.length()*tt.length()));
  tt.normalize();
  Vector3 bb=normalize(B);
  Vector3 N=cross(bb,tt);
  return r*N;
}


void Bezier::draw() {
  if (nbPoint()<2) return;


    double pas=1.0/(100.0-1);
    double t=0;

    vector<Vector3> drawPts;

    for(int i=0;i<100;i++) {
        drawPts.push_back(eval(t));
        t+=pas;
    }
    p3d::drawThickLineStrip(drawPts);
}


