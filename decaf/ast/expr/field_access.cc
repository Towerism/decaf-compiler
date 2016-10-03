#include "field_access.hh"

#include <util/utility.hh>

#include <ast/symbol_table.hh>
#include <ast/decl/decl.hh>

FieldAccess::FieldAccess(Expr *b, Identifier *f)  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
  Assert(f != NULL); // b can be be NULL (just means no explicit base)
  base = b;
  if (base) base->SetParent(this);
  (field=f)->SetParent(this);
}

void FieldAccess::PrintChildren(int indentLevel) {
  if (base) base->Print(indentLevel+1);
  field->Print(indentLevel+1);
}

void FieldAccess::analyze(Symbol_table* symbol_table, reasonT focus) {
  symbol_table->check_variable_declared(field);
}

Type* FieldAccess::evaluate_type(Symbol_table* symbol_table) {
  auto symbol = symbol_table->check_variable_declared(field);
  return symbol->get_type();
}
