#include "Expression.h"


Expression::Expression(std::string str)
{
  _str = str;
}

bool Expression::isOperator(char ch)
{
  return ch=='*' || ch=='+' || ch=='/' || ch=='-';
}

bool Expression::isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}

// Removes the outermost parenthesis (but does not handle for example (a+b) * (b+c) correct. 
std::string Expression::trim(std::string str)
{
  for(unsigned int i = 0; i < str.length()/2; i++){
    if(str[i]=='(' && str[str.length()-i-1] == ')'){
      str.erase(i,1);
      str.erase(str.length()-i-1,1);
      i--;
    }
  }
  return str;
}

// Creates an tree from the string
Node* Expression::toTree(){
  preProcess();
  Node* central = new Node();
  recursive(central, _str);
  
  return central;
}

void Expression::recursive(Node* currNode, std::string currStr)
{ 
  std::cout << currStr << std::endl;
  int pos = getHighestPrecendence(currStr);

  // the currStr doesn't contain any operator
  if(pos==-1){
    currNode->setData(trim(currStr));
    std::cout << "nod satt till: "<< currNode->getData() << std::endl;
  }
  else{
    currNode->setData(std::string(1,currStr[pos])); //TODO: setOperator()
    std::cout << "operand satt till: " << currStr[pos] << std::endl;
    currNode->setRight(new Node());
    currNode->setLeft(new Node());

    recursive(currNode->getLeft(), fixParenthesis(currStr.substr(0,pos)));
    recursive(currNode->getRight(), fixParenthesis(currStr.substr(pos+1)));
  }
}


// Prepare the string before we split it in parts
int Expression::preProcess()
{
  
  // Prepare the string for the algorithm:
  // 1. Add * between character and paranthesis. 
  // 2. Change c^3 to c*c*c
  // TODO: Add paranthesis around multi character numbers (for example 42) to treat them as one entity. But can have problems with for example 4c. Needs to add a * inbetween them. 
  
  char lastCharacter = 0;
  for(unsigned int i = 0; i < _str.length(); ++i) {
    // 1. * Before parantesis
    if(_str[i] == '(' && i>0 && !isOperator(lastCharacter) && lastCharacter!='('){
      _str.insert(i,1,'*');
      i++;
    }
    
    // Note: This makes (a+b)(c+d) fails, and only adds support for rare expressions on the form (a+b)K
    //if(lastCharacter == ')' && !isOperator(str[i])){
    //str.insert(i,"*");
    //i++;
    //}
    
    // 2. Fix powers
    if(_str[i] == '^'){
      // Does next character exists? It has to do for it to be an correct expression, in addition the exponent has to be a number (limited to 0-9)
      if(i+1==_str.length() || i==0 || !isNumber(_str[i+1]))
	return -1;
      
      // Make the part that we will add repeatly
      std::string expression = "";// + str[i-1];
      int parantesis = 0;
      for(int j = i-1; j>=0; --j){
	expression.insert(0,1,_str[j]);
	if(_str[j] == ')')
	  parantesis++;
	else if(_str[j] == '(')
	  parantesis--;
	if(parantesis==0)
	  break;
      }
      expression.insert(0,1,'*');
      std::cout << expression << std::endl;
      for(int j = '0'; j < _str[i+1] - 1; j++){
	_str.insert(i,expression);
	i+=expression.length();
      }
      // Remove the ^5 part of the string
      _str.erase(i,2);
    }
    
    // Update last character
    lastCharacter = _str[i];
  }
  return 0;
}


int Expression::getHighestPrecendence(std::string str)
{
  	// Search for the operator with highest precedence. 
	int numberOfParantesis = 0;
	int deepness = 0;
	int groundest = 255;
	int groundestPosition = -1;
	char lastOperador = 0;
	
	for(unsigned int i = 0; i < str.length(); ++i) {
		switch(str[i])
		{
			case '(':
				/*if(lastOperador=='n')
				{
					printf("%d: explicit * detected\n", i);
					deepness++;
					printf("%d: deepness increased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}*/
			        std::cout << i << ": ( detected" << std::endl;
				numberOfParantesis++;
				deepness++;
				break;
			case ')':
			        std::cout << i << ": ) detected" << std::endl;
				if(numberOfParantesis==0)
					return -1;
				numberOfParantesis--;
				deepness--;
				lastOperador='*'; // Changed from 'n' 22 May at 4:42, with 'n' I had problems with (d+4)*(a)+4 
				break;
			case '*':
			case '/':
			        std::cout << i << ": */ detected" << std::endl;
				if(lastOperador=='+')
				{
					deepness++;
					std::cout << i << ": deepness increased to "<< deepness << std::endl;
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					std::cout << i << ": groundestPosition = "<< i << std::endl;
				}
				lastOperador = '*';
				break;
			case '+':
			case '-':
			  std::cout << i << ": +- detected" << std::endl;
			  
			        // If this is a new operator
				if(lastOperador=='*')
				{
					deepness--;
					std::cout << i << ": deepness decreased to " << deepness << std::endl;
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					std::cout << i << ": groundestPosition = "<<i<<std::endl;
				}
				lastOperador = '+';
			default: 
				// Number (or variable detected)
				// TODO: Concat if several caracters, i.e.  8132
				//lastOperador='n';
				break;
		}
	}
	return groundestPosition;
}

// Removes a possible spare outer parantesis (this is due to the way I split the expressions at the operator with the highest predence, for example (4+5*3) will split as "(4" and "5*3)" )
std::string Expression::fixParenthesis(std::string str)
{
  int parenthesis = 0;
  for(unsigned int i = 0; i < str.length(); ++i)
    {
      if(str[i] == '(')
	parenthesis++;
      else if(str[i] == ')')
	parenthesis--;
    }
  // If we have an extra ) at the end we remove it 
  if(parenthesis==1 && str[0]=='(')
    str.erase(0,1);
  // If we have an extra ( at the beginning we remove it 
  else if(parenthesis==-1 && str[str.length()-1] == ')')
    str.erase(str.length()-1,1);
   
  return str;
}
