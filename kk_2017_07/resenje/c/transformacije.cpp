#include "transformacije.hpp"
#include <cmath>
#include <map>

using namespace std;

Vektor Scaling::transform(const Vektor& v) const {
  return v * _num;
}

Vektor Rotation::transform(const Vektor& v) const {
  double fi = _num + atan(v.getY()/v.getX());
  double r = sqrt(v.getX()*v.getX() + v.getY()*v.getY());
  return Vektor(cos(fi)*r, sin(fi)*r);
}

extern map<string, Transformation*> tablica_transformacija;
Vektor TransformationId::transform(const Vektor& v) const {
  map<string, Transformation*>::iterator tmp = tablica_transformacija.find(_name);
  if (tmp == tablica_transformacija.end()) {
    cerr << "Transformacija " << _name << " nije definisana" << endl;
    exit(EXIT_FAILURE);
  }
  return tmp->second->transform(v);
}
