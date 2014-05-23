#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream> // cin and cout
#include <sstream> // To convert string to integer
#include <algorithm>    // std::max
#include <vector>    // std::vector

#include "Expression.h"
class Expression;

// Operators in precedence order
enum Operator{
  MULT, DIV, ADD, SUB, NUM
};



// Class representing a binary tree
class Node{

 private: 
  Node* _rightChildren;
  Node* _leftChildren;
  //Node _parent;
  Operator _operator;
  std::string _data; // If _operator is NUM we keep the number in _data

  int maxLevel(Node* node);
  bool isAllElementsNull(std::vector<Node*> list);
  void printTreeInternal(std::vector<Node*> nodes, int level, int maxLevel);
 


 public: 
  //Node(Operator _operator, Node* leftChildren, Node* rightChildren);
  Node(); // Empty constructor that we use when creating three
  //Node getParent();
  Node* getRight();
  Node* getLeft();
  void setRight(Node* node);
  void setLeft(Node* node);
  bool setOperator(char op);
  bool setOperator(Operator op);
  void setData(std::string data);
  std::string getData();
  
  // Return a printable representation of this tree
  void printTree();
  
  Expression toExpression();
  
  // Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
  void shorten();
  
  // Derive this tree, recursive
  void derive();
  
  
};

  
#endif  
