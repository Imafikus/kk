#include "ast.hpp"

LLVMContext TheContext;
map<string, AllocaInst*> NamedValues;
IRBuilder<> Builder(TheContext);
Module* TheModule;
FunctionPassManager *TheFPM;


SynTree::~SynTree() {

}

Vektor ConstVector::interpret() const {
  return _v;
}

extern StructType *VektorTip;

Value* ConstVector::codegen() const {
  vector<Constant*> Coords;
  Coords.push_back(ConstantFP::get(TheContext, APFloat(_v.getX())));
  Coords.push_back(ConstantFP::get(TheContext, APFloat(_v.getY())));
  return ConstantStruct::get(VektorTip, Coords);
}

extern map<string, Vektor> tablica_vektora;

Vektor Variable::interpret() const {
  map<string, Vektor>::iterator tmp = tablica_vektora.find(_name);
  if (tmp == tablica_vektora.end()) {
    cerr << "Vektor " + _name + " nije definisan" << endl;
    exit(EXIT_FAILURE);
  }
  return tmp->second;
}

Value* Variable::codegen() const {
  AllocaInst* tmp = NamedValues[_name];
  if (tmp == NULL) {
    cerr << "Vektor " + _name + " nije definisan" << endl;
    return NULL;
  }
  return Builder.CreateLoad(VektorTip, tmp, _name.c_str());
}

Vektor Add::interpret() const {
  return _l->interpret() + _r->interpret();
}

Add::~Add() {
  delete _l;
  delete _r;
}

Value* Add::codegen() const {
  Value *l = _l->codegen();
  Value *r = _r->codegen();
  if (l == NULL || r == NULL)
    return NULL;
  
  vector<unsigned> Idxs1;
  Idxs1.push_back(0);
  Value* First1 = Builder.CreateExtractValue(l, Idxs1, "x");
  vector<unsigned> Idxs2;
  Idxs2.push_back(1);
  Value* Second1 = Builder.CreateExtractValue(l, Idxs2, "y");
  
  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First2 = Builder.CreateExtractValue(r, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second2 = Builder.CreateExtractValue(r, Idxs4, "y");

  Value* First = Builder.CreateFAdd(First1, First2, "addtmp1");
  Value* Second = Builder.CreateFAdd(Second1, Second2, "addtmp2");
  
  vector<Constant*> Coords;
  Coords.push_back((Constant*)First);
  Coords.push_back((Constant*)Second);
  return ConstantStruct::get(VektorTip, Coords);
}

Vektor Sub::interpret() const {
  return _l->interpret() - _r->interpret();
}

Sub::~Sub() {
  delete _l;
  delete _r;
}

Value* Sub::codegen() const {
  Value *l = _l->codegen();
  Value *r = _r->codegen();
  if (l == NULL || r == NULL)
    return NULL;
  
  vector<unsigned> Idxs1;
  Idxs1.push_back(0);
  Value* First1 = Builder.CreateExtractValue(l, Idxs1, "x");
  vector<unsigned> Idxs2;
  Idxs2.push_back(1);
  Value* Second1 = Builder.CreateExtractValue(l, Idxs2, "y");
  
  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First2 = Builder.CreateExtractValue(r, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second2 = Builder.CreateExtractValue(r, Idxs4, "y");

  Value* First = Builder.CreateFSub(First1, First2, "subtmp1");
  Value* Second = Builder.CreateFSub(Second1, Second2, "subtmp2");
  
  vector<Constant*> Coords;
  Coords.push_back((Constant*)First);
  Coords.push_back((Constant*)Second);
  return ConstantStruct::get(VektorTip, Coords);
}

Vektor Mul::interpret() const {
  return _r->interpret() * _d;
}

Mul::~Mul() {
  delete _r;
}

Value* Mul::codegen() const {
  Value *r = _r->codegen();
  if (r == NULL)
    return NULL;
  
  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First2 = Builder.CreateExtractValue(r, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second2 = Builder.CreateExtractValue(r, Idxs4, "y");

  Value* First = Builder.CreateFMul(ConstantFP::get(TheContext, APFloat(_d)), First2, "multmp1");
  Value* Second = Builder.CreateFMul(ConstantFP::get(TheContext, APFloat(_d)), Second2, "multmp2");
  
  vector<Constant*> Coords;
  Coords.push_back((Constant*)First);
  Coords.push_back((Constant*)Second);
  return ConstantStruct::get(VektorTip, Coords);
  
}

Vektor Print::interpret() const {
  cout << _r->interpret() << endl;
  return Vektor();
}

Print::~Print() {
  delete _r;
}

extern Function *PrintfFja;
extern Value *Str;

Value* Print::codegen() const {
  Value* r = _r->codegen();
  if (r == NULL)
    return NULL;

  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First1 = Builder.CreateExtractValue(r, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second1 = Builder.CreateExtractValue(r, Idxs4, "y");

  /* ispis konstantnog vektora pozivom printf fje */
  vector<Value *> ArgsV1;
  ArgsV1.push_back(Str);
  ArgsV1.push_back(First1);
  ArgsV1.push_back(Second1);
  return Builder.CreateCall(PrintfFja, ArgsV1, "printfCall");
}

Vektor Assign::interpret() const {
  map<string, Vektor>::iterator tmp = tablica_vektora.find(_name);
  if (tmp == tablica_vektora.end()) {
    cerr << "Vektor " + _name + " nije definisan" << endl;
    exit(EXIT_FAILURE);
  }  
  Vektor v = _r->interpret();
  tmp->second = v;
  return v;
}

Assign::~Assign() {
  delete _r;
}

Value* Assign::codegen() const {
  Value* tmp = _r->codegen();
  if (tmp == NULL)
    return NULL;
  AllocaInst* Alloca = NamedValues[_name];
  if (Alloca == NULL) {
    cerr << "Promenljiva " + _name + " nije definisana" << endl;
    return NULL;
  }

  vector<unsigned> Idxs3;
  Idxs3.push_back(0);
  Value* First1 = Builder.CreateExtractValue(tmp, Idxs3, "x");
  vector<unsigned> Idxs4;
  Idxs4.push_back(1);
  Value* Second1 = Builder.CreateExtractValue(tmp, Idxs4, "y");
  
  /* postavljanje x koordinate vektora */
  vector<Value*> Idxs1;
  Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Value* GEP1 = Builder.CreateGEP(VektorTip, Alloca, Idxs1, "x_coord");
  Builder.CreateStore(First1, GEP1);

  /* postavljanje y koordinate vektora */
  vector<Value*> Idxs2;
  Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
  Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 1)));
  Value* GEP2 = Builder.CreateGEP(VektorTip, Alloca, Idxs2, "y_coord");
  Builder.CreateStore(Second1, GEP2);
  
  return tmp;
}

Vektor Declaration::interpret() const {
  for (unsigned i = 0; i < _decls.size(); i++) {
    map<string, Vektor>::iterator tmp = tablica_vektora.find(_decls[i].first);
    if (tmp != tablica_vektora.end()) {
      cerr << "Vektor " + _decls[i].first + " je vec definisan" << endl;
      exit(EXIT_FAILURE);
    }  
    Vektor v = _decls[i].second->interpret();
    tablica_vektora[_decls[i].first] = v;
  }
  return Vektor();
}

Declaration::~Declaration() {
  for (unsigned i = 0; i < _decls.size(); i++)
    delete _decls[i].second;
}

extern Function *MainFja;
Value* Declaration::codegen() const {
  for (unsigned i = 0; i < _decls.size(); i++) {
    string Name = _decls[i].first;
    if (NamedValues[Name] != NULL) {
      cerr << "Vektor " << Name << " je vec deklarisan" << endl;
      return NULL;
    }
    Value* Tmp = _decls[i].second->codegen();
    AllocaInst* Alloca = CreateEntryBlockAlloca(MainFja, Name, VektorTip);

    vector<unsigned> Idxs3;
    Idxs3.push_back(0);
    Value* First1 = Builder.CreateExtractValue(Tmp, Idxs3, "x");
    vector<unsigned> Idxs4;
    Idxs4.push_back(1);
    Value* Second1 = Builder.CreateExtractValue(Tmp, Idxs4, "y");

    /* postavljanje x koordinate vektora */
    vector<Value*> Idxs1;
    Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
    Idxs1.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
    Value* GEP1 = Builder.CreateGEP(VektorTip, Alloca, Idxs1, "x_coord");
    Builder.CreateStore(First1, GEP1);
    
    /* postavljanje y koordinate vektora */
    vector<Value*> Idxs2;
    Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 0)));
    Idxs2.push_back(ConstantInt::get(TheContext, APInt(32, 1)));
    Value* GEP2 = Builder.CreateGEP(VektorTip, Alloca, Idxs2, "y_coord");
    Builder.CreateStore(Second1, GEP2);
    
    NamedValues[Name] = Alloca;
  }
  return NULL;
}

