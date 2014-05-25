#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<iostream> // For debug purpose

#include "Node.h"
class Node;

class Expression
{
 private: 
  std::string _str;

  bool isOperator(char);
  //bool isParenthesis(char);
  bool isNumber(char);
  int preProcess();
  int getHighestPrecedence(std::string);
  std::string fixParenthesis(std::string);
  void toTreeInternal(Node*, std::string);
  std::string trim(std::string);


 public:
  Expression(std::string);
  Node* toTree();
};
#endif
