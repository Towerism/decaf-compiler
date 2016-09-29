#include "symbol_table.hh"

#include <algorithm>

#include <util/errors.hh>

#include "decl/ast_decl.hh"

void Symbol_table::declare(Decl* declaration) {
  detect_previous_declaration(declaration);
  auto class_declaration = dynamic_cast<ClassDecl*>(declaration);
  if (class_declaration != nullptr) {
    classes.declare(class_declaration);
    return;
  }
  auto interface_declaration = dynamic_cast<InterfaceDecl*>(declaration);
  if (interface_declaration != nullptr) {
    interfaces.declare(interface_declaration);
    return;
  }
  auto variable_declaration = dynamic_cast<VarDecl*>(declaration);
  if (variable_declaration != nullptr) {
    variables.declare(variable_declaration);
    return;
  }
  auto function_declaration = dynamic_cast<FnDecl*>(declaration);
  if (function_declaration != nullptr) {
    functions.declare(function_declaration);
    return;
  }
}

#include <iostream>
void Symbol_table::add_virtual(NamedType* interface_type, Decl* declaration) {
  Decl* prev_decl = virtuals.contains(declaration->getName());
  if (prev_decl)
    ReportError::DeclConflict(declaration, prev_decl);
  auto function_declaration = dynamic_cast<FnDecl*>(declaration);
  virtuals.add(interface_type->getName(), function_declaration);
}

void Symbol_table::detect_previous_declaration(Decl* new_declaration) {
  Decl* prev_decl = variables.contains(new_declaration->getName());
  if (!prev_decl) {
    prev_decl = functions.contains(new_declaration->getName());
  }
  if (!prev_decl)
    prev_decl = classes.contains(new_declaration->getName());
  if (!prev_decl)
    prev_decl = interfaces.contains(new_declaration->getName());
  if (prev_decl)
    ReportError::DeclConflict(new_declaration, prev_decl);
}

void Symbol_table::check_virtual(Decl* declaration) {
  auto virtual_function = virtuals.contains(declaration->getName());
  auto function = dynamic_cast<FnDecl*>(declaration);
  if (virtual_function && !function->matches_signature(virtual_function))
    ReportError::OverrideMismatch(function);
}

void Symbol_table::check_super(Decl* declaration) {
  if (!super)
    return;
  auto current = super;
  do {
    auto function = dynamic_cast<FnDecl*>(declaration);
    if (function) {
      auto super_function = current->functions.contains(function->getName());
      if (super_function && !function->matches_signature(super_function))
        ReportError::OverrideMismatch(function);
    }
    auto variable = dynamic_cast<VarDecl*>(declaration);
    if (variable) {
      auto super_variable = current->variables.contains(variable->getName());
      if (super_variable)
        ReportError::DeclConflict(variable, super_variable);
    }
    current = current->super;
  } while(current != nullptr);
}

void Symbol_table::check_virtuals_implemented(ClassDecl* class_decl, List<NamedType*>* interface_types) {
  interface_types->Apply([&](NamedType* type) {
      auto interface = Program::symbol_table.get_interface(type->getName());
      if (!interface) {
        return;
      }
      auto interface_virtuals = virtuals.get_functions(type->getName());
      bool not_fully_implemented = std::any_of(interface_virtuals->begin(), interface_virtuals->end(), [&](std::pair<std::string, FnDecl*> key_function) {
          auto function = key_function.second;
          auto function_implemented = functions.contains(function->getName());
          if (nullptr == function_implemented)
            function_implemented = find_inherited_function(function);
          return nullptr == function_implemented;
        });
      if (not_fully_implemented)
        ReportError::InterfaceNotImplemented(class_decl, type);
    });
}

// refactor this
FnDecl* Symbol_table::find_inherited_function(FnDecl* declaration) {
  if (!super)
    return nullptr;
  auto current = super;
  do {
    auto function = dynamic_cast<FnDecl*>(declaration);
    if (function) {
      auto super_function = current->functions.contains(function->getName());
      if (super_function)
        return super_function;
    }
    current = current->super;
  } while(current != nullptr);
  return nullptr;
}

bool Symbol_table::type_exists(std::string name) {
  return classes.contains(name) || interfaces.contains(name);
}

ClassDecl* Symbol_table::get_class(std::string name) {
  return classes.contains(name);
}

InterfaceDecl* Symbol_table::get_interface(std::string name) {
  return interfaces.contains(name);
}
