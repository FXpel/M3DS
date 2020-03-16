#pragma once

#include <vector>
#include <array>
#include <string>
#include <stack>

/**
*
* @file
*
* @brief Matrix 4x4 class
* @author F. Aubert
*
*/

namespace p3d {
class Vector3;
class Vector4;
class Quaternion;
class AngleAxis;
class Matrix3;

/**
@class Matrix4
@brief Matrix 4x4
*/
class Matrix4 {

public:
    /// @return the i-th coefficient of this (Matrix4 is column-major) lvalue
    inline double &operator()(size_t i) {return m_[i];}
    /// @return the i-th coefficient of this (Matrix4 is column-major) rvalue
    inline double operator()(size_t i) const {return m_[i];}
    /// @return the element at row i and at column j lvalue
    inline double &operator()(size_t i,size_t j) {return m_[i|(j<<2)];}
    /// @return the element at row i and at column j rvalue
    inline double operator()(size_t i,size_t j) const {return m_[i|(j<<2)];}
    /// @return coordinates pointer
    const double *v() const;
    /// @return the i-th row
    Vector4 row(size_t i) const;
    /// @return the i-th column
    Vector4 column(size_t i) const;
    /// @return transpose of this
    Matrix4 transpose() const;
    /// @brief return inverse
    Matrix4 inverse() const;
    void fromRotation();
    /// @return the point p is transformed according to this (internally the w component is set to 1)
    Vector3 transformPoint(const Vector3 &p) const;
    /// @return the direction u is transformed according to this (internally the w component is set to 0)
    Vector3 transformDirection(const Vector3 &u) const;
    /// @return p is transformed according to this
    Vector4 transform(const Vector4 &p) const;


    /// swap columns i and j
    void swapColumn(size_t i,size_t j);
    /// multiply column i by k
    void scaleColumn(size_t i,double k);
    /// multiply row i of this by k
    void scaleRow(size_t i,double k);
    /// scale the j-th column by k then substracts the column j : Ci = Ci-k*Cj
    void subScaleColumn(size_t i,size_t j,double k);
    /// Scale the j-th row by k then substracts the j-th row : Ri<-Ri-k*Rj
    void subScaleRow(size_t i,size_t j,double s);
    /// set the ith row to (x,y,z,w)
    void row(size_t i,double x,double y,double z,double w);
    void row(size_t i,const Vector4 &u);
    /// set the ith column to (x,y,z,w)
    void column(size_t i,double x,double y,double z,double w);
    void column(size_t i,const Vector4 &v);

    /// right-multiplies this by a : this = this*a
    Matrix4 &mul(const Matrix4 &a);
    /// left-multiplies this by a : this = a*this
    Matrix4 &mulLeft(const Matrix4 &a);
    /// set this to a*b
    Matrix4 &mul(const Matrix4 &a,const Matrix4 &b);
    /// printf the matrix
    void print(std::string mesg) const;

    /// this matrix is composed with a translation : right-multiplies this by the translation matrix from u
    Matrix4 &translate(const Vector3 &u);
    /// this matrix is composed with a translation : right-multiplies this by the translation matrix from (x,y,z)
    Matrix4 &translate(double x,double y,double z);

    /// this matrix is composed with a rotation (angle in degree) : right-multiplies this by the rotation matrix from (angle,u)
    Matrix4 &rotate(double angle,const Vector3 &u);
    /// this matrix is composed with a rotation (angle in degree) : right-multiplies this by the rotation matrix from (angle,(x,y,z))
    Matrix4 &rotate(double angle, double xaxis, double yaxis, double zaxis);
    /// this matrix is composed with a rotation : right-multiplies this by the rotation matrix from q
    Matrix4 &rotate(const Quaternion &q);
    /// this matrix is composed with a scaling : right-multiplies this by the scaling matrix from u
    Matrix4 &scale(const Vector3 &u);
    /// this matrix is composed with a scaling : right-multiplies this by the scaling matrix from k (actually multiplies all the matrix elements by k)
    Matrix4 &scale(double k);
    /// this matrix is composed with a scaling : right-multiplies this by the scaling matrix from k (actually multiplies all the matrix elements by (x,y,z))
    Matrix4 &scale(double x,double y,double z=1.0);
    /// right-multiplies this by the frustum matrix (prepared for 1/z interpolation, i.e. same as openGL 2.1)
    Matrix4 &frustum(double left, double right, double bottom, double top, double dnear, double dfar);
    /// right-multiplies this by the orthogonal projection matrix
    Matrix4 &ortho(double left,double right,double bottom,double top,double near,double far);
    /// right-multiplies this by the perspective matrix (same as gluPerspective)
    Matrix4 &perspective(double fov, double aspect, double dnear, double dfar);
    /// returns the normal matrix (i.e. the top-left 3x3 matrix from transpose(inverse(this))
    Matrix3 normalMatrix() const;
    /// saves the content of this onto the top of the stack (!caution : only one stack for all matrices)
    void push();
    /// restores the content of this from the top of the stack
    void pop();


