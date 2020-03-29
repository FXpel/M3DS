#pragma once

#include "Vector3.h"
#include "Texture.h"
/**
@file
@author F. Aubert
@brief Particle

*/

class Particle {
  public:
    Particle();
    virtual ~Particle();


    p3d::Vector3 position() const;
    p3d::Vector3 velocity() const;

    void position(const p3d::Vector3 &p);
    void velocity(const p3d::Vector3 &v);

    void addForce(const p3d::Vector3 &f);
    void resetForce();

    void addVelocityCorrec(const p3d::Vector3 &v);
    void addPositionCorrec(const p3d::Vector3 &p);

    void velocityCorrection();
    void positionCorrection();

    void color(double r,double g,double b) {color_=p3d::Vector3(r,g,b);}

    p3d::Vector3 force();

    void mass(double m);
    double mass() const;


    void drawSphere() const;
    void drawPoint() const;

    bool isAlive() const;
    void initRandom();
    void birth();
    void updateLife(double deltaTime);
    void kill();

    void timeToLive(double t);
    double timeToLive();
    double radius();
    void radius(double r);

  private:
    p3d::Vector3 position_;
    p3d::Vector3 velocity_;

    p3d::Vector3 positionCorrection_;
    p3d::Vector3 velocityCorrection_;

    p3d::Vector3 color_;

    double mass_;

    p3d::Vector3 cumulForce_;


    bool isAlive_;
    double timeLeft_;
    double birthDate_;

    double radius_;

};


