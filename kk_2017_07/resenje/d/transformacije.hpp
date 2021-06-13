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
  Vektor transform(const Vektor& v) const;
private:
  double _num;
};

class Rotation : public Transformation {
public:
  Rotation(double num)
    :_num(num)
  {}
  Vektor transform(const Vektor& v) const;
private:
  double _num;
};

class TransformationId : public Transformation {
public:
  TransformationId(string name)
    :_name(name)
  {}
  Vektor transform(const Vektor& c) const;
private:
  string _name;
};

class Composition : public Transformation {
public:
  Composition(Transformation* l, Transformation* r)
    :_l(l), _r(r)
  {}
  Vektor transform(const Vektor& v) const;
  ~Composition();
private:
  Composition(const Composition& c);
  Composition& operator=(const Composition& c);
  Transformation *_l, *_r;
};


#endif
