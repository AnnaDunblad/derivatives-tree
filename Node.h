#ifndef NODE_H
#define NODE_H

#include <string>
#include <sstream> // To convert string to integer

// Operators in precedence order
enum Operator{
  MULT, DIV, ADD, SUB, NUM
};

class Node{
  
 private: 
  Node* _rightChildren;
  Node* _leftChildren;
  //Node _parent;
  Operator _operator;
  std::string _data; // If _operator is NUM we keep the number in _data
 public: 
  Node(Operator _operator, Node* leftChildren, Node* rightChildren);
  Node(); // Empty constructor that we use when creating three
  //Node getParent();
  Node* getRightChildren();
  Node* getLeftChildren();
  void setRightChildren(Node* node);
  void setLeftChildren(Node* node);
  bool setOperator(char op);
  bool setOperator(Operator op);
  void setData(std::string data);
  std::string getData();
};
  
#endif  
