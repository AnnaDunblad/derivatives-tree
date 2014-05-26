#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream> // cin and cout
#include <sstream> // To convert string to integer
#include <algorithm>    // std::max
#include <vector>    // std::vector
#include <cmath>     // pow()
#include <stdexcept>

#include "Expression.h"
class Expression;


// Class representing a binary tree
class Node{

 private: 
  Node* _rightChildren;
  Node* _leftChildren;
  Node* _parent;
  //Operator _operator;
  std::string _data; // If _operator is NUM we keep the number in _data

  int maxLevel(Node* node);
  bool isAllElementsNull(std::vector<Node*>);
  void printTreeInternal(std::vector<Node*>, int, int);
  int shorten(Node*);
  void setParent(Node*);
  char getOperator();
  bool isNumeric();
  float getNumber();
  static std::string doOperation(float,char,float);
 public: 
  //Node(Operator _operator, Node* leftChildren, Node* rightChildren);
  Node(); // Empty constructor that we use when creating three
  void changeChild(Node*, Node*);
  Node* getParent();
  Node* getRight();
  Node* getLeft();
  void setRight(Node*, Node*);
  void setLeft(Node*, Node*);
  //bool setOperator(char);
  //bool setOperator(Operator);
  void setData(std::string);
  std::string getData();
  
  // Return a printable representation of this tree
  void printTree();
  
  Expression toExpression();
  
  // Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
  void shorten();
  
  // Derive this tree, recursive
  Node* derive(std::string var);   //overload method to return result of derive
  Node* derive(std::string variable, Node* node); //recursive derive
  void multDerive(std::string var, Node* node); //derive mutiplication
 // void divDerive(std::string var, Node* node); //derive divisions
  Node* copyNode(Node* node);

  
};

  
#endif  
