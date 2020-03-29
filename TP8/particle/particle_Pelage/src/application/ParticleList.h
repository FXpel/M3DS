#pragma once

#include "Particle.h"
#include "Texture.h"
#include <vector>
#include <list>

enum EParticleMode {Particle_Point,Particle_Sphere};

class ParticleList {
  public:
    ParticleList();

    void texture(p3d::Texture *t);
    void initState(Particle *p);
    void maxi(unsigned int nb);

    void computeForce();

    void euler(double dt);

    void draw();
    void particleMode(EParticleMode mode) {particleMode_=mode;}
    EParticleMode particleMode() {return particleMode_;}


    void updateLife(double deltaTime);

    void birthRate(double nb);

    void drawData();

    void link(size_t i,size_t j);

    size_t nbAlive();

    void startTime();

    Particle &operator[](size_t i) {return particle_[i];}

    size_t size() {return particle_.size();}

  protected:
  private:

    std::vector<Particle> particle_{};
    std::list<Particle *> deadParticle_{};
    double remainingBirth_{0};
    double nbBirthPersec_{0};

  p3d::Texture *texture_{nullptr};

  EParticleMode particleMode_{Particle_Point};
};


