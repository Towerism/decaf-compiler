#ifndef PROGRAM_H
#define PROGRAM_H

#include <ast/node.hh>
#include <ast/closeable.hh>
#include <ast/emittable.hh>
#include <ast/symbol_table.hh>
#include <ast/decl/decl.hh>

#include <util/errors.hh>
#include <util/list.hh>

class Program : public Node, public Closeable
{
protected:
  List<Decl*> *decls;
public:
  Program(List<Decl*> *declList);
  const char *GetPrintNameForNode() override { return "Program"; }
  void PrintChildren(int indentLevel) override;
  void build_table() override;
  void analyze(reasonT focus = LookingForType) override;
  void set_parent(Symbol_table& other) override {}

  void emit();

  static Symbol_table symbol_table;
};

#endif /* PROGRAM_H */
