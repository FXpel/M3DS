#include "Matrix4.h"

#include "AngleAxis.h"
#include "p3helpers.h"

#include "Tools.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix3.h"

#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>



/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/
using namespace p3d;
using namespace std;

std::stack<Matrix4> Matrix4::stack; ///< for push/pop. Caution !!! static member (only one stack for all matrix4).



Vector4 p3d::operator*(const Matrix4 &m,const Vector4 &p) {
  Vector4 res; /// (mutual dependancies of x,y,z,w)
  res.x(m(0)*p.x()+m(4)*p.y()+m(8)*p.z()+m(12)*p.w());
  res.y(m(1)*p.x()+m(5)*p.y()+m(9)*p.z()+m(13)*p.w());
  res.z(m(2)*p.x()+m(6)*p.y()+m(10)*p.z()+m(14)*p.w());
  res.w(m(3)*p.x()+m(7)*p.y()+m(11)*p.z()+m(15)*p.w());
  return res;
}

Matrix4 Matrix4::fromFrame(const Vector3 &o,const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  return Matrix4::fromColumn(Vector4(i,0.0),Vector4(j,0.0),Vector4(k,0.0),Vector4(o,1.0));
}



const double *Matrix4::v() const {
    return m_.data();
}



Vector3 Matrix4::transformPoint(const Vector3 &p) const {
  return ((*this)*Vector4(p,1.0)).toPosition();
}

Vector3 Matrix4::transformDirection(const Vector3 &p) const {
  return ((*this)*Vector4(p,0.0)).toDirection();
}




Matrix4 Matrix4::identity() {
  return Matrix4();
}

/*
void Matrix4::print(string mesg) const {
    cout << "matrix " << mesg << endl;
    int index=0;
    for(int i=0;i<4;i++) {
        cout << "(";
        for(int j=0;j<4;j++) {
          cout << setw(4) << setprecision(2) << m_[index] << " ";
          index+=4;
        }
        cout << ")" << endl;
        index-=15;
    }
}
*/

void Matrix4::swapColumn(size_t i,size_t j) {
    double swap;
    size_t c1=i*4;
    size_t c2=j*4;
    for(size_t k=0;k<4;k++) {
        swap=m_[c1];
        m_[c1]=m_[c2];
        m_[c2]=swap;
        c1++;c2++;
    }
}


void Matrix4::scaleColumn(size_t i,double k) {
    size_t c=i*4;
    for(int j=0;j<4;++j) {
        m_[c++]*=k;
    }
}

void Matrix4::scaleRow(size_t i,double k) {
    size_t c=i;
    for(int j=0;j<4;++j) {
        m_[c]*=k;
        c+=4;
    }
}




Matrix4 Matrix4::transpose() const {
  Matrix4 res;
  for(size_t i=0;i<4;++i) {
    for(size_t j=0;j<4;++j) {
        res(i,j)=(*this)(j,i);
    }
  }
  return res;
}




void Matrix4::subScaleColumn(size_t i,size_t j,double s) {
    auto c1=i*4;auto c2=j*4;
    for(int k=0;k<4;k++) {
        m_[c1]-=(m_[c2]*s);
        c1++;c2++;
    }
}

void Matrix4::subScaleRow(size_t i,size_t j,double s) {
    auto r1=i;auto r2=j;
    for(int k=0;k<4;k++) {
        m_[r1]-=(m_[r2]*s);
        r1+=4;r2+=4;
    }
}



void Matrix4::row(size_t i,double x,double y,double z,double w) {
    m_[i]=x;m_[i+4]=y;m_[i+8]=z;m_[i+12]=w;
}

void Matrix4::row(size_t i,const Vector4 &u) {
    row(i,u.x(),u.y(),u.z(),u.w());
}

void Matrix4::column(size_t i,double x,double y,double z,double w) {
    auto i4=i*4;
    m_[i4]=x;m_[i4+1]=y;m_[i4+2]=z;m_[i4+3]=w;
}

