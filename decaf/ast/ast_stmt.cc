/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.hh"
#include "ast_type.hh"
#include "ast_decl.hh"
#include "ast_expr.hh"

Symbol_table Program::symbol_table;

Program::Program(List<Decl*> *d) {
  Assert(d != NULL);
  (decls=d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
  decls->PrintAll(indentLevel+1);
  printf("\n");
}

void Program::build_table() {
  decls->Apply([&](Decl* decl) { Program::symbol_table.declare(decl); });
  decls->Apply([&](Decl* decl) { decl->build_table(); });
}

void Program::analyze(reasonT focus) {
  build_table();

  decls->Apply([&](Decl* decl) { decl->analyze(focus); });
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
  Assert(d != NULL && s != NULL);
  (decls=d)->SetParentAll(this);
  (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
  decls->PrintAll(indentLevel+1);
  stmts->PrintAll(indentLevel+1);
}

void StmtBlock::build_table() {
  decls->Apply([&](Decl* decl) { symbol_table.declare(decl); });
  decls->Apply([&](Decl* decl) { decl->build_table(); });
  stmts->Apply([&](Stmt* stmt) { stmt->build_table(); });
}

void StmtBlock::analyze(reasonT focus) {
  decls->Apply([&](VarDecl* decl) { decl->analyze(focus); });
  stmts->Apply([&](Stmt* stmt) { stmt->analyze(focus); });
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {    Assert(t != NULL && b != NULL);
  (test=t)->SetParent(this);    (body=b)->SetParent(this);
}

void ConditionalStmt::build_table() {
  body->build_table();
}

void ConditionalStmt::analyze(reasonT focus) {
  body->analyze(focus);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) {    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
  (init=i)->SetParent(this);
  (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
  init->Print(indentLevel+1, "(init) ");
  test->Print(indentLevel+1, "(test) ");
  step->Print(indentLevel+1, "(step) ");
  body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::PrintChildren(int indentLevel) {
  test->Print(indentLevel+1, "(test) ");
  body->Print(indentLevel+1, "(body) ");
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) {    Assert(t != NULL && tb != NULL); // else can be NULL
  elseBody = eb;
  if (elseBody) elseBody->SetParent(this);
}

void IfStmt::build_table() {
  ConditionalStmt::build_table();
  if (elseBody) elseBody->build_table();
}

void IfStmt::analyze(reasonT focus) {
  ConditionalStmt::analyze(focus);
  if (elseBody) elseBody->analyze(focus);
}

void IfStmt::PrintChildren(int indentLevel) {
  test->Print(indentLevel+1, "(test) ");
  body->Print(indentLevel+1, "(then) ");
  if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}

CaseStmt::CaseStmt(yyltype loc, IntConstant* label, List<Stmt*>* body)
  : Stmt(loc), label(label), body(body) {
  this->label->SetParent(this);
  this->body->SetParentAll(this);
}

void CaseStmt::PrintChildren(int identLevel) {
  this->label->Print(identLevel+1, "(label) ");
  this->body->PrintAll(identLevel+1);
}

void CaseStmt::analyze(reasonT focus) {
  body->Apply([&](Stmt* stmt) { stmt->analyze(focus); });
}

void CaseStmt::build_table() {
  body->Apply([&](Stmt* stmt) { stmt->build_table(); });
}

DefaultStmt::DefaultStmt(yyltype loc, List<Stmt*>* body) : Stmt(loc), body(body) {
  this->body->SetParentAll(this);
}

void DefaultStmt::PrintChildren(int identLevel) {
  this->body->PrintAll(identLevel+1);
}

void DefaultStmt::analyze(reasonT focus) {
  body->Apply([&](Stmt* stmt) { stmt->analyze(focus); });
}

void DefaultStmt::build_table() {
  body->Apply([&](Stmt* stmt) { stmt->build_table(); });
}

SwitchStmt::SwitchStmt(yyltype loc, Expr* test, List<CaseStmt*>* cases, DefaultStmt* defaultStmt)
  : Stmt(loc), test(test), cases(cases), defaultStmt(defaultStmt) {
  this->test->SetParent(this);
  this->cases->SetParentAll(this);
  if (defaultStmt)
    this->defaultStmt->SetParent(this);
}

void SwitchStmt::PrintChildren(int identLevel) {
  this->test->Print(identLevel+1, "(test) ");
  this->cases->PrintAll(identLevel+1, NULL);
  this->defaultStmt->Print(identLevel+1, "(default) ");
}

void SwitchStmt::build_table() {
  cases->Apply([&](CaseStmt* caseStmt) { caseStmt->build_table(); });
  if (defaultStmt)
    defaultStmt->build_table();
}

void SwitchStmt::analyze(reasonT focus) {
  cases->Apply([&](CaseStmt* caseStmt) { caseStmt->analyze(focus); });
  if (defaultStmt)
    defaultStmt->analyze(focus);
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {    Assert(e != NULL);
  (expr=e)->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
  expr->Print(indentLevel+1);
}
PrintStmt::PrintStmt(List<Expr*> *a) {    Assert(a != NULL);
  (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
  args->PrintAll(indentLevel+1, "(args) ");
}


