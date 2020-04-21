#include "Box.h"
#include "Texture.h"

#include "GLDraw.h"
#include "GLRender.h"
#include "GLPrimitive.h"
#include "GLDrawCamera.h"

#include "AngleAxis.h"
#include "Tools.h"

/**
@file
@author F. Aubert
@brief opérations sur OBB pour mini simu physique (collision+impulsion)

*/

using namespace p3d;

using namespace std;



/** Indique si le point p, donné dans le repère du monde appartient à la boite
- Principe : on transforme dans le repère local et on teste dans le repère local (la définition de la boite étant naturelle dans ce repère).
- this->width() et this->height() donnent la largeur et hauteur de la boite.
**/

bool Box::isInside(const Vector3 &p) const {
  bool res;
  Vector3 v=this->toLocal(p);
  res=((v.x()<width_/2.0) && (v.y()<height_/2.0) && (v.x()>-width_/2.0) && (v.y()>-height_/2.0));
  return res;
}


/** Permet d'obtenir le point d'attache du ressort dans le repère du monde
**/
Vector3 Box::attachWorld() {
  return toWorld(attach_);
}



/** Projete les 4 sommets de la boite sur l'axe axe. Donne l'intervalle [*d,*f] de la projection (*d<*f)
  - this->vertex(i) : donne le i-ème sommet de la boite (dans le repère du monde)
  - il faut déterminer le minimum (dans kmin) et le maximum (dans kmax) des projections des 4 sommets pour déterminer l'intervalle de la boite sur l'axe
**/
Interval Box::project(const Vector3 &axe) const {
  Interval res;
  double kmin=0.0,kmax=0.0;
  // A compléter : l'intervalle [kmin,kmax] doit correspondre à la projection de la boite sur l'axe
  double k=0;
  for(int i = 0; i<4; i++){
    Vector3 sommet = this->vertex(i);
    k = dot(sommet,axe);
    if(i == 0){
        kmin = k;
        kmax = k;
    }
    else {
        kmin = min(kmin,k);
        kmax = max(kmax,k);
   }
  }

  res.inf_=kmin;
  res.sup_=kmax;

  return res;
}




/** calcule la distance de recouvrement des boites b1 et b2 sur l'axe : le resultat sera mis dans *distance
  le résultat *direction sera utilisé dans la question suivante, mais il faut l'affecter ici :
    - *direction = 1 si pour séparer b2 de b1 il faut bouger b2 dans le sens de u
    - *direction = -1 si pour séparer b2 de b1 il faut bouger b2 dans le sens opposé de u
**/






/** **************************************************************************************************************************** **/

Box::Box() {
}



void Box::computeInertia() {
  inertia_=1.0/12.0*mass_*(width_*width_+height_*height_);
}



void Box::dimension(double w,double h) {
  width_=w;
  height_=h;
  computeInertia();
}


void Box::addForce(const Vector3 &f) {
  cumulForce_+=f;
}

void Box::addMoment(const Vector3 &m) {
  cumulMoment_+=m;
}

void Box::addMoment(const Vector3 &f,const Vector3 &p) {
  addMoment(computeMoment(f,p));
}



void Box::enableVisualEffect(unsigned int i) {
  visualEffect_=visualEffect_ | i;
}

void Box::disableVisualEffect(unsigned int i) {
  visualEffect_=visualEffect_ & ~i;
}


void Box::draw() const {
  texture_->bind(0);
  p3d::modelviewMatrix.push();
  p3d::modelviewMatrix.translate(position_);
  p3d::modelviewMatrix.rotate(theta_*180.0/M_PI,0,0,1);
  p3d::modelviewMatrix.scale(width_/2.0,height_/2.0,1);
  p3d::ambientColor=Vector4(1,1,1,1);
  p3d::shaderTextureAmbient();
  p3d::drawSquare();


  if (visualEffect_>0) {
    glDepthFunc(GL_ALWAYS);
    if (visualEffect_ & 1) p3d::ambientColor=Vector4(1,0,0,1);
    else p3d::ambientColor=Vector4(color_,1);
    p3d::drawThickLineStrip({Vector3(-1,-1,0),Vector3(-1,1,0),Vector3(1,1,0),Vector3(1,-1,0),Vector3(-1,-1,0)});
    glDepthFunc(GL_LESS);
  }

  p3d::modelviewMatrix.pop();
}



