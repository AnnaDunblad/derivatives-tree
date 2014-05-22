#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<iostream> // For debug purpose

//#include "Tree.h"
#include "Node.h"
class Expression
{
 private: 
  std::string _str;
  bool isOperator(char);
  //bool isParenthesis(char);
  bool isNumber(char);
  int preProcess();
  int getHighestPrecendence(std::string);
  std::string fixParenthesis(std::string);
  void recursive(Node*, std::string);
  std::string trim(std::string);
 public:
  Expression(std::string);
  Node toTree();
};
#endif