    /// right-multiplies this matrix by m
    Matrix4 &operator*=(Matrix4 m); // m by copy to allow self-parameter

    ///
    Matrix3 topLeft3x3() const;

    Quaternion toQuaternion() const;

    /// converts translation v to the corresponding matrix
    static Matrix4 fromTranslation(const Vector3 &v);
    /// converts translation (x,y,z) to the corresponding matrix
    static Matrix4 fromTranslation(double x,double y,double z);
    /// converts scale v to the corresponding matrix
    static Matrix4 fromScale(const Vector3 &v);
    /// converts scale (k,k,k) to the corresponding matrix
    static Matrix4 fromScale(double k);
    /// converts scale (x,y,z) to the corresponding matrix
    static Matrix4 fromScale(double x,double y,double z=1.0);
    /// @return the rotation matrix from v1 to v2 (axis is \f$ v1\wedge v2\f$, and angle is (v1,v2))
    static Matrix4 fromRotation(const Vector3 &v1,const Vector3 &v2);
    /// returns a frustum projection matrix
    static Matrix4 fromFrustum(double left, double right, double bottom, double top, double dnear, double dfar);
    /// returns an orthogonal projection matrix
    static Matrix4 fromOrtho(double left, double right, double bottom, double top, double dnear, double dfar);
    /// returns a perspective projection matrix
    static Matrix4 fromPerspective(double fov, double aspect, double dnear, double dfar);
    /// converts a quaternion to the corresponding matrix
    static Matrix4 fromQuaternion(const Quaternion &q);
    ///
    static Matrix4 fromAngleAxis(double angle,double axisX,double axisY,double axisZ);
    static Matrix4 fromAngleAxis(double angle,const Vector3 &axis);
    static Matrix4 fromAngleAxis(const AngleAxis &r);
    /// returns the identity
    static Matrix4 identity();
    static Matrix4 fromColumn(const Vector4 &c0, const Vector4 &c1, const Vector4 &c2, const Vector4 &c3);
    static Matrix4 mix(const Matrix4 &m1, const Matrix4 &m2, double t);
    /// @return Matrix with vector basis (o,i,j,k) (if vectors of frame 1=(o,i,j,k) are expressed in frame 0 then this=M1->0; the last row is (0,0,0,1))
    static Matrix4 fromFrame(const Vector3 &o,const Vector3 &i,const Vector3 &j,const Vector3 &k);
    static Matrix4 fromSlerp(const Matrix4 &m1, const Matrix4 &m2,double lambda);
    static Matrix4 fromLookAt(const Vector3 &position,const Vector3 &at,const Vector3 &up);

private:
    std::array<double,16> m_ {1.0,0.0,0.0,0.0,
                              0.0,1.0,0.0,0.0,
                              0.0,0.0,1.0,0.0,
                              0.0,0.0,0.0,1.0}; ///< coefficients (column order)

      static std::stack<Matrix4> stack; ///< for push/pop. Caution !!! static member (only one stack for all matrix4).
};


/// returns the opposite of m
Matrix4 operator-(const Matrix4 &m);
/// returns m1*m2 (composition of m1 with m2)
Matrix4 operator*(Matrix4 m1,const Matrix4 &m2);
/// returns m1*p (transformation of p according to m1)
Vector4 operator*(const Matrix4 &m,const Vector4 &p);



/// use it only for conversion from Matrix4 (double coefficients) to float coefficients when necessary (OpenGL)
class Mat4f {
  std::array<float,16> m;
public:
  explicit Mat4f(const Matrix4 &m4) {for(size_t i=0;i<16;++i) m[i]=static_cast<float>(m4(i));}
  float *v() {return m.data();}
};





}

