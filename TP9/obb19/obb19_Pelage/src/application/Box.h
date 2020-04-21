#pragma once
/**
@file
@author F. Aubert
@brief opérations sur OBB pour mini simu physique (collision+impulsion)

*/


#include "Vector3.h"

class Interval {
public:
  double inf_;
  double sup_;
};

namespace p3d {
class Vector3;
class Texture;
}

class Box
{
  public:
    Box();

    void position(const p3d::Vector3 &p) {position_=p;}
    void velocity(const p3d::Vector3 &v) {velocity_=v;}

    void theta(double a) {theta_=a;}
    void omega(const p3d::Vector3 &w) {omega_=w;}

    void mass(double m) {mass_=m;}
    void dimension(double w,double h);
    void computeInertia();
    p3d::Vector3 computeMoment(const p3d::Vector3 &vect,const p3d::Vector3 &application);

    void texture(p3d::Texture *t);

    const p3d::Vector3 &position() const {return position_;}
    const p3d::Vector3 &velocity() const {return velocity_;}
    double mass() const {return mass_;}
    double width() const {return width_;}
    double height() const {return height_;}
    double theta() const {return theta_;}
    const p3d::Vector3 &omega() const {return omega_;}
    double inertia() const {return inertia_;}

    void enableVisualEffect(unsigned int i);
    void disableVisualEffect(unsigned int i);

    void addForce(const p3d::Vector3 &f);
    void addMoment(const p3d::Vector3 &f,const p3d::Vector3 &application);
    void addMoment(const p3d::Vector3 &m);

    const p3d::Vector3 &force() const {return cumulForce_;}
    const p3d::Vector3 &moment() const {return cumulMoment_;}

    void resetForce() {cumulForce_=p3d::Vector3(0,0,0);}
    void resetMoment() {cumulMoment_=p3d::Vector3(0,0,0);}

    void addVelocityCorrec(const p3d::Vector3 &v);
    void addPositionCorrec(const p3d::Vector3 &p);
    void addOmegaCorrec(const p3d::Vector3 &w);

    void velocityCorrection();
    void positionCorrection();
    void omegaCorrection();


    p3d::Vector3 vertex(size_t i) const;

    p3d::Vector3 toWorld(const p3d::Vector3 &p) const;
    p3d::Vector3 toLocal(const p3d::Vector3 &p) const;

    void draw() const;

    void attachWorld(const p3d::Vector3 &p);
    const p3d::Vector3 &attachLocal();

    p3d::Vector3 attachWorld();



    Interval project(const p3d::Vector3 &axis) const;
    void distance(Box &b1,Box &b2,const p3d::Vector3 &axis);
    inline void color(const p3d::Vector3 &c) {color_=c;}
    inline const p3d::Vector3 &color() const {return color_;}

    // axes x et y de la boite
    p3d::Vector3 directionX() const;
    p3d::Vector3 directionY() const;

    bool isInside(const p3d::Vector3 &p) const;


  protected:
  private:

  // placement boite
    p3d::Vector3 position_{};
    double theta_{0.0};
    double width_{0},height_{0};

  // propriétés physiques
    double mass_{0};
    double inertia_{0};
    p3d::Vector3 velocity_{};


  // simulation
    p3d::Vector3 omega_{};
    p3d::Vector3 cumulForce_{};
    p3d::Vector3 cumulMoment_{};

    p3d::Vector3 vCorrec_{};
    p3d::Vector3 pCorrec_{};
    p3d::Vector3 wCorrec_{};

    p3d::Vector3 color_{};
    p3d::Texture *texture_; // Box cant be the owner (no copy semantic for p3d::Texture)


    unsigned int visualEffect_{0};

    // application force
    p3d::Vector3 attach_{};

};


