#ifndef HERMITE_H
#define HERMITE_H

#include "Vector3.h"
#include "InteractPosition.h"
#include <iostream>

/**
@author F. Aubert
*/

class Hermite : public p3d::InteractPosition
{
  public:
  Hermite() {}
    Hermite(const p3d::Vector3 &a,const p3d::Vector3 &ta,const p3d::Vector3 &b,const p3d::Vector3 &tb);
    void set(const p3d::Vector3 &a, const p3d::Vector3 &ta, const p3d::Vector3 &b, const p3d::Vector3 &tb);
    p3d::Vector3 eval(double t);
    void draw();
    void drawControl();

    ///////
    p3d::Vector3 *interactPoint(size_t i) override;
    size_t interactSize() override {return nbInput_;}
    void interactInsert(size_t i,const p3d::Vector3 &p) override;
    void interactUpdate(size_t i) override;


    void nbInput(size_t n)  {nbInput_=n;}
    void incNbInput() {nbInput_++;}
    size_t nbInput() {return nbInput_;}

    void a(const p3d::Vector3 &p) {a_=p;}
    void b(const p3d::Vector3 &p) {b_=p;}
    void ta(const p3d::Vector3 &p) {ta_=p;}
    void tb(const p3d::Vector3 &p) {tb_=p;}

    p3d::Vector3 a() {return a_;}
    p3d::Vector3 b() {return b_;}
    p3d::Vector3 ta() {return ta_;}
    p3d::Vector3 tb() {return tb_;}

  protected:
  private:
    p3d::Vector3 a_{};
    p3d::Vector3 b_{};
    p3d::Vector3 ta_{};
    p3d::Vector3 tb_{};

    size_t nbInput_{0};
    p3d::Vector3 interactTa_{};
    p3d::Vector3 interactTb_{};
};

#endif // HERMITE_H

