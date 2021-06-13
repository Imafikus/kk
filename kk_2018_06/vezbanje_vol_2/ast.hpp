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

class ShrExprAST : public BinaryExprAST {
public:
  ShrExprAST(ExprAST *l, ExprAST *r)
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

class NotExprAST : public ExprAST {
public:
  NotExprAST(ExprAST* _e)
    :e(_e)
  {}
  Value* codegen() const;
private:
  ExprAST *e;
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

class PrintExprAST : public ExprAST {
public:
  PrintExprAST(ExprAST *e)
    :Expression(e)
  {}
  Value* codegen() const;
  ~PrintExprAST();
private:
  PrintExprAST(const PrintExprAST&);
  PrintExprAST& operator=(const PrintExprAST&);
  ExprAST *Expression;
};

class FunctionExprAST : public ExprAST {
public:
  FunctionExprAST(string n, vector<string>_args, ExprAST* _blok)
    :Name(n), args(_args), blok(_blok)
  {}
  Value* codegen() const;
  ~FunctionExprAST();
private:
  FunctionExprAST(const FunctionExprAST&);
  FunctionExprAST& operator=(const FunctionExprAST&);
  string Name;
  vector<string> args;
  ExprAST* blok;
};

class CallExprAST : public ExprAST {
public:
  CallExprAST(string s, vector<ExprAST*> v)
    :Callee(s), Args(v)
  {}
  Value* codegen() const;
  ~CallExprAST();
private:
  CallExprAST(const CallExprAST&);
  CallExprAST& operator=(const CallExprAST&);
  string Callee;
  vector<ExprAST*> Args;
};

class SeqExprAST : public ExprAST {
public:
  SeqExprAST(vector<ExprAST*> v)
    :cmds(v)
  {}
  Value* codegen() const;
private:
  SeqExprAST(const SeqExprAST&);
  SeqExprAST& operator=(const SeqExprAST&);
  vector<ExprAST*> cmds;
};


void initMain();
void endMain();
AllocaInst* CreateEntryBlockAlloca(Function *f, string s);

#endif