#pragma once

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

#include "Quaternion.h"
#include <stack>
#include "Vector3.h"
#include "Line.h"

namespace p3d {

  typedef enum {Coordinate_Local,Coordinate_World} ECoordinate;

  class Object3D {


    public:
    Object3D();

    Matrix4 worldLocal() const;
    Matrix4 localWorld() const;

    void compose(const Object3D &f);

    void translate(const Vector3 &tr,ECoordinate f=Coordinate_World);
    void translate(double x,double y,double z,ECoordinate f=Coordinate_World) {translate(Vector3(x,y,z),f);} // TODO : should be the right way... (Vector3 => x,y,z)
    void rotate(double angle,const Vector3 &axe,ECoordinate f=Coordinate_Local);
    void rotate(double angle,double x,double y,double z,ECoordinate f=Coordinate_Local);
    void rotate(double angle, const Vector3 &axe, const Vector3 &center,ECoordinate f=Coordinate_Local);
    void rotate(const Quaternion &q,ECoordinate f=Coordinate_Local);
    //! scale is always considered in LOCAL coordinate
    void scale(const Vector3 &s/*,EMovable f=LOCAL_COORDINATE*/) {scale_=s;}
    void scale(double x,double y,double z/*,EMovable f=LOCAL_COORDINATE*/) {scale(Vector3(x,y,z));}
    void scale(double k) {scale(k,k,k);}

    void setIdentity();

    inline const Vector3 &position() const {return position_;}
    inline const Quaternion &orientation() const {return orientation_;}
    inline const Vector3 &scale() const {return scale_;}

    inline void position(const Vector3 &pos) {position_=pos;}
    inline void position(double x,double y,double z) {position_=Vector3(x,y,z);}
    inline void orientation(const Quaternion &q) {orientation_=q;}
    inline void scaling(const Vector3 &s) {scale_=s;}
    inline void scaling(double k) {scale_=Vector3(k,k,k);}

    void orientation(const Vector3 &i,const Vector3 &j,const Vector3 &k);
    void orientation(double *a,Vector3 *u);
    void orientation(double a,const Vector3 &u);

    // linear interpolation this=(1-t)f1+tf2 (i.e. linear interpolation of the translation and the quaternion)
    void mix(const Object3D &f1,const Object3D &f2,double t);

    // returns the point/direction in LOCAL (f=LOCAL_COORDINATE and then u supposed to be given in World) or in WORLD (f=WORLD_COORDINATE and then u supposed to be given in LOCAL)
    Vector3 pointTo(ECoordinate f,const Vector3 &u) const;
    Vector3 directionTo(ECoordinate f,const Vector3 &u) const;

    //! get the direction (i.e. the z axis) in the *WORLD* coordinate system
    p3d::Vector3 direction() const;
    //! set the direction (i.e. the z axis) with u expressed in the *WORLD* coordinate system. Caution : you may want use lookAt instead.
    void direction(const Vector3 &u);

    //! set the orientation such that the z direction point to pos (pos is expressed in *WORLD*).
    void lookAt(const Vector3 &pos,const Vector3 &up=Vector3(0,1,0));


    void lineTo(ECoordinate f, Line *l);
    Line lineTo(ECoordinate f, const Line &l);

    //! rotate this coordinate system around the *WORLD* axes (y then x). center is assumed to be in *WORLD* coordinates
    void rotateXY(double ax, double ay, const Vector3 &center, const Vector3 &verticalAxis=Vector3(0,1,0), ECoordinate f=Coordinate_Local);

    void push();
    void pop();
private:
    static std::stack<Object3D> stack_;

    Vector3 position_ {Vector3(0,0,0)};
    Quaternion orientation_{Quaternion::identity()};
    Vector3 scale_{Vector3(1,1,1)};

  };

}


