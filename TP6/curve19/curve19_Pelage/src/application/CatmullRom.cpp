#include "GLDrawCamera.h"
#include "CatmullRom.h"
#include "Vector3.h"
#include "Tools.h"

#include "Curve.h"
#include "Bezier.h"

#include "Matrix4.h"
#include "GLTool.h"

/**
@file
@author F. Aubert
*/


using namespace p3d;
using namespace std;


/**
* Tracé de la courbe d'interpolation
*/

void CatmullRom::draw() {


  // A COMPLETER :
  //
  // doit tracer une courbe en reliant 2 points saisis successifs par une bézier cubique
  // nbPoint() vous donne le nombre de points saisis
  // on doit donc tracer nbPoint()-1 courbes de bézier cubiques : la ième courbe a pour points de controle point(i),intermediate(i,0),intermediate(i,1) et point(i+1)
  // il suffit donc d'affecter la variable "cubic" avec ces points :
  // => affectez les points de contrôle de cubic avec le setter cubic.point(j,un_point)
  // puis de la tracer (en appelant simplement cubic.draw() ).


  Bezier cubic;
  cubic.resize(4); // Bézier cubique = 4 points de contrôle

  if (nbPoint()<2) return; // tracé uniquement si plus de 2 points saisis.

  for(int i = 0; i < nbPoint() -1; i++) {
        cubic.point(0, point(i));
        cubic.point(1, intermediate(i,0));
        cubic.point(2, intermediate(i,1));
        cubic.point(3, point(i + 1));
        cubic.draw();
    }



}


/**
*  Calcul des points intermédiaires pour obtenir une continuité C1 par Catmull-Rom
*/
void CatmullRom::setup(bool close) {
  Vector3 parallele;

  // A COMPLETER : il s'agit d'initialiser les points intermédiaires (les points bleus) en appelant les setters intermediate(i,0,un Vector3) et intermediate(i,1,un Vector3) )
  // les points intermédiaires doivent être fixés par la méthode vue en cours (tangentes parallèles aux segments [point(i-1),point(i+1)]).
  if(!close && nbPoint() >=4)//bug si le nombre de point est inférieur à 4
  {
      float k = 0.5;
      for(unsigned int i=1;i<nbPoint() - 1;i++) {
          parallele = point(i + 1) - point(i - 1);

          intermediate(i - 1, 1, point(i) - (k * parallele) / 2);
          intermediate(i, 0, point(i) + (k * parallele) / 2);
      }
  }

}

Matrix4 CatmullRom::tbn(double tValue) {
  double tCubic;
  Bezier c=segment(tValue,&tCubic);

  Vector3 p=c.evalCubic(tCubic);
  Vector3 v=c.evalCubicVelocity(tCubic);
  Vector3 a=c.evalCubicAcceleration(tCubic);

  Vector3 t=v;
  Vector3 b=cross(v,a);
  t.normalize();
  b.normalize();
  Vector3 n=cross(b,t);


  return Matrix4::fromFrame(p,t,b,n);
}


/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */


void CatmullRom::resize(size_t nb) {
  Curve::resize(nb);
  _intermediate.resize(2*(nb-1));
}

void CatmullRom::interactInsert(size_t i, const Vector3 &insertP) {
    Curve::interactInsert(i/3+i%3,insertP);
    addIntermediate();
}


const Vector3 &CatmullRom::intermediate(size_t i,size_t j) const {
  return _intermediate[2*i+j];
}

void CatmullRom::intermediate(size_t i,size_t j,const Vector3 &p) {
  _intermediate[2*i+j]=p;
}

void CatmullRom::addIntermediate() {
  if (nbPoint()<2) return;
  size_t i=nbPoint()-2;

  if (_intermediate.size()!=(nbPoint()-2)*2)
    throw ErrorD("incoherence between pts and intermediate");

  Vector3 p1=point(i)+0.33*(point(i+1)-point(i));
  Vector3 p2=point(i)+0.66*(point(i+1)-point(i));
  _intermediate.push_back(p1);
  _intermediate.push_back(p2);
}




void CatmullRom::drawTBN(double t) {

 p3d::drawFrame(this->tbn(t),"",0.5);
}


void CatmullRom::drawPoint() {
  vector<Vector3> allPts;

  for(size_t i=0; i<nbPoint(); i++) {
    allPts.push_back(point(i));
    p3d::draw(int(i),point(i)+Vector3(0.02,0.02,0.0));
  }
  p3d::shaderVertexAmbient();
  p3d::draw(GL_POINTS,allPts);
}

vector<Vector3> *CatmullRom::intermediatev() {
  return &_intermediate;
}

void CatmullRom::drawControl(int mode) {
  if (nbPoint()>0) {
    if (mode>0) {
      p3d::uniformAmbient(1,0,0);
      if (mode==1) {
        drawPoint();
      }
      if (mode>1) {
        Curve::drawControl();
        if (mode>2) {
          p3d::uniformAmbient(0,0,1);
          for(unsigned int i=0; i<nbPoint()-1; i++) {
            p3d::drawLines(vector<Vector3>{point(i),intermediate(i,0),
                            intermediate(i,0),intermediate(i,1),
                            intermediate(i,1),point(i+1)});
            p3d::drawPoints(vector<Vector3>{intermediate(i,0),intermediate(i,1)});
          }
        }
      }
    }
  }
}

Bezier CatmullRom::segment(size_t i) {
  Bezier cubic;
  cubic.resize(4);

  cubic.point(0,point(i));
  cubic.point(1,intermediate(i,0));
  cubic.point(2,intermediate(i,1));
  cubic.point(3,point(i+1));

  return cubic;
}


Bezier CatmullRom::segment(double t,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  double realT=fracpart;
  size_t whichCurve=size_t(intpart);


  Bezier cubic;
  cubic.resize(4);

  cubic.point(0,point(whichCurve));
  cubic.point(1,intermediate(whichCurve,0));
  cubic.point(2,intermediate(whichCurve,1));
  cubic.point(3,point(whichCurve+1));

  *tCubic=realT;

  return cubic;
}



void CatmullRom::whichSegment(double t,size_t *cubicNumber,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  *tCubic=fracpart;
  *cubicNumber=size_t(intpart);
}



Vector3 CatmullRom::eval(double t) {
  double tCubic;
  Bezier c=segment(t,&tCubic);
  return c.evalCubic(tCubic);
}