Vektor Block::interpret() const {
  for (unsigned i = 0; i < _v.size(); i++)
    _v[i]->interpret();
  return Vektor();
}

Block::~Block() {
  for (unsigned i = 0; i < _v.size(); i++)
    delete _v[i];
}

Value* Block::codegen() const {
  for (unsigned i = 0; i < _v.size(); i++)
    _v[i]->codegen();
  return NULL;
}

Vektor TransformNode::interpret() const {
  return _trans->transform(_t->interpret());
}

TransformNode::~TransformNode() {
  delete _trans;
  delete _t;
}

Value* TransformNode::codegen() const {
  return NULL;
}

extern map<string, Transformation*> tablica_transformacija;
Vektor DeclarationT::interpret() const {
  map<string, Transformation*>::iterator tmp = tablica_transformacija.find(_name);
  if (tmp != tablica_transformacija.end()) {
    cerr << "Transformacija " + _name + " je vec definisana" << endl;
    exit(EXIT_FAILURE);    
  }
  tablica_transformacija[_name] = _t;
  return Vektor();
}

DeclarationT::~DeclarationT() {
  delete _t;
}

Value* DeclarationT::codegen() const {
  return NULL;
}

void InitializeModuleAndPassManager(void) {
  TheModule = new Module("my_module", TheContext);

  // Create a new pass manager attached to it.
  TheFPM = new FunctionPassManager(TheModule);

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  TheFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(createCFGSimplificationPass());
  TheFPM->add(createPromoteMemoryToRegisterPass());

  TheFPM->doInitialization();
}

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const string &VarName, Type* t) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(t, 0, VarName.c_str());
}

