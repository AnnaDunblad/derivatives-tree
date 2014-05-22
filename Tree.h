#ifndef TREE_H
#define TREE_H

#include <string>
#include "Expression.h"

class Tree{
 
 public: 
    
  // Takes a string expression and convert it to a tree, recursively, returns 1 if successful
  int decode(Expression expression);
  
  // Return a printable representation of this tree (i.e. convert it to an expression)
  std::string toString();
  
  // Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
  void Short();
  
  // Derive this tree, recursive
  void derive();
  
};

#endif
