#ifndef COMPOUND_EXPR_H
#define COMPOUND_EXPR_H

#include "expr.hh"
#include "operator.hh"

class CompoundExpr : public Expr {
protected:
  Operator *op;
  Expr *left, *right; // left will be NULL if unary

  bool left_is_compatible_with_right(Symbol_table* symbol_table);
public:
  CompoundExpr(Expr *lhs, Operator *op, Expr *rhs); // for binary
  CompoundExpr(Operator *op, Expr *rhs);             // for unary
  CompoundExpr(Expr *lhs, Operator *op);             // for postfix
  void PrintChildren(int indentLevel) override;
  void analyze(Symbol_table* symbol_table, reasonT focus) override;
  virtual void emit(CodeGenerator* codegen, Frame_allocator* frame_allocator, Symbol_table* symbol_table) override;
};

#endif /* COMPOUND_EXPR_H */