void Matrix4::column(size_t i,const Vector4 &u) {
    column(i,u.x(),u.y(),u.z(),u.w());
}



Matrix4 Matrix4::inverse() const {
    /// TODO : naive Gauss-Pivot (down-top)
  Matrix4 temp(*this);
  Matrix4 res=Matrix4::identity();

  double t,t2;
  size_t swap;

  // down
  for(size_t i=0;i<4;++i) {
    double max=temp(i,i);
    swap=i;
    for(size_t j=(i+1);j<4;++j) {
      if (fabs(temp(i,j))>fabs(max)) {
        max=temp(i,j);
        swap=j;
      }
    }
    if (fabs(max)<1e-08)
      throw Error("matrix inverse",__LINE__,__FILE__);
    if (swap!=i) {
      temp.swapColumn(i,swap);
      res.swapColumn(i,swap);
    }
    t=temp(i,i);

    res.scaleColumn(i,1.0/t);
    temp.scaleColumn(i,1.0/t);
    for(size_t k=i+1;k<4;++k) {
      t2=temp(i,k);
      res.subScaleColumn(k,i,t2);
      temp.subScaleColumn(k,i,t2);
    }
  }

  // up
  for(int ii=3;ii>=0;--ii) {
    for(int kk=ii-1;kk>=0;--kk) {
        auto i=static_cast<size_t>(ii);
        auto k=static_cast<size_t>(kk);
      t2=temp(i,k);
      res.subScaleColumn(k,i,t2);
      temp.subScaleColumn(k,i,t2);
    }
  }
  return res;
}



Matrix4 Matrix4::fromQuaternion(const Quaternion &q) {
    Matrix4 res;
  double x=q.x(),y=q.y(),z=q.z(),w=q.w();
  double x2=x*x,y2=y*y,z2=z*z,w2=w*w;
  double xy=2.0*x*y,xz=2.0*x*z,xw=2.0*x*w,yz=2.0*y*z,yw=2.0*y*w,zw=2.0*z*w;
  res.column(0,w2+x2-y2-z2,xy+zw,xz-yw,0);
  res.column(1,xy-zw,w2-x2+y2-z2,yz+xw,0);
  res.column(2,xz+yw,yz-xw,w2-x2-y2+z2,0);
  res.column(3,0,0,0,1);
  return res;
}

Matrix4 Matrix4::fromColumn(const Vector4 &c0,const Vector4 &c1,const Vector4 &c2,const Vector4 &c3) {
  Matrix4 res;
  res.column(0,c0);
  res.column(1,c1);
  res.column(2,c2);
  res.column(3,c3);
  return res;
}


Matrix4 Matrix4::fromAngleAxis(double angle,double xaxis,double yaxis,double zaxis) { // TODO : should be Vector3 version from x,y,z version
  Vector3 u=normalize(Vector3(xaxis,yaxis,zaxis));
  Vector3 u2=u*u;
  double c=cos(toRadian(angle));
  double s=sin(toRadian(angle));



  Matrix4 res;
  res.row(0, u2.x()+(1-u2.x())*c,        u.x()*u.y()*(1-c)-u.z()*s,       u.x()*u.z()*(1-c)+u.y()*s,       0.0);
  res.row(1, u.x()*u.y()*(1-c)+u.z()*s,  u2.y()+(1-u2.y())*c,             u.y()*u.z()*(1-c)-u.x()*s,       0.0);
  res.row(2, u.x()*u.z()*(1-c)-u.y()*s,  u.y()*u.z()*(1-c)+u.x()*s,       u2.z()+(1-u2.z())*c,             0.0);
  res.row(3, 0,                          0,                               0,                               1.0);
  return res;
}

Matrix4 Matrix4::fromAngleAxis(double angle,const Vector3 &axe) {
  return fromAngleAxis(angle,axe.x(),axe.y(),axe.z());
}

