#pragma once
#include <chrono>
#include <memory>

#include "BoxList.h"
#include "Plane.h"

/**
@file
@author F. Aubert
@brief opérations sur OBB pour simu physique (collision+impulsion)

*/

class DistanceInfo;
class Collision;

class EngineBox {
  public:
    EngineBox();
    virtual ~EngineBox();

    void startTime();
    double currentTime();
    double deltaTime();
    double deltaTimeSimule();
    void factorTime(double f);
    void mulFactorTime(double f);

    void boxList(const BoxList &b) {boxList_=b;}
    BoxList &boxList() {return boxList_;}
    void setBoxTexture(size_t i,std::string filename);


    void update();
    void euler(double dt);


    void computeForce();
    void collisionPlane();
    void interCollision();
    void applyCorrection();


    void addVelocityCorrec(const p3d::Vector3 &v);
    void addPositionCorrec(const p3d::Vector3 &p);
    void addAngularVelocityCorrec(double w);

    void velocityCorrection();
    void positionCorrection();
    void angularVelocityCorrection();

    void activeCursor(const p3d::Vector3 &p);
    void motionCursor(const p3d::Vector3 &p);
    void disableCursor();

    void add(const p3d::Plane &p);
    void add(const Box &b);

    void draw();
    void drawPlane2D();

    bool isCursorActive();

    static DistanceInfo distance(const Box &b1, const Box &b2, const p3d::Vector3 &axe);
    static Collision detectCollision(const Box &b1, const Box &b2);


protected:
  private:

    std::chrono::time_point<std::chrono::steady_clock> startTime_;
    double previousTime_{0};
    double factorTime_{1};

    BoxList boxList_{};

    bool cursorActive_{};
    p3d::Vector3 cursor_{};

    p3d::Vector3 forceInsert_{};

    std::vector<p3d::Plane> planeList_{};
    std::vector<std::unique_ptr<p3d::Texture>> texture_;


};


