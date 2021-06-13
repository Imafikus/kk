#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <string>
#include <vector>
using namespace std;

#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/LinkAllPasses.h"

#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

class ExprAST {
public:
  virtual Value* codegen() const = 0;
  virtual ~ExprAST() {

  }
};

class NumberExprAST : public ExprAST {
public:
  NumberExprAST(unsigned x)
    :Val(x)
  {}
  Value* codegen() const;
private:
  unsigned Val;
};


class VariableExprAST : public ExprAST {
public:
  VariableExprAST(string s)
    :Name(s)
  {}
  Value* codegen() const;
private:
  string Name;
};

class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(ExprAST *l, ExprAST *r)
    :LHS(l), RHS(r)
  {}
  ~BinaryExprAST();
private:
  BinaryExprAST(const BinaryExprAST&);
  BinaryExprAST& operator=(const BinaryExprAST&);
protected:
  ExprAST *LHS, *RHS;
};

class AndExprAST : public BinaryExprAST {
public:
  AndExprAST(ExprAST *l, ExprAST *r)
    :BinaryExprAST(l, r)
  {}
  Value* codegen() const;
};

class OrExprAST : public BinaryExprAST {
public:
  OrExprAST(ExprAST *l, ExprAST *r)
    :BinaryExprAST(l, r)
  {}
  Value* codegen() const;
};

class XorExprAST : public BinaryExprAST {
public:
  XorExprAST(ExprAST *l, ExprAST *r)
    :BinaryExprAST(l, r)
  {}
  Value* codegen() const;
};

class ShlExprAST : public BinaryExprAST {
public:
  ShlExprAST(ExprAST *l, ExprAST *r)
    :BinaryExprAST(l, r)
  {}
  Value* codegen() const;
};

class ShrExprAST : public BinaryExprAST {
public:
  ShrExprAST(ExprAST *l, ExprAST *r)
    :BinaryExprAST(l, r)
  {}
  Value* codegen() const;
};

class UnaryExprAST : public ExprAST {
public:
  UnaryExprAST(ExprAST *e):
    val(e)
  {}
  ~UnaryExprAST();
private:
  UnaryExprAST(const UnaryExprAST&);
  UnaryExprAST& operator=(const UnaryExprAST&);
protected:
  ExprAST *val;
};

class NotExprAST : public UnaryExprAST {
public:
  NotExprAST(ExprAST *e)
    :UnaryExprAST(e)
  {}
  Value* codegen() const;
};

class PrintExprAST : public UnaryExprAST {
public:
  PrintExprAST(ExprAST *e)
    :UnaryExprAST(e)
  {}
  Value* codegen() const;
};

class PrototypeAST {
public:
  PrototypeAST(string s, vector<string> v)
    :Name(s), Args(v)
  {}
  Function* codegen() const;
  string getName() const {
    return Name;
  }
private:
  string Name;
  vector<string> Args;
};

class AssignExprAST : public ExprAST {
public:
  AssignExprAST(string n, ExprAST *e)
    :Name(n), Expression(e)
  {}
  Value* codegen() const;
  ~AssignExprAST();
private:
  AssignExprAST(const AssignExprAST&);
  AssignExprAST& operator=(const AssignExprAST&);
  string Name;
  ExprAST *Expression;
};

void InitializeModuleAndPassManager();
AllocaInst* CreateEntryBlockAlloca(Function *f, string s);
void endOfMain();
void printModule();
#endif