Matrix4 Matrix4::fromAngleAxis(const AngleAxis &r) {
  return fromAngleAxis(r.angleDegree(),r.axis());
}

Matrix4 Matrix4::fromScale(double kx,double ky,double kz) {
  Matrix4 res=Matrix4::identity();
  res(0,0)=kx;
  res(1,1)=ky;
  res(2,2)=kz;
  return res;
}

Matrix4 Matrix4::fromScale(const Vector3 &s) {
  return fromScale(s.x(),s.y(),s.z());
}

Matrix4 Matrix4::fromTranslation(double x,double y,double z) {
  Matrix4 res=Matrix4::identity();
  res.column(3,x,y,z,1);
  return res;
}


Matrix4 Matrix4::fromTranslation(const Vector3 &v) {
  return fromTranslation(v.x(),v.y(),v.z());
}


Matrix4 Matrix4::fromScale(double k) {
  return fromScale(k,k,k);
}



Matrix4 &Matrix4::scale(const Vector3 &u) {
    this->scale(u.x(),u.y(),u.z());
    return *this;
}

Matrix4 &Matrix4::scale(double k) {
  for(unsigned int i=0;i<12;++i) { // last column unchanged by scale
    m_[i]*=k;
  }
  return *this;
}

Matrix4 &Matrix4::scale(double x,double y,double z) {
  for(unsigned int i=0;i<4;++i) {
    m_[i]*=x;
  }
  for(unsigned int i=4;i<8;++i) {
    m_[i]*=y;
  }
  for(unsigned int i=8;i<12;++i) {
    m_[i]*=z;
  }
  return *this;
}



Vector4 Matrix4::row(size_t i) const {
  Vector4 res(m_[i],m_[i+4],m_[i+8],m_[i+12]);
	return res;
}

Vector4 Matrix4::column(size_t i) const {
  auto ii=i*4;
  return Vector4(m_[ii],m_[ii+1],m_[ii+2],m_[ii+3]);
}

Matrix4 &Matrix4::translate(double x,double y,double z) {
  Matrix4 m=Matrix4::fromTranslation(x,y,z);
  return (*this)*=m;
}


Matrix4 &Matrix4::translate(const Vector3 &u) {
  return translate(u.x(),u.y(),u.z());
}

Matrix4 &Matrix4::rotate(double angle,const Vector3 &u) {
  return (*this)*=Matrix4::fromAngleAxis(angle,u);
}

Matrix4 &Matrix4::rotate(const Quaternion &q) {
  return (*this)*=Matrix4::fromQuaternion(q);
}

Matrix4 &Matrix4::rotate(double angle,double xaxis,double yaxis,double zaxis) {
  return this->rotate(angle,Vector3(xaxis,yaxis,zaxis));
}

Matrix4 Matrix4::mix(const Matrix4 &m1,const Matrix4 &m2,double t) {
  Matrix4 res;
  for(size_t k=0;k<16;++k) {
    res(k)=m1(k)+t*(m2(k)-m1(k));
  }
  return res;
}


Matrix4 p3d::operator*(Matrix4 m1,const Matrix4 &m2) {
    return m1*=m2;
}

Matrix4 &Matrix4::operator*=(Matrix4 m) {
    Matrix4 temp;
    for(size_t i=0;i<4;++i) {
      for(size_t j=0;j<4;++j) {
        temp(i,j)=0;
        for(size_t k=0;k<4;++k) {
          temp(i,j)+=(*this)(i,k)*m(k,j);
        }
      }
    }
    return *this=temp;
}

Matrix4 p3d::operator-(const Matrix4 &m) {
  Matrix4 res;
  for(unsigned int i=0;i<16;i++) {
    res(i)=-m(i);
  }
  return res;
}

Matrix4 &Matrix4::frustum(double left,double right,double bottom,double top,double dnear,double dfar) {
  return (*this)*=Matrix4::fromFrustum(left,right,bottom,top,dnear,dfar);
}


