#ifndef OPERATOR_H
#define OPERATOR_H

#include <ast/node.hh>

class Operator : public Node {
protected:
  char tokenString[4];
public:
  Operator(yyltype loc, const char *tok);
  const char *GetPrintNameForNode() { return "Operator"; }
  void PrintChildren(int indentLevel);
};

#endif /* OPERATOR_H */