Vector3 Box::computeMoment(const Vector3 &f,const Vector3 &a) {
  return cross(f,position()-a);
}


void Box::addVelocityCorrec(const Vector3 &v) {
  vCorrec_+=v;
}

void Box::addPositionCorrec(const Vector3 &p) {
  pCorrec_+=p;
}

void Box::addOmegaCorrec(const Vector3 &w) {
  wCorrec_+=w;
}

void Box::omegaCorrection() {
  omega_+=wCorrec_;
  wCorrec_=Vector3(0,0,0);
}

void Box::velocityCorrection() {
  velocity_+=vCorrec_;
  vCorrec_=Vector3(0,0,0);
}

void Box::positionCorrection() {
  position_+=pCorrec_;
  pCorrec_=Vector3(0,0,0);
}

void Box::attachWorld(const Vector3 &p) {
  attach_=toLocal(p);
}

const Vector3 &Box::attachLocal() {
  return attach_;
}



Vector3 Box::vertex(size_t i) const {
  Vector3 res;
  switch(i) {
  case 0:
    res=toWorld(Vector3(-width_/2.0,-height_/2.0,0));
    break;
  case 1:
    res=toWorld(Vector3(width_/2.0,-height_/2.0,0));
    break;
  case 2:
    res=toWorld(Vector3(width_/2.0,height_/2.0,0));
    break;
  case 3:
    res=toWorld(Vector3(-width_/2.0,height_/2.0,0));
    break;
  default:
    throw ErrorD("Box::Vertex");
  }
  return res;
}



Vector3 Box::directionX() const {
  Vector3 res(1,0,0);
  res=AngleAxis(this->theta()*180.0/M_PI,Vector3(0,0,1)).rotate(res);
  return res;
}

Vector3 Box::directionY() const {
  Vector3 res(0,1,0);
  res=AngleAxis(this->theta()*180.0/M_PI,Vector3(0,0,1)).rotate(res);
  return res;
}





/** Idem que toLocal mais on donne un point dans le repère local pour obtenir ses coordonnées dans le repère du monde (i.e. res= M_W->L*p)
**/

Vector3 Box::toWorld(const Vector3 &p) const {
  Vector3 res;
  Matrix4 mat=Matrix4::identity(); // matrice de passage W->L

  mat.translate(position());
  mat.rotate(theta()*180.0/M_PI,Vector3(0,0,1));

  res=mat.transformPoint(p);

  return res;
}

/** Transforme les coordonnées d'un point données dans le repère du monde pour les obtenir dans le repère local de la boite
(i.e. res=M_L->W * p avec M_L->W matrice de passage de Local à World)
- la position du centre de la boite est donnée par this->position()
- l'angle de rotation par rapport au centre de la boite est donné par this->theta()
- vous pouvez utiliser le type Matrix4 pour calculer la matrice M_W->L :
  - mat.setIdentity() => affecte à l'identité
  - mat.translate(un Vector3 ou un Vector3) => multiplie à droite par la matrice de translation
  - mat.rotate(un angle (en degrés ! attention à la conversion), un Vector3) => multiplie à droite par la matrice de rotation
  - un Vector3 p'= mat.transform(un Vector3 p) => affecte p' avec mat*p
**/

Vector3 Box::toLocal(const Vector3 &p) const {
  Vector3 res;
  Matrix4 mat=Matrix4::identity();

  mat.rotate(-theta()*180.0/M_PI,Vector3(0,0,1));
  mat.translate(-position());
  res=mat.transformPoint(p);
  return res;
}

void Box::texture(Texture *t) {
  texture_=t;

}


