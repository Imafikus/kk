#include <iostream>
#include <string>

using namespace std;

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

/* Kontekst */
LLVMContext TheContext;

/* alokacija promenljive VarName tipa int */
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(TheContext), 0, VarName.c_str());
}

int main() {
  /* Module i builder */
  Module* TheModule = new Module("my module", TheContext);
  IRBuilder<> Builder(TheContext);
  cout << endl;

  /* printf fja */
  FunctionType *FT1 = FunctionType::get(IntegerType::getInt32Ty(TheContext), PointerType::get(Type::getInt8Ty(TheContext), 0), true);
  Function *PrintfFja = Function::Create(FT1, Function::ExternalLinkage, "printf", TheModule);

  /* fja main */
  vector<Type*> v(0);
  FunctionType* FT2 = FunctionType::get(Type::getInt32Ty(TheContext), v, false);
  Function *MainFja = Function::Create(FT2, Function::ExternalLinkage, "main", TheModule);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", MainFja);
  Builder.SetInsertPoint(BB);

  /* globalni string potreban za ispis vektora */
  Value *Str = Builder.CreateGlobalStringPtr("%u\n");
  Value *Str1 = Builder.CreateGlobalStringPtr("0x%x\n");

  /* ispis inta pozivom printf fje */
  vector<Value *> ArgsV;
  ArgsV.push_back(Str);
  ArgsV.push_back(ConstantInt::get(TheContext, APInt(32, 121)));
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko AND */
  Value *x1 = Builder.CreateAnd(ConstantInt::get(TheContext, APInt(32, 2)), ConstantInt::get(TheContext, APInt(32, 4)), "andtmp");
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko OR */
  x1 = Builder.CreateOr(ConstantInt::get(TheContext, APInt(32, 2)), ConstantInt::get(TheContext, APInt(32, 4)), "ortmp");
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko XOR */
  x1 = Builder.CreateXor(ConstantInt::get(TheContext, APInt(32, 2)), ConstantInt::get(TheContext, APInt(32, 4)), "xortmp");
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko shiftovanje u desno */
  x1 = Builder.CreateLShr(ConstantInt::get(TheContext, APInt(32, 2)), ConstantInt::get(TheContext, APInt(32, 4)), "shrtmp");
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko shiftovanje u levo */
  x1 = Builder.CreateShl(ConstantInt::get(TheContext, APInt(32, 2)), ConstantInt::get(TheContext, APInt(32, 4)), "shltmp");
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Logicko NOT */
  x1 = Builder.CreateNot(ConstantInt::get(TheContext, APInt(32, 1)), "nottmp");
  ArgsV[0] = Str1;
  ArgsV[1] = x1;
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* zavrsetak main fje */
  Builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 0)));
  verifyFunction(*MainFja);
  
  /* ispis llvm koda */
  TheModule->dump();

  /* brisanje modula */
  delete TheModule;
  
  return 0;
}
