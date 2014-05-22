#include <iostream>
#include <string>
#include "Expression.h"


int main()
{
  std::string input;
  std::cin >> input;
  
  Expression exp(input);
  
  exp.toTree();
}


