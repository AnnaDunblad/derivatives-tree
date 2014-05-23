#include <iostream>
#include <string>
#include <algorithm>    // std::max
#include <vector>    // std::vector
#include "Expression.h"

int main(int argc, char* argv[])
{ 
  std::string input;
  if(argc > 1){
    input = std::string(argv[1]);
    std::cout << "Expression is: " << input << std::endl;
  }else{
    std::cout << "Introduce expression"<<std::endl;
    std::cin >> input;
  }
  Expression exp(input);
  exp.toTree().printNode();
}
