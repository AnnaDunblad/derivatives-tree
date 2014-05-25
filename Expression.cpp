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
  toTreeInternal(central, _str);
  
  return central;
}

// Recursive method to create an tree from a string
void Expression::toTreeInternal(Node* currNode, std::string currStr)
{ 
  std::cout << currStr << std::endl;
  int pos = getHighestPrecedence(currStr);

  // the currStr doesn't contain any operator
  if(pos==-1){
    currNode->setData(trim(currStr));
    std::cout << "nod satt till: "<< currNode->getData() << std::endl;
  }
  else{
    currNode->setData(std::string(1,currStr[pos])); //TODO: setOperator()
    std::cout << "operand satt till: " << currStr[pos] << std::endl;
    currNode->setRight(new Node(),currNode);
    currNode->setLeft(new Node(),currNode);

    toTreeInternal(currNode->getLeft(), fixParenthesis(currStr.substr(0,pos)));
    toTreeInternal(currNode->getRight(), fixParenthesis(currStr.substr(pos+1)));
  }
}

// Check string for errors
//int Expression::checkError()
//{
  // The string can only contain "0-9, a-z, ()"
  // Every opened ( needs to have a corresponding )
  
  // TODO: Implement, until later we assume every input is correct
//}


// Prepare the string before we split it in parts
int Expression::preProcess()
{
  
  // Prepare the string for the algorithm:
  // 1. Add * between character and paranthesis. 
  // 2. Change c^3 to c*c*c
  // TODO: Add paranthesis around multi character numbers (for example 42) to treat them as one entity. But can have problems with for example 4c. Needs to add a * inbetween them. 
  // TODO 2: Remove all spaces in the expression, for example (a + b) => (a+b)
  // TODO 3: Fix negative exponents, or more complex exponents i.e. (5/4) or (a+b)
  
  
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
    /*if(_str[i] == '^'){
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
      }*/
    
    // Update last character
    lastCharacter = _str[i];
  }
  return 0;
}

                          
int Expression::getHighestPrecedence(std::string str)
{
  	// Search for the operator with highest precedence. 
	int numberOfParentheses = 0;
	int groundest = 255;
	int groundestPosition = -1;
	
	for(int i = str.length(); i >= 0; --i) {
		switch(str[i])
		{
			case '(':
			        std::cout << i << ":" << 2*numberOfParentheses << " ( detected" << std::endl;
				if(numberOfParentheses==0)
				  return -1;
				numberOfParentheses--; // OBS, the loop is in reverse order to make 5/3*4 appear (5/3)*4
				break;
			case ')':
 			        std::cout << i << ":"<< 2*numberOfParentheses <<" ) detected" << std::endl;
				numberOfParentheses++;
				break;
			case '*':
			case '/':
			        std::cout << i << ":" << 2*numberOfParentheses+1  << " */ detected" << std::endl;
				
				// If this is the operator with the highest precedence up to now
				if(2*numberOfParentheses + 1 < groundest)
				{
  				        groundest = 2*numberOfParentheses + 1;
					groundestPosition = i;
					std::cout << i << ":" << 2*numberOfParentheses+1  << " groundestPosition = "<< i << std::endl;
				}
				break;
			case '+':
			case '-':
			        std::cout << i << ":" << 2*numberOfParentheses  << " +- detected" << std::endl;
				
				// If this is the operator with the highest precedence up to now
				if(2*numberOfParentheses < groundest)
				{
					groundest = 2*numberOfParentheses;
					groundestPosition = i;
					std::cout << i << ":" << 2*numberOfParentheses  << " groundestPosition = "<<i<<std::endl;
				}
			default: 
				// Number (or variable detected)
				break;
		}
	}
	return groundestPosition;
}

// Removes a possible spare outer parantesis (this is due to the way I split the expressions at the operator with the highest predence, for example (4+5*3) will split as "(4" and "5*3)" )
std::string Expression::fixParenthesis(std::string str)
{
  int parentheses = 0;
  for(unsigned int i = 0; i < str.length(); ++i) {
    if(str[i] == '(')
      parentheses++;
    else if(str[i] == ')')
      parentheses--;
  }
  // If we have an extra ) at the end we remove it 
  if(parentheses==1 && str[0]=='(')
    str.erase(0,1);
  // If we have an extra ( at the beginning we remove it 
  else if(parentheses==-1 && str[str.length()-1] == ')')
    str.erase(str.length()-1,1);
  
  return str; //annas kommentar
}
