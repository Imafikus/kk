#ifndef __VEKTOR_HPP__
#define __VEKTOR_HPP__ 1

#include <iostream>

using namespace std;

// klasa koja predstavlja vektor
class Vektor {
public:
  // konstruktor sa dva argumenta
  Vektor(double x, double y)
    :_x(x), _y(y)
  {}
  // konstruktor bez argumenata
  Vektor()
  {}
  // metod za ispit vektora
  void write(ostream& ostr) const;
  // metod za posltavljanje vrednosti vektora
  void set(double x, double y);
  // operatori nad vektorima
  Vektor operator + (const Vektor& v) const;
  Vektor operator - (const Vektor& v) const;
  Vektor operator * (const double& k) const;
  // geteri za koordinate vektora
  double getX() const;
  double getY() const;
private:
  // koordinate
  double _x, _y;
};

// ispis vektora
ostream& operator << (ostream& ostr, const Vektor& v);

#endif
