#include "transformacije.hpp"
#include <cmath>

Vektor Scaling::transform(const Vektor& v) const {
  return v * _num;
}

Vektor Rotation::transform(const Vektor& v) const {
  double fi = _num + atan(v.getY()/v.getX());
  double r = sqrt(v.getX()*v.getX() + v.getY()*v.getY());
  return Vektor(cos(fi)*r, sin(fi)*r);
}

