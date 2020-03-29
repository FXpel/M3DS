#include <cmath>

#include "Tools.h"

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace p3d;



Vector3 p3d::operator*(const Matrix3 &m,const Vector3 &u) {
  return Vector3(m(0)*u.x()+m(3)*u.y()+m(6)*u.z(),
                 m(1)*u.x()+m(4)*u.y()+m(7)*u.z(),
                 m(2)*u.x()+m(5)*u.y()+m(8)*u.z());
}


Vector3 Matrix3::column(size_t c) const {
  const Matrix3 &m=*this;
  auto c2=c*3;
  return Vector3(m(c2),m(c2+1),m(c2+2));
}

Vector3 Matrix3::row(size_t r) const {
  const Matrix3 &m=*this;
  return Vector3(m(r),m(r+3),m(r+6));
}

void Matrix3::column(size_t c, double x,double y,double z) {
  Matrix3 &m=*this;
  auto c2=c*3;
  m(c2)=x;m(c2+1)=y;m(c2+2)=z;
}

void Matrix3::column(size_t c,const Vector3 &u) {
  column(c,u.x(),u.y(),u.z());
}

void Matrix3::row(size_t r, double x,double y,double z) {
  Matrix3 &m=*this;
  m(r)=x;m(r+3)=y;m(r+6)=z;
}

void Matrix3::row(size_t r,const Vector3 &u) {
  row(r,u.x(),u.y(),u.z());
}



Matrix3 Matrix3::inverse() const {
  Matrix3 temp(*this);
  Matrix3 res=Matrix3::identity();

  double t,t2;
  size_t swap;

  // down
  for(size_t i=0;i<3;++i) {
    double max=temp(i,i);
    swap=i;
    for(size_t j=(i+1);j<3;++j) {
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
    for(size_t k=i+1;k<3;++k) {
      t2=temp(i,k);
      res.subScaleColumn(k,i,t2);
      temp.subScaleColumn(k,i,t2);
    }
  }

  // up
  for(int ii=2;ii>=0;--ii) {
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

Matrix3 Matrix3::transpose() const {
  Matrix3 res;
  for(size_t c=0;c<3;++c) {
    res.column(c,this->row(c));
  }
  return res;
}


void Matrix3::subScaleColumn(size_t i,size_t j,double s) {
    auto c1=i*3;auto c2=j*3;
    for(int k=0;k<3;k++) {
        c_[c1]-=(c_[c2]*s);
        c1++;c2++;
    }
}


Matrix3 Matrix3::identity() {
  return Matrix3::fromColumn(Vector3(1,0,0),Vector3(0,1,0),Vector3(0,0,1));
}

Matrix3 Matrix3::fromColumn(const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  Matrix3 res;
  res.column(0,i);
  res.column(1,j);
  res.column(2,k);
  return res;
}



void Matrix3::swapColumn(size_t i,size_t j) {
    double swap;
    auto c1=i*3;
    auto c2=j*3;
    for(size_t k=0;k<3;k++) {
        swap=c_[c1];
        c_[c1]=c_[c2];
        c_[c2]=swap;
        c1++;c2++;
    }
}


void Matrix3::scaleColumn(size_t i,double k) {
    auto c=i*3;
    for(size_t j=0;j<3;j++) {
        c_[c++]*=k;
    }
}

