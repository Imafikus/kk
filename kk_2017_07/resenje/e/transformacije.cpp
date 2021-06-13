#include "transformacije.hpp"
#include <cmath>
#include <map>

using namespace std;



Vektor Scaling::transform(const Vektor& v) const {
  return v * _num;
}

Vektor Rotation::transform(const Vektor& v) const {
  double fi = _num + atan(v.getY()/v.getX());
  double r = sqrt(v.getX()*v.getX() + v.getY()*v.getY());
  return Vektor(cos(fi)*r, sin(fi)*r);
}

extern map<string, Transformation*> tablica_transformacija;
Vektor TransformationId::transform(const Vektor& v) const {
  map<string, Transformation*>::iterator tmp = tablica_transformacija.find(_name);
  if (tmp == tablica_transformacija.end()) {
    cerr << "Transformacija " << _name << " nije definisana" << endl;
    exit(EXIT_FAILURE);
  }
  return tmp->second->transform(v);
}

Vektor Composition::transform(const Vektor& v) const {
  return _l->transform(_r->transform(v));
}

Composition::~Composition() {
  delete _l;
  delete _r;
}

extern map<string, AllocaInst*> NamedValues;
extern IRBuilder<> Builder;
extern Module* TheModule;
extern StructType *VektorTip;
extern FunctionPassManager *TheFPM;
extern LLVMContext TheContext;
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName, Type* t);

Value* Scaling::codegen() const {
  // tip fje
  vector<Type*> Argumenti(1, VektorTip);
  FunctionType *FT = FunctionType::get(VektorTip, Argumenti, false);

  // za svako skaliranje pravimo po jednu fju ako vec nije napravljena
  Function *TheFunction = TheModule->getFunction("scaling" + to_string(_num));
  if (!TheFunction)
    TheFunction = Function::Create(FT, Function::ExternalLinkage, "scaling" + to_string(_num), TheModule);

  // ime argumenta fje je uvek ARG
  for (auto &Arg : TheFunction->args())
    Arg.setName("ARG");

  // Pravimo novi blok
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  // Pre nego sto postavimo insert point, zapamtimo gde smo stali, kako
  // bismo se posle pravljenja fje vratili na pravo mesto
  BasicBlock::iterator TTT1 = Builder.GetInsertPoint();
  BasicBlock* TTT2 = Builder.GetInsertBlock();
  Builder.SetInsertPoint(BB);

  // kopiramo argumente fje (uvek imamo samo jedan)
  for (auto &Arg : TheFunction->args()) {
    AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), VektorTip);
    NamedValues[Arg.getName()] = Alloca;
    Builder.CreateStore(&Arg, Alloca);
  }

  // ucitamo vektor koji je argument fje
  Value* RetVal = Builder.CreateLoad(VektorTip, NamedValues["ARG"], "arg");

  // nadjemo njegovu x i y koordinatu
  vector<unsigned> Idxs1;
  Idxs1.push_back(0);
  Value* First = Builder.CreateExtractValue(RetVal, Idxs1, "x");
  vector<unsigned> Idxs2;
  Idxs2.push_back(1);
  Value* Second = Builder.CreateExtractValue(RetVal, Idxs2, "y");

  // pravimo novi rezultujuci vektor
  AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, "result", VektorTip);
  
  // postavljanje x koordinate vektora
  vector<Value*> Idxs11;
  Idxs11.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs11.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Value* GEP1 = Builder.CreateGEP(VektorTip, Alloca, Idxs11, "x_coord");
  Builder.CreateStore(Builder.CreateFMul(First, ConstantFP::get(TheContext, APFloat(_num)), "multmp"), GEP1);
  
  // postavljanje y koordinate vektora
  vector<Value*> Idxs22;
  Idxs22.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs22.push_back(ConstantInt::get(TheContext, APInt(32, 1)));
  Value* GEP2 = Builder.CreateGEP(VektorTip, Alloca, Idxs22, "y_coord");
  Builder.CreateStore(Builder.CreateFMul(Second, ConstantFP::get(TheContext, APFloat(_num)), "multmp"), GEP2);

  // povratna vrednost fje
  Builder.CreateRet(Builder.CreateLoad(Alloca));
  
  // validacija i optimizacija
  verifyFunction(*TheFunction);
  TheFPM->run(*TheFunction);

  // vracmo insert point na staro mesto
  Builder.SetInsertPoint(TTT2, TTT1);

  // fja je napravljena
  return TheFunction;  
}

Value* Rotation::codegen() const {
  return NULL;
}

Value* TransformationId::codegen() const {
  return NULL;
}

Value* Composition::codegen() const {
  return NULL;
}
