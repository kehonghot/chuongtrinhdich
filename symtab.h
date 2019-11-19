/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "token.h"

//cac kieu du lieu
enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_ARRAY
};

//phan loai doi tuong
enum ObjectKind {
  OBJ_CONSTANT,
  OBJ_VARIABLE,
  OBJ_TYPE,
  OBJ_FUNCTION,
  OBJ_PROCEDURE,
  OBJ_PARAMETER,
  OBJ_PROGRAM
};

enum ParamKind {
  PARAM_VALUE,
  PARAM_REFERENCE
};

struct Type_ {
  enum TypeClass typeClass;
  //danh cho mang
  int arraySize;
  struct Type_ *elementType;
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;


struct ConstantValue_ {
  enum TypeClass type;
  union {
    int intValue;
    char charValue;
  };
};

typedef struct ConstantValue_ ConstantValue;

struct Scope_;
struct ObjectNode_;
struct Object_;

struct ConstantAttributes_ {
  ConstantValue* value;
};

struct VariableAttributes_ {
  Type *type;
  struct Scope_ *scope;
};

struct TypeAttributes_ {
  Type *actualType;
};

struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;
  struct Scope_* scope;
};

struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;
  Type* returnType;
  struct Scope_ *scope;
};

struct ProgramAttributes_ {
  struct Scope_ *scope;
};

struct ParameterAttributes_ {
  enum ParamKind kind;
  Type* type;
  struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

//thuoc tinh cua doi tuong tren bang ky hieu
struct Object_ {
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union {
    ConstantAttributes* constAttrs;
    VariableAttributes* varAttrs;
    TypeAttributes* typeAttrs;
    FunctionAttributes* funcAttrs;
    ProcedureAttributes* procAttrs;
    ProgramAttributes* progAttrs;
    ParameterAttributes* paramAttrs;
  };
};

typedef struct Object_ Object;

struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;

//pham vi cua 1 block
struct Scope_ {
  //danh sach cac doi tuong trong block
  ObjectNode *objList;
  //ham, thu tuc, chuong trinh tuong ung block
  Object *owner;
  //pham vi bao ngoai
  struct Scope_ *outer;
};

typedef struct Scope_ Scope;

//bang ki hieu
struct SymTab_ {
  //chuong trinh chinh
  Object* program;
  //pham vi hien tai
  Scope* currentScope;
  //cac doi tuong toan cuc: WRITEI, WRITEC, WRITELN, READI, READC
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;

Type* makeIntType(void);
Type* makeCharType(void);
Type* makeArrayType(int arraySize, Type* elementType);
//sao chep kieu, co le vay
Type* duplicateType(Type* type);
int compareType(Type* type1, Type* type2);
void freeType(Type* type);

//tao cac gia tri hang so
ConstantValue* makeIntConstant(int i);
ConstantValue* makeCharConstant(char ch);
ConstantValue* duplicateConstantValue(ConstantValue* v);

Scope* createScope(Object* owner, Scope* outer);

//tao cac doi tuong
Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);

//dung cho semantics
Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
//dang ki doi tuong voi block hien tai
void declareObject(Object* obj);

#endif
