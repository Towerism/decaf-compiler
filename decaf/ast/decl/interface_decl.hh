#ifndef INTERFACE_DECL_H
#define INTERFACE_DECL_H

#include <iostream>

#include <util/list.hh>
#include <ast/node.hh>
#include <ast/identifier.hh>
#include <ast/closeable.hh>
#include <ast/symbol_table.hh>
#include <ast/decl/decl.hh>

class InterfaceDecl : public Decl
{
protected:
  List<Decl*> *members;
  Symbol_table symbol_table;

public:
  InterfaceDecl(Identifier *name, List<Decl*> *members);
  const char *GetPrintNameForNode() override { return "InterfaceDecl"; }
  void PrintChildren(int indentLevel) override;
  void build_table() override;
  void set_parent(Symbol_table& other) override { symbol_table.set_parent(other); }

  List<Decl*>* get_members() { return members; }
  Type* get_type() override { return new NamedType(id); }
  Symbol_table* get_table() { return &symbol_table; }
};

#endif /* INTERFACE_DECL_H */
