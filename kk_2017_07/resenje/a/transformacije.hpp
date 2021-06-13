#ifndef __TRANSFORMACIJE_HPP__
#define __TRANSFORMACIJE_HPP__ 1

#include "vektor.hpp"

class Transformation {
public:
  virtual ~Transformation() {

  }
  virtual Vektor transform(const Vektor& v) const = 0;
};

class Scaling : public Transformation {
public:
  Scaling(double num)
    :_num(num)
  {}
  Vektor transform(const Vektor& c) const;
private:
  double _num;
};



#endif
