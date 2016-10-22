#ifndef DECL_H
#define DECL_H

#include <ast/closeable.hh>
#include <ast/emittable.hh>
#include <ast/node.hh>
#include <ast/identifier.hh>

class Decl : public Node, public Closeable, public Emittable
{
protected:
  Identifier *id;

public:
  Decl(Identifier *name);
  std::string getName() const { return id->getName(); }
  virtual void build_table() override {}

  virtual void emit(CodeGenerator* codegen, Frame_allocator* frame_allocator, Symbol_table* symbol_table) override {}

  friend std::ostream& operator<<(std::ostream& out, Decl* decl) {
    return out << decl->getName();
  }
  virtual void set_parent(Symbol_table& other) override {}
  virtual Type* get_type() = 0;
};

#endif /* DECL_H */
