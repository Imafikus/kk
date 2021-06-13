#ifndef __TRANSFORMACIJE_HPP__
#define __TRANSFORMACIJE_HPP__ 1

#include "vektor.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;
using namespace llvm::legacy;

class Transformation {
public:
  virtual ~Transformation() {

  }
  virtual Vektor transform(const Vektor& v) const = 0;
  virtual Value* codegen() const = 0;
};

class Scaling : public Transformation {
public:
  Scaling(double num)
    :_num(num)
  {}
  Vektor transform(const Vektor& v) const;
  Value* codegen() const;
private:
  double _num;
};

class Rotation : public Transformation {
public:
  Rotation(double num)
    :_num(num)
  {}
  Vektor transform(const Vektor& v) const;
  Value* codegen() const;
private:
  double _num;
};

class TransformationId : public Transformation {
public:
  TransformationId(string name)
    :_name(name)
  {}
  Vektor transform(const Vektor& c) const;
  Value* codegen() const;
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
  Value* codegen() const;
private:
  Composition(const Composition& c);
  Composition& operator=(const Composition& c);
  Transformation *_l, *_r;
};


#endif
