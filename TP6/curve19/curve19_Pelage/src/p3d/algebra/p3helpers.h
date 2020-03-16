#pragma once


#include <limits>

namespace p3d {

const double epsilon=0.00001;
const std::size_t invalid_index=std::numeric_limits<std::size_t>::max();


double toDegree(double a);
double toRadian(double a);


template<class T> void swapIfMin(T *a,T *b) {T c;if (*b<*a) {c=*a;*a=*b;*b=c;}}

}

