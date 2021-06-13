#include "ast.hpp"

#include <iostream>


LLVMContext TheContext;
map<string, AllocaInst*> NamedValues;
IRBuilder<> Builder(TheContext);
Module* TheModule;
Value *Str;
Value *Str1;
Function* MainFja;
Function* PrintfFja;

// FunctionPassManager *TheFPM;

BinaryExprAST::~BinaryExprAST() {
  delete LHS;
  delete RHS;
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

Value* ShrExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateLShr(L, R, "shrtmp");
}

Value* ShlExprAST::codegen() const {
  Value* L = LHS->codegen();
  Value* R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  return Builder.CreateShl(L, R, "shltmp");
}

Value* NotExprAST::codegen() const {
  Value* val = e->codegen();
  if (!val)
    return nullptr;
  return Builder.CreateNot(val, "nottmp");
}

Value* NumberExprAST::codegen() const {
  return ConstantInt::get(TheContext, APInt(32, Val));
}

Value* VariableExprAST::codegen() const {
  AllocaInst* V = NamedValues[Name];
  if (!V) {
    cerr << "Nepoznata promenljiva " << Name << endl;
    return nullptr;
  }
  return Builder.CreateLoad(V, Name);
}

Value* AssignExprAST::codegen() const {
  Value *r = Expression->codegen();
  if (!r)
      return nullptr;
      
  auto Alloca = NamedValues[Name];
  if(!Alloca) {
      Alloca = CreateEntryBlockAlloca(MainFja, Name);
      NamedValues[Name] = Alloca;
  }
  
  return Builder.CreateStore(r, Alloca);

}

AssignExprAST::~AssignExprAST() {
  delete Expression;
}


Value* PrintExprAST::codegen() const {
    Value* v = Expression->codegen();
    
    if(!v) {
        return nullptr;
    }
    
    if (Str == NULL) {
      Str = Builder.CreateGlobalStringPtr("%d\n");
    }  
    if (Str1 == NULL) {
      Str1 = Builder.CreateGlobalStringPtr("0x%x\n");
    }
    
    AllocaInst* Alloca = NamedValues["flag"];
    if (Alloca == nullptr) {
      Alloca = CreateEntryBlockAlloca(MainFja, "flag");
      NamedValues["flag"] = Alloca;
    }
    
    Value *tmp = Builder.CreateLoad(Type::getInt32Ty(TheContext), Alloca, "flag");
    Value *CondV = Builder.CreateICmpEQ(tmp, ConstantInt::get(TheContext, APInt(32, 0)));
    
    
    Function *f = Builder.GetInsertBlock()->getParent();
  
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", f);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", f);
    BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont", f);
    
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);
    
    Builder.SetInsertPoint(ThenBB);
    vector<Value *> ArgsV;
    ArgsV.push_back(Str);
    ArgsV.push_back(v);
    Builder.CreateCall(PrintfFja, ArgsV, "printfCall");
    Builder.CreateBr(MergeBB);
    ThenBB = Builder.GetInsertBlock();
    
    Builder.SetInsertPoint(ElseBB);
    ArgsV.clear();
    ArgsV.push_back(Str1);
    ArgsV.push_back(v);
    Builder.CreateCall(PrintfFja, ArgsV, "printfCall");
    Builder.CreateBr(MergeBB);
    ElseBB = Builder.GetInsertBlock();
    
    Builder.SetInsertPoint(MergeBB);
    return v;
}

PrintExprAST::~PrintExprAST() {
  delete Expression;
}


Value* FunctionExprAST::codegen() const {
  Function* f = TheModule->getFunction(Name);
  
  if(!f) {
    vector<Type*> vec_tmp;
    for (unsigned i = 0; i < args.size(); i++)
      vec_tmp.push_back(Type::getInt32Ty(TheContext));
      
    // cout << "Stigao 2" << endl;
    
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(TheContext), vec_tmp, false);
    
    f = Function::Create(FT, Function::ExternalLinkage, Name, TheModule);
    unsigned i = 0;
    for (auto &a : f->args())
    a.setName(args[i++]);
  }

  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", f);
  
  Builder.SetInsertPoint(BB);

  // cout << "Stigao 3" << endl;
  
  NamedValues.clear();
  for (auto &a : f->args()) {
    AllocaInst *Alloca = CreateEntryBlockAlloca(f, string(a.getName()));
    NamedValues[string(a.getName())] = Alloca;
    Builder.CreateStore(&a, Alloca);
  }
  
  Value* tmp = blok->codegen();
  
  // cout << "Stigao 4" << endl;
  
  if (tmp != nullptr) {
    Builder.CreateRet(tmp);
    verifyFunction(*f);    
    return f;
  }

  f->eraseFromParent();
  
  // cout << "Stigao 5" << endl;
  
  return nullptr;
}

FunctionExprAST::~FunctionExprAST() {
  
}

Value* CallExprAST::codegen() const {
  Function *f = TheModule->getFunction(Callee);
  if (!f) {
    cerr << "Poziv nedefinisane funckije " << Callee << endl;
    return nullptr;
  }

  if (Args.size() != f->arg_size()) {
    cerr << "Funkcija " << Callee << " je pozvana sa neodgovarajucim brojem argumenata" << endl;
    return nullptr;
  }

  vector<Value*> ArgsV;
  for (unsigned i = 0; i < Args.size(); i++) {
    Value *tmp = Args[i]->codegen();
    if (!tmp)
      return nullptr;
    ArgsV.push_back(tmp);
  }

  return Builder.CreateCall(f, ArgsV, "calltmp");
}

CallExprAST::~CallExprAST() {
  for (vector<ExprAST*>::iterator i = Args.begin(); i != Args.end(); i++)
    delete *i;
}

Value* SeqExprAST::codegen() const {
  Value *tmp = NULL;
  for (auto &c : cmds) {
    tmp = c->codegen();
    if (tmp == NULL)
      return NULL;
  }
  return tmp;
}

void initMain() {
       /* Module i builder */
  TheModule = new Module("my module", TheContext);
  
  vector<Type*> v(0);
  FunctionType* FT2 = FunctionType::get(Type::getInt32Ty(TheContext), v, false);
  MainFja = Function::Create(FT2, Function::ExternalLinkage, "main", TheModule);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", MainFja);
  Builder.SetInsertPoint(BB);


  /* printf fja */
  FunctionType *FT1 = FunctionType::get(IntegerType::getInt32Ty(TheContext), PointerType::get(Type::getInt8Ty(TheContext), 0), true);
    PrintfFja = Function::Create(FT1, Function::ExternalLinkage, "printf", TheModule);  
  
}

void endMain() {
    Builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 0)));
    verifyFunction(*MainFja);
    
    /* ispis llvm koda */
    TheModule->print(outs(), nullptr);

    /* brisanje modula */
    delete TheModule;
}

AllocaInst* CreateEntryBlockAlloca(Function *f, string s) {
  IRBuilder<> TmpBuilder(&(f->getEntryBlock()), f->getEntryBlock().begin());
  return TmpBuilder.CreateAlloca(Type::getInt32Ty(TheContext), 0, s);
}
