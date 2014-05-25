#include <iostream>
#include <string>
#include "Expression.h"

int main()
{
	std::string input;

	std::cout<< "introduce expression"<<std::endl;
	std::cin>> input;
	char variable = 'x';

	Expression exp(input);
	exp.toTree()->derive(variable)->printTree();
	

	  
}