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

/* alokacija promenljive VarName u funkciji TheFunction tipa t */
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName, Type* t) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(t, 0, VarName.c_str());
}

int main() {
  /* Kontekst, module i builder */
  LLVMContext TheContext;
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
  Value *Str = Builder.CreateGlobalStringPtr("[%g, %g]\n");

  /* struktura vektor u ravni, sa x i y koordinatama tipa double */
  StructType *VektorTip = StructType::create(TheContext, "vektor");;
  vector<Type*> Members;
  Members.push_back(Type::getDoubleTy(TheContext));
  Members.push_back(Type::getDoubleTy(TheContext));
  VektorTip->setBody(Members);

  /* Alokacija lokalnog vektora u main fji */
  AllocaInst* Alloca = CreateEntryBlockAlloca(MainFja, "v1", VektorTip);

  /* postavljanje x koordinate vektora */
  vector<Value*> Idxs1;
  Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Value* GEP1 = Builder.CreateGEP(VektorTip, Alloca, Idxs1, "x_coord");
  Builder.CreateStore(ConstantFP::get(TheContext, APFloat(1.1)), GEP1);

  /* postavljanje y koordinate vektora */
  vector<Value*> Idxs2;
  Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 1)));
  Value* GEP2 = Builder.CreateGEP(VektorTip, Alloca, Idxs2, "y_coord");
  Builder.CreateStore(ConstantFP::get(TheContext, APFloat(2.2)), GEP2);
  /* instrukcija store moze da se koristi i za cuvanje vektora odjednom
   * Builder.CreateStore(Value* v, AllocaInst *alloca)
   */

  /* ucitavanje x i y koordinata vektora */
  Value *First = Builder.CreateLoad(Type::getDoubleTy(TheContext), GEP1, "x");
  Value *Second = Builder.CreateLoad(Type::getDoubleTy(TheContext), GEP2, "y");
  /* instrukcija load moze da ucita vektor odjednom
   * Builder.CreateLoad(VektorTip, Alloca, "name");
   */

  /* ispis vektora pozivom printf fje */
  vector<Value *> ArgsV;
  ArgsV.push_back(Str);
  ArgsV.push_back(First);
  ArgsV.push_back(Second);
  Builder.CreateCall(PrintfFja, ArgsV, "printfCall");

  /* Konstantni vektor */
  vector<Constant*> Coords;
  Coords.push_back(ConstantFP::get(TheContext, APFloat(3.3)));
  Coords.push_back(ConstantFP::get(TheContext, APFloat(4.4)));
  Constant* C = ConstantStruct::get(VektorTip, Coords);

  /* x i y koordinata */
  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First1 = Builder.CreateExtractValue(C, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second1 = Builder.CreateExtractValue(C, Idxs4, "y");

  /* ispis konstantnog vektora pozivom printf fje */
  vector<Value *> ArgsV1;
  ArgsV1.push_back(Str);
  ArgsV1.push_back(First1);
  ArgsV1.push_back(Second1);
  Builder.CreateCall(PrintfFja, ArgsV1, "printfCall");

  /* pamcenje pozicije gde Builder upisuje instrukcije */ 
  BasicBlock::iterator TTT1 = Builder.GetInsertPoint();
  BasicBlock* TTT2 = Builder.GetInsertBlock();
  /* i vracanje na to mesto */
  Builder.SetInsertPoint(TTT2, TTT1);

  /* zavrsetak main fje */
  Builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 0)));
  verifyFunction(*MainFja);
  
  /* ispis llvm koda */
  TheModule->print(outs(), nullptr);

  /* brisanje modula */
  delete TheModule;
  
  return 0;
}