Matrix4 Matrix4::fromFrustum(double left,double right,double bottom,double top,double dnear,double dfar) {
  Matrix4 mat;
  mat.row(0,2.0*dnear/(right-left)  ,0                        ,(right+left)/(right-left)      ,0                           );
  mat.row(1,0                       ,2.0*dnear/(top-bottom)   ,(top+bottom)/(top-bottom)      ,0                           );
  mat.row(2,0                       ,0                        ,-(dfar+dnear)/(dfar-dnear)     ,-2.0*dfar*dnear/(dfar-dnear));
  mat.row(3,0                       ,0                        ,-1                             ,0                           );
  return mat;
}

Matrix4 &Matrix4::ortho(double left,double right,double bottom,double top,double dnear,double dfar) {
  return (*this)*=Matrix4::fromOrtho(left,right,bottom,top,dnear,dfar);
}


Matrix4 Matrix4::fromOrtho(double left,double right,double bottom,double top,double dnear,double dfar) {
  Matrix4 mat;
  mat.row(0,2.0/(right-left)  ,0                 ,0                    ,-(right+left)/(right-left));
  mat.row(1,0                 ,2.0/(top-bottom)  ,0                    ,-(top+bottom)/(top-bottom));
  mat.row(2,0                 ,0                 ,-2.0/(dfar-dnear)    ,-(dfar+dnear)/(dfar-dnear));
  mat.row(3,0                 ,0                 ,0                    ,1                         );
  return mat;
}

Matrix4 &Matrix4::perspective(double fov,double aspect,double dnear,double dfar) {
  return (*this)*=Matrix4::fromPerspective(fov,aspect,dnear,dfar);
}

Matrix4 Matrix4::fromPerspective(double fov,double aspect,double dnear,double dfar) {
  double f=1.0/tan(fov/2.0);
  Matrix4 mat;
  mat.row(0,f/aspect  ,0  ,0                         ,0                          );
  mat.row(1,0         ,f  ,0                         ,0                          );
  mat.row(2,0         ,0  ,(dfar+dnear)/(dnear-dfar) ,2.0*dnear*dfar/(dnear-dfar));
  mat.row(3,0         ,0  ,-1                        ,0                          );
  return mat;
}


Matrix4 Matrix4::fromLookAt(const Vector3 &position,const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k=normalize(position-at);
  i=normalize(cross(up,k));
  j=normalize(cross(k,i));

  Matrix4 lookAt;
  lookAt=Matrix4::fromFrame(position,i,j,k);
  return lookAt.inverse();
}


void Matrix4::push() {
  stack.push(*this);
}

void Matrix4::pop() {
  *this=stack.top();
  stack.pop();
}

Matrix3 Matrix4::topLeft3x3() const {
  Matrix3 res;
  res.column(0,m_[0],m_[1],m_[2]);
  res.column(1,m_[4],m_[5],m_[6]);
  res.column(2,m_[8],m_[9],m_[10]);
  return res;
}

Matrix3 Matrix4::normalMatrix() const {
  Matrix3 res=this->topLeft3x3();
  res=res.inverse().transpose();
  return res;

}

Quaternion Matrix4::toQuaternion() const {
  return Quaternion::fromFrame(
        Vector3(m_[0],m_[1],m_[2]),
      Vector3(m_[4],m_[5],m_[6]),
      Vector3(m_[8],m_[9],m_[10])
      );
}


Matrix4 Matrix4::fromSlerp(const Matrix4 &m1,const Matrix4 &m2,double lambda) {
  Vector3 p1=m1.column(3).toPosition();
  Vector3 p2=m2.column(3).toPosition();
  Quaternion q1=m1.toQuaternion();
  Quaternion q2=m2.toQuaternion();

  Vector3 pI=(1.0-lambda)*p1+lambda*p2;
  Quaternion qI=slerp(q1,q2,lambda);

  Matrix4 res=Matrix4::identity();
  res.translate(pI);
  res.rotate(qI);
  return res;
}




