#include "assign_expr.hh"

#include <codegen/codegen.hh>

void AssignExpr::analyze(Symbol_table* symbol_table, reasonT focus) {
  CompoundExpr::analyze(symbol_table, focus);
  auto left_type = left->evaluate_type(symbol_table);
  auto right_type = right->evaluate_type(symbol_table);
  if (!right_type->coerce(left_type, symbol_table))
    ReportError::IncompatibleOperands(op, left_type, right_type);
}

void AssignExpr::emit(CodeGenerator* codegen, Frame_allocator* frame_allocator, Symbol_table* symbol_table) {
  CompoundExpr::emit(codegen, frame_allocator, symbol_table);
  auto left_location = left->get_frame_location();
  auto right_location = right->get_frame_location();
  frame_location = left->assign(right_location, codegen);
}
