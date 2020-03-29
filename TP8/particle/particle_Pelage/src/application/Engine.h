#pragma once

#include "ParticleList.h"
#include "Plane.h"
#include <chrono>
/**
@file
@author F. Aubert
@brief Basic Particle Engine

*/


class Engine
{
  public:
    Engine();

    void initParticle(EParticleMode mode);

    void computeForce();
    void euler(double dt);

    void draw();

    void run();
    void update();

    void updatePositionVelocityCollision();

    size_t nbAlive();

    void addCollisionPlane(const p3d::Plane &p);
    void collisionPlane();
    void interCollision();

    double computeImpulse(const Particle &p1, const Particle &p2,const p3d::Vector3 &n, double restitution);


    void resetForce();

    void enableWind(const p3d::Line &ray);
    void disableWind();

    double currentTime();
    double deltaTime();
    void startTime();
    void factorTime(double f);
    void mulFactorTime(double f);
    double deltaTimeSimule();
    void texture(p3d::Texture *t);
protected:
  private:
    ParticleList particleList_{};
    std::vector<p3d::Plane> planeList_{};

    std::chrono::time_point<std::chrono::steady_clock> startTime_;
    double previousTime_{0};
    double factorTime_{1};

    p3d::Line wind_{};
    bool windEnable_{false};
};


