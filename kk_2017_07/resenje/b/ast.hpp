#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include "transformacije.hpp"
using namespace std;

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

class SynTree {
public:
  virtual Vektor interpret() const = 0;
  virtual Value* codegen() const = 0;
  virtual ~SynTree();
};

class ConstVector : public SynTree {
public:
  ConstVector(Vektor v)
    :_v(v)
  {}
  Vektor interpret() const;
  Value* codegen() const;
private:
  Vektor _v;
};

class Variable : public SynTree {
public:
  Variable(string name)
    :_name(name)
  {}
  Vektor interpret() const;
  Value* codegen() const;
private:
  string _name;
};

class Add : public SynTree {
public:
  Add(SynTree *l, SynTree *r)
    :_l(l), _r(r)
  {}
  Vektor interpret() const;
  ~Add();
  Value* codegen() const;
private:
  Add(const Add&);
  Add& operator=(const Add&);
  SynTree *_l, *_r;
};

class Sub : public SynTree {
public:
  Sub(SynTree *l, SynTree *r)
    :_l(l), _r(r)
  {}
  Vektor interpret() const;
  ~Sub();
  Value* codegen() const;
private:
  Sub(const Sub&);
  Sub& operator=(const Sub&);
  SynTree *_l, *_r;
};

class Mul : public SynTree {
public:
  Mul(double d, SynTree *r)
    :_d(d), _r(r)
  {}
  Vektor interpret() const;
  ~Mul();
  Value* codegen() const;
private:
  Mul(const Mul&);
  Mul& operator=(const Mul&);
  double _d;
  SynTree *_r;
};

class Print : public SynTree {
public:
  Print(SynTree *r)
    :_r(r)
  {}
  Vektor interpret() const;
  ~Print();
  Value* codegen() const;
private:
  Print(const Print&);
  Print& operator=(const Print&);
  SynTree *_r;
};

class Assign : public SynTree {
public:
  Assign(string name, SynTree *r)
    :_name(name), _r(r)
  {}
  Vektor interpret() const;
  ~Assign();
  Value* codegen() const;
private:
  Assign(const Assign&);
  Assign& operator=(const Assign&);
  string _name;
  SynTree *_r;
};

class Declaration : public SynTree {
public:
  Declaration(vector < pair < string, SynTree* > > decls)
    :_decls(decls)
  {}
  Vektor interpret() const;
  ~Declaration();
  Value* codegen() const;
private:
  Declaration(const Declaration&);
  Declaration& operator=(const Declaration&);
  vector < pair < string, SynTree* > > _decls;
};

class Block : public SynTree {
public:
  Block(vector < SynTree* > v)
    :_v(v)
  {}
  Vektor interpret() const;
  ~Block();
  Value* codegen() const;
private:
  Block(const Block&);
  Block& operator=(const Block&);
  vector <SynTree*> _v;
};

class TransformNode : public SynTree {
public:
  TransformNode(Transformation *trans, SynTree* t)
    :_trans(trans), _t(t)
  {}
  Vektor interpret() const;
  ~TransformNode();
  Value* codegen() const;
private:
  TransformNode(const TransformNode&);
  TransformNode& operator=(const TransformNode&);  
  Transformation *_trans;
  SynTree *_t;
};


void InitializeModuleAndPassManager(void);
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName, Type* t);


#endif
