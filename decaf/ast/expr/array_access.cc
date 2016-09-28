#include "array_access.hh"

ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
  (base=b)->SetParent(this);
  (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
  base->Print(indentLevel+1);
  subscript->Print(indentLevel+1, "(subscript) ");
}