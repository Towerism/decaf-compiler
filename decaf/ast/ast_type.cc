/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.hh"
#include "ast_decl.hh"
#include <string.h>


/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error");

Type::Type(const char *n) {
  Assert(n);
  typeName = strdup(n);
}

void Type::PrintChildren(int indentLevel) {
  printf("%s", typeName);
}


NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
  Assert(i != NULL);
  (id=i)->SetParent(this);
}

void NamedType::PrintChildren(int indentLevel) {
  id->Print(indentLevel+1);
}

void NamedType::analyze(Scope_stack& scope_stack) {
  analyze(scope_stack, LookingForType);
}

void NamedType::analyze(Scope_stack& scope_stack, reasonT focus) {
  if (!Program::symbol_table.type_exists(id->getName()))
    ReportError::IdentifierNotDeclared(id, focus);
}

ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
  Assert(et != NULL);
  (elemType=et)->SetParent(this);
}
void ArrayType::PrintChildren(int indentLevel) {
  elemType->Print(indentLevel+1);
}

void ArrayType::analyze(Scope_stack& scope_stack) {
  elemType->analyze(scope_stack);
}

void ArrayType::analyze(Scope_stack& scope_stack, reasonT focus) {
  elemType->analyze(scope_stack, focus);
}