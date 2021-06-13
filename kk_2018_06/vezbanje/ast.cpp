#include "ast.hpp"
#include <iostream>

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
Module* TheModule;
map<string, AllocaInst*> NamedValues;
legacy::FunctionPassManager* TheFPM;

extern Value* Str;
extern Value* Str1;
extern Function* PrintFja;
extern Function* MainFja;

BinaryExprAST::~BinaryExprAST() {
  delete LHS;
  delete RHS;
}

UnaryExprAST::~UnaryExprAST() {
  delete val;
}

Value* VariableExprAST::codegen() const {
  AllocaInst* V = NamedValues[Name];
  if (!V) {
    cerr << "Nepoznata promenljiva " << Name << endl;
    return nullptr;
  }
  return Builder.CreateLoad(V, Name);
}

Value* NumberExprAST::codegen() const {
  return ConstantInt::get(TheContext, APInt(32, Val));
}

Value* AndExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateAnd(L, R, "andtmp");
}

Value* OrExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateOr(L, R, "ortmp");
}

Value* XorExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateXor(L, R, "xortmp");
}

Value* ShlExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateShl(L, R, "shltmp");
}

Value* ShrExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateLShr(L, R, "shrtmp");
}

Value* NotExprAST::codegen() const {
  Value* V = val->codegen();
  if (!V)
    return nullptr;
  return Builder.CreateNot(V, "nottmp");
}


Function* PrototypeAST::codegen() const {
  vector<Type*> tmp;
  for (unsigned i = 0; i < Args.size(); i++)
    tmp.push_back(Type::getDoubleTy(TheContext));
  
  
  FunctionType *FT = FunctionType::get(Type::getDoubleTy(TheContext), tmp, false);
  
  Function* f = Function::Create(FT, Function::ExternalLinkage, Name, TheModule);

  unsigned i = 0;
  for (auto &a : f->args())
    a.setName(Args[i++]);

  return f;
}

Value* AssignExprAST::codegen() const {
  Function *f = Builder.GetInsertBlock()->getParent();
  
  AllocaInst *Alloca = nullptr;
  if(NamedValues[Name] == nullptr) {
    Alloca = CreateEntryBlockAlloca(f, Name);
    NamedValues[Name] = Alloca;  
  }

  Value *tmp = Expression->codegen();
  if (!tmp)
    return nullptr;
  return Builder.CreateStore(tmp, Alloca);
}

AssignExprAST::~AssignExprAST() {
  delete Expression;
}

Value* PrintExprAST::codegen() const {
  Value* V = val->codegen();
  if (!V)
    return nullptr;
  
  AllocaInst* Alloca = NamedValues["flag"];
  if (Alloca == NULL) {
    Alloca = CreateEntryBlockAlloca(MainFja, "flag");
    NamedValues["flag"] = Alloca;
  }

  Value *tmp = Builder.CreateLoad(Type::getInt32Ty(TheContext), Alloca, "flag");
  Value *CondV = Builder.CreateICmpEQ(tmp, ConstantInt::get(TheContext, APInt(32, 0)));


  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");
  Builder.CreateCondBr(CondV, ThenBB, ElseBB);
  
  Builder.SetInsertPoint(ThenBB);
  vector<Value *> ArgsV;
  ArgsV.push_back(Str);
  ArgsV.push_back(V);
  Builder.CreateCall(PrintFja, ArgsV, "printfCall");
  Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);
  ArgsV.clear();
  ArgsV.push_back(Str1);
  ArgsV.push_back(V);
  Builder.CreateCall(PrintFja, ArgsV, "printfCall");
  Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  return V;
}

void InitializeModuleAndPassManager() {
  TheModule = new Module("Moj modul", TheContext);
  

  vector<Type*> v(0);
  FunctionType* FT2 = FunctionType::get(Type::getInt32Ty(TheContext), v, false);
  MainFja = Function::Create(FT2, Function::ExternalLinkage, "main", TheModule);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", MainFja);
  Builder.SetInsertPoint(BB);

  FunctionType *FT1 = FunctionType::get(IntegerType::getInt32Ty(TheContext), PointerType::get(Type::getInt8Ty(TheContext), 0), true);
  PrintFja = Function::Create(FT1, Function::ExternalLinkage, "printf", TheModule);
  Str = Builder.CreateGlobalStringPtr("%u\n");
  Str1 = Builder.CreateGlobalStringPtr("0x%x\n");
}

void endOfMain() {
  Builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 0)));
  verifyFunction(*MainFja);
}

void printModule() {
  TheModule->print(outs(), nullptr);
}

AllocaInst* CreateEntryBlockAlloca(Function *f, string s) {
  IRBuilder<> TmpBuilder(&(f->getEntryBlock()), f->getEntryBlock().begin());
  return TmpBuilder.CreateAlloca(Type::getInt32Ty(TheContext), 0, s);
}
