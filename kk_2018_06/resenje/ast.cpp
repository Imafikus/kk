#include "ast.hpp"

#include <iostream>


LLVMContext TheContext;
map<string, AllocaInst*> NamedValues;
IRBuilder<> Builder(TheContext);
Module* TheModule;
// FunctionPassManager *TheFPM;

ExprAST::~ExprAST() {

}

InnerExprAST::InnerExprAST(ExprAST* a) {
  _nodes.resize(0);
  _nodes.push_back(a);
}

InnerExprAST::InnerExprAST(ExprAST* a, ExprAST* b) {
  _nodes.resize(0);
  _nodes.push_back(a);
  _nodes.push_back(b);
}

InnerExprAST::InnerExprAST(ExprAST* a, ExprAST* b, ExprAST* c) {
  _nodes.resize(0);
  _nodes.push_back(a);
  _nodes.push_back(b);
  _nodes.push_back(c);
}

InnerExprAST::InnerExprAST(ExprAST* a, ExprAST* b, ExprAST* c, ExprAST* d) {
  _nodes.resize(0);
  _nodes.push_back(a);
  _nodes.push_back(b);
  _nodes.push_back(c);
  _nodes.push_back(d);
}

InnerExprAST::InnerExprAST(vector<ExprAST*> a) {
  _nodes = a;
}

InnerExprAST::~InnerExprAST() {
  for (unsigned i = 0; i < _nodes.size(); i++)
    delete _nodes[i];
}

Value* NumberExprAST::codegen() const {
  return ConstantInt::get(TheContext, APInt(32, Val));
}

Value* VariableExprAST::codegen() const {
  AllocaInst* tmp = NamedValues[Name];
  if (tmp == NULL) {
    cerr << "Promenljiva " + Name + " nije definisana" << endl;
    return NULL;
  }
  return Builder.CreateLoad(Type::getInt32Ty(TheContext), tmp, Name.c_str());
}

Value* AndExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  Value *d = _nodes[1]->codegen();
  if (l == NULL || d == NULL)
    return NULL;
  return Builder.CreateAnd(l, d, "andtmp");
}

Value* OrExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  Value *d = _nodes[1]->codegen();
  if (l == NULL || d == NULL)
    return NULL;
  return Builder.CreateOr(l, d, "ortmp");
}

Value* XorExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  Value *d = _nodes[1]->codegen();
  if (l == NULL || d == NULL)
    return NULL;
  return Builder.CreateXor(l, d, "xortmp");
}

Value* ShlExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  Value *d = _nodes[1]->codegen();
  if (l == NULL || d == NULL)
    return NULL;
  return Builder.CreateShl(l, d, "shltmp");
}

Value* ShrExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  Value *d = _nodes[1]->codegen();
  if (l == NULL || d == NULL)
    return NULL;
  return Builder.CreateLShr(l, d, "shrtmp");
}

Value* NotExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  if (l == NULL)
    return NULL;
  return Builder.CreateNot(l, "nottmp");
}

extern Value* Str;
extern Value* Str1;
extern Function *PrintfFja;
extern Function *MainFja;

Value* PrintExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  if (l == NULL)
    return NULL;

  AllocaInst* Alloca = NamedValues["flag"];
  if (Alloca == NULL) {
    Alloca = CreateEntryBlockAlloca(MainFja, "flag");
    NamedValues["flag"] = Alloca;
  }
  
  Value *tmp = Builder.CreateLoad(Type::getInt32Ty(TheContext), Alloca, "flag");
  Value *CondV = Builder.CreateICmpEQ(tmp, ConstantInt::get(TheContext, APInt(32, 0)));

  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock *ThenBB =
    BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");
  
  Builder.CreateCondBr(CondV, ThenBB, ElseBB);
  Builder.SetInsertPoint(ThenBB);
  vector<Value *> ArgsV;
  ArgsV.push_back(Str);
  ArgsV.push_back(l);
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");
  Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);
  ArgsV.clear();
  ArgsV.push_back(Str1);
  ArgsV.push_back(l);
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");
  Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  return l;
}

Value* SetExprAST::codegen() const {
  Value *l = _nodes[0]->codegen();
  if (l == NULL)
    return NULL;
  AllocaInst* Alloca = NamedValues[Name];
  if (Alloca == NULL) {
    Alloca = CreateEntryBlockAlloca(MainFja, Name);
    NamedValues[Name] = Alloca;
  }
  return Builder.CreateStore(l, Alloca);
}

Value* SeqExprAST::codegen() const {
  for (unsigned i = 0; i < _nodes.size(); i++) {
    Value *tmp = _nodes[i]->codegen();
    if (tmp == NULL)
      return NULL;
  }
  return NULL;
}


void InitializeModuleAndPassManager(void) {
  TheModule = new Module("my_module", TheContext);

  // Create a new pass manager attached to it.
  // TheFPM = new FunctionPassManager(TheModule);

  // // Do simple "peephole" optimizations and bit-twiddling optzns.
  // TheFPM->add(createInstructionCombiningPass());
  // // Reassociate expressions.
  // TheFPM->add(createReassociatePass());
  // // Eliminate Common SubExpressions.
  // TheFPM->add(createGVNPass());
  // // Simplify the control flow graph (deleting unreachable blocks, etc).
  // TheFPM->add(createCFGSimplificationPass());
  // TheFPM->add(createPromoteMemoryToRegisterPass());

  // TheFPM->doInitialization();
}

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(TheContext), 0, VarName.c_str());
}
