#ifndef TREE_H
#define TREE_H

#include <string>
#include "Expression.h"

class Tree{
 private: 
  int Tree::maxLvl(Node* node);
  bool Tree::isAllElementsNull(std::vector<Node*> list);
  void Tree::printNodeInternal(std::vector<Node*> nodes, int level, int maxLevel);

 public: 
    
  // Takes a string expression and convert it to a tree, recursively, returns 1 if successful
  int decode(Expression expression);
  
  // Return a printable representation of this tree (i.e. convert it to an expression)
  std::string toString();
  
  // Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
  void Short();
  
  // Derive this tree, recursive
  void derive();
  
  // Print this tree
  void Tree::printNode(Node* root);
};

#endif
