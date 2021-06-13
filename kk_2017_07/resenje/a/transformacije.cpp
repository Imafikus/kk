#include "transformacije.hpp"

Vektor Scaling::transform(const Vektor& c) const {
  return c * _num;
}

