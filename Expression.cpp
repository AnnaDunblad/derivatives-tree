#include<Expression.h>

bool Expression::isOperator(char ch)
{
  return ch=='*' || ch=='+' || ch=='/' || ch=='-';
}

bool Expression::isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}

int Expression::preProcess(std::string str)

  // Prepare the string for the algorithm:
  // 1. Add * between character and paranthesis. 
  // 2. Change c^3 to c*c*c
  // TODO: Add paranthesis around multi character numbers (for example 42) to treat them as one entity. But can have problems with for example 4c. Needs to add a * inbetween them. 
  
  char lastCharacter = 0;
  for(int i = 0; i < str.length(); ++i)
    {
      // 1. * Before parantesis
      if(str[i] == '(' && i>0 && !isOperator(lastCharacter)){
	str.insert(i,1,'*');
	i++;
      }

      // Note: This makes (a+b)(c+d) fails, and only adds support for rare expressions on the form (a+b)K
      //if(lastCharacter == ')' && !isOperator(str[i])){
      //str.insert(i,"*");
      //i++;
      //}
      
      // 2. Fix powers
      if(str[i] == '^')
        {
	  // Does next character exists? It has to do for it to be an correct expression, in addition the exponent has to be a number (limited to 0-9)
	  if(i+1 == str.length() || i==0 || !isNumber(str[i+1]))
	    return -1;
	  
	  // Make the part that we will add repeatly
	  std::string expression = "";// + str[i-1];
	  int parantesis = 0;
	  for(int j = i-1; j>=0; --j)
	    {
	      expression.insert(0,1,str[j]);
	      if(str[j] == ')')
		parantesis++;
	      else if(str[j] == '(')
		parantesis--;
	      if(parantesis==0)
		break;
	    }
	  expression.insert(0,1,'*');
	  std::cout << expression << std::endl;
	  for(int j = '0'; j < str[i+1] - 1; j++){
	    str.insert(i,expression);
	    i+=expression.length();
	  }
	  // Remove the ^5 part of the string
	  str.erase(i,2);
	}
      
      // Update last character
      lastCharacter = str[i];
    }
  std::cout << "done" << std::endl;
  std::cout << str << std::endl;
  return str;
}





int Expression::getHighestPrecendence()
{
  	// Search for the operator with highest precedence. 
	int numberOfParantesis = 0;
	int deepness = 0;
	int groundest = 255;
	int groundestPosition = -1;
	char lastOperador = 0;
	
	for(int i = 0; str.length(); ++i) {
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
				printf("%d: ( detected\n", i);
				numberOfParantesis++;
				deepness++;
				break;
			case ')':
				printf("%d: ) detected\n", i);
				if(numberOfParantesis==0)
					return -1;
				numberOfParantesis--;
				deepness--;
				lastOperador='n';
				break;
			case '*':
			case '/':
				printf("%d: */ detected\n", i);
				if(lastOperador=='+')
				{
					deepness++;
					printf("%d: deepness increased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}
				lastOperador = '*';
				break;
			case '+':
			case '-':
				printf("%d: +- detected\n", i);
				// If this is a new operator
				if(lastOperador=='*')
				{
					deepness--;
					printf("%d: deepness decreased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}
				lastOperador = '+';
			default: 
				// Number (or variable detected)
				// TODO: Concat if several caracters, i.e.  8132
				//lastOperador='n';
				break;
		}
	}
}

// Removes a possible spare outer parantesis (this is due to the way I split the expressions at the operator with the highest predence, for example (4+5*3) will split as "(4" and "5*3)" )
void fixParantesis()
{
  int parantesis = 0;
  for(int i = 0; i < str.length(); ++i)
    {
      if(str[i] == '(')
	parantesis++;
      else if(str[i] == ')')
	parantesis--;
    }
  // If we have an extra ) at the end we remove it 
  if(parantesis==1 && str[0]=='(')
    str.erase(0,1);
  // If we have an extra ( at the beginning we remove it 
  else if(parantesis==-1 && str[str.length()-1] == ')')
    str.erase(str.length()-1,1);
   
}
