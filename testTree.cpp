#include <iostream>
#include <string>



bool isOperator(char ch)
{
  return ch=='*' || ch=='+' || ch=='/' || ch=='-';
}

bool isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}

int main()
{
  std::string str;// = "(b+c+b)+1";
  std::cin >> str;
  
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
  
  
}


