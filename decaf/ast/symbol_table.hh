#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>

#include "declaration_table.hh"
#include "virtual_table.hh"

#include <functional>

#include <ast/identifier.hh>
#include <ast/type/named_type.hh>

#include <util/list.hh>
#include <util/utility.hh>

class Decl;
class ClassDecl;
class InterfaceDecl;
class VarDecl;
class FnDecl;

class Symbol_table {
public:
  void declare(Decl* declaration);
  void add_virtual(NamedType* interface_type, Decl* declaration);
  bool type_exists(std::string name);

  void check_virtual(Decl* declaration);
  void check_super(Decl* declaration);
  void set_super(Symbol_table& super_table) { super = &super_table; }
  void check_virtuals_implemented(ClassDecl* class_decl, List<NamedType*>* interface_types);
  void set_parent(Symbol_table& parent_table) { parent = &parent_table; }
  VarDecl* check_variable_declared(Identifier* identifier,
                                std::function<void()> error_action = []() {});
  FnDecl* check_function_declared(Identifier* identifier,
                                std::function<void()> error_action = []() {});
  // call back called with num args expected, and num given
  bool check_function_args_length(Identifier* identifier, List<Expr*>* args,
                                  std::function<void(int, int)> = [](int,int) {});
  // call back called with arg, arg index, type given, type expected
  bool check_function_args(Identifier* identifier, List<Expr*>* args,
                           List<Type*>* arg_types,
                           std::function<void(Expr*, int, Type*, Type*)>
                           = [](Expr*,int,Type*,Type*) {});

  ClassDecl* get_class(std::string name);
  InterfaceDecl* get_interface(std::string name);
  VarDecl* get_variable(std::string name);
  void set_return_type(Type* type) { return_type = type; }
  void set_this_type(Type* type) { this_type = type; }
  Type* find_return_type();
  Type* find_this_type();
  bool class_extends_type(Type* subclass, Type* extends);
  Symbol_table* get_table_for_functions(Type* type);
  Symbol_table* get_table_for_variables(Type* type);

private:
  Declaration_table<ClassDecl> classes;
  Declaration_table<InterfaceDecl> interfaces;
  Declaration_table<VarDecl> variables;
  Declaration_table<FnDecl> functions;
  Virtual_table virtuals;
  Type* return_type = nullptr;
  Type* this_type = nullptr;

  Symbol_table* super = nullptr;
  Symbol_table* parent = nullptr;

  void detect_previous_declaration(Decl* new_declaration);
  FnDecl* find_inherited_function(std::string name);
  VarDecl* find_inherited_variable(std::string name);
  VarDecl* find_parent_variable(std::string name);
};

#endif // SYMBOL_TABLE_H
