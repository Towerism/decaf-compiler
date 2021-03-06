#ifndef DECLARATION_TABLE_H
#define DECLARATION_TABLE_H

#include <string>
#include <unordered_map>

class Decl;

template <typename decl_t>
class Declaration_table {
public:
  void declare(decl_t* declaration) {
    std::string name = declaration->getName();
    declarations[name] = declaration;
  }

  void maybe_declare(Decl* declaration) {
    decl_t* decl = dynamic_cast<decl_t*>(declaration);
    if (decl)
      declare(decl);
  }

  decl_t* contains(std::string name) {
    auto key_value = declarations.find(name);
    if (key_value != declarations.end())
      return key_value->second;
    return nullptr;
  }

  typename std::unordered_map<std::string, decl_t*>::iterator begin() {
    return declarations.begin();
  }
  typename std::unordered_map<std::string, decl_t*>::iterator end() {
    return declarations.end();
  }
private:
  std::unordered_map<std::string, decl_t*> declarations;
};

#endif // DECLARATION_TABLE_H
