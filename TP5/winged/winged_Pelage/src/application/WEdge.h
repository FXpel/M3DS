#pragma once

/**
@file
@author F. Aubert


*/


class WVertex;
class WFace;
class Winged;

#include "Vector3.h"

class WEdge
{
  public:
    WEdge(WVertex *v1,WVertex *v2,Winged *prop);
    virtual ~WEdge();

    size_t index();
    void index(size_t i);

    Winged *owner();

    WVertex *begin() const;
    WVertex *end() const;
    void begin(WVertex *v);
    void end(WVertex *v);

    WFace *left() const;
    WFace *right() const;
    void left(WFace *f);
    void right(WFace *f);

    WEdge *succLeft() const;
    WEdge *succRight() const;
    WEdge *predLeft() const;
    WEdge *predRight() const;

    void succLeft(WEdge *e);
    void succRight(WEdge *e);
    void predLeft(WEdge *e);
    void predRight(WEdge *e);

    void draw();

    void computeVector();
    void computePointEdge();

    const p3d::Vector3 &vec() const;
    p3d::Vector3 direction();
    const p3d::Vector3 &pointEdge() const;
    p3d::Vector3 midPoint();

    void insertVertexPointEdge();

    void mark();
    void unmark();
    bool isMark();

    void succLine(WEdge *e);
    WEdge *succLine();

    void followBoundary();
    void drawLine();

    friend std::ostream& operator <<(std::ostream &s,const WEdge &e);



  protected:
  private:
    WVertex *begin_{nullptr};
    WVertex *end_{nullptr};
    WFace *left_{nullptr};
    WFace *right_{nullptr};
    WEdge *succLeft_{nullptr},*succRight_{nullptr},*predLeft_{nullptr},*predRight_{nullptr};

    bool mark_{false};

    WEdge *succLine_{nullptr};

    Winged *owner_{nullptr};

    p3d::Vector3 vec_{};
    p3d::Vector3 pointEdge_{};

    size_t index_=0;
};




