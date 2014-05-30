#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<iostream> // For printing stuff (debug etc.)
#include<ostream> // For overloading << to cout

#include "Node.h"
class Node;

class Expression
{
 private: 

  // Variables
  static const char _allowedCharacters[];
  std::string _str;

  // Methods, mayority static because they don't use class variables, but parameters (usually in recursive calls)
  static bool isOperator(char);
  static bool isNumber(char);
  static bool isVariable(char);
  static bool isFunction(char);

  static void replaceString(std::string&, const std::string&, const std::string&);
  static std::string trim(std::string);
  static std::string fixParenthesis(std::string);

  static int getHighestPrecedence(std::string);
  int preProcess();
  void toTree(Node*, std::string);

     
 public:
  Node* toTree();
  bool checkError();

  Expression(std::string);

  friend std::ostream& operator<<(std::ostream&, const Expression&);

};
#endif
