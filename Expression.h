#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<iostream> // For debug purpose

#include "Node.h"
class Node;

class Expression
{
 private: 
  static char _allowedCharacters[];

  std::string _str;

  bool isOperator(char);
  //bool isParenthesis(char);
  bool isNumber(char);
  bool isVariable(char);
  bool isFunction(char);
  void replaceString(std::string&, const std::string&, const std::string&);
  int preProcess();
  int getHighestPrecedence(std::string);
  std::string fixParenthesis(std::string);
  void toTreeInternal(Node*, std::string);
  std::string trim(std::string);


 public:
  Expression(std::string);
  Node* toTree();
  bool checkError();

};
#endif
