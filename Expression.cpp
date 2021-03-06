// Author: Fredrik Lofgren
// Date: 30 May
// Open Source for any use by anyone

#include "Expression.h"

const char Expression::_allowedCharacters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/^()."; 

// -----------------------------------------------------------
//                          PRIVATE
// -----------------------------------------------------------

// Return true if ch is operator
bool Expression::isOperator(char ch)
{
  return ch=='*' || ch=='+' || ch=='/' || ch=='-' || ch=='^';
}
// Returns true if ch is a number
bool Expression::isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}
// Returns true if ch is a letter
bool Expression::isVariable(char ch)
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
// Returns true if ch is a function (sin, cos, ln)
bool Expression::isFunction(char ch)
{
  // In the preprocessing we have changed the functions to a single character symbol
  return ch=='&' || ch=='%' || ch=='#';
}

// Replace search with replace in subject
void Expression::replaceString(std::string& subject, const std::string& search, const std::string& replace) {
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
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
  
  return str;
}


// Returns the position of the operator where we should split the expression
int Expression::getHighestPrecedence(std::string str)
{
  // Search for the operator with highest precedence. 
  int numberOfParentheses = 0;
  int groundest = 255;
  int groundestPosition = -1;

  // Loop through the string in reverse order!
  for(int i = str.length()-1; i >= 0; --i) {
    
    switch(str[i]) {
    case '(':
      if(numberOfParentheses==0)
	throw std::invalid_argument("Mismatch of parentheses exception");
      numberOfParentheses--; // OBS, the loop is in reverse order to make 5/3*4 appear (5/3)*4
      break;
      
    case ')':
      numberOfParentheses++;
      break;
      
    case '&':
    case '%':
    case '#':
      // If this is the operator with the highest precedence up to now
      if(4*numberOfParentheses + 3 < groundest) {
	groundest = 4*numberOfParentheses + 3;
	groundestPosition = i;
      }
      break;
      
    case '^':
      // If this is the operator with the highest precedence up to now
      if(4*numberOfParentheses + 2 < groundest) {
	groundest = 4*numberOfParentheses + 2;
	groundestPosition = i;
      }
      break;
      
    case '*':
    case '/':
      // If this is the operator with the highest precedence up to now
      if(4*numberOfParentheses + 1 < groundest){
	groundest = 4*numberOfParentheses + 1;
	groundestPosition = i;
      }
      break;
      
    case '+':
    case '-': //TODO: Don't return unary - signs as precedece value
      // If this is the operator with the highest precedence up to now
      if(4*numberOfParentheses < groundest){
	groundest = 4*numberOfParentheses;
	groundestPosition = i;
      }
    default: 
      // Number (or variable detected)
      break;
    }
  }
  return groundestPosition;
}

// Prepare the string before we split it in parts
int Expression::preProcess()
{
  
  // Prepare the string for the algorithm:
  // 1. Add * between character and paranthesis. 
  // 2. Remove all spaces in the expression, for example (a + b) => (a+b)
  // 3. Add * between letter and number, i.e. 3b=>3*b
  // TODO 4: Unary -
  
  // 5. Replace sin etc. with single charcter because toTree() will have hard to split the string otherwise
  replaceString(_str,"sin","#");
  replaceString(_str,"cos","%");
  replaceString(_str,"ln","&");
  
  
  char lastCharacter = 0;
  for(unsigned int i = 0; i < _str.length(); ++i) {
    // 1. * Before parantesis: a(b+c), 5(b+c) or (a+b)(c+d)
    if(_str[i] == '(' && (isNumber(lastCharacter) || isVariable(lastCharacter) || lastCharacter==')')){
      _str.insert(i,1,'*');
      i++;
    }
    // 2. Remove all spaces in the expression, for example (a + b) => (a+b)
    if(_str[i]==' '){
      _str.erase(i,1);
      i--;
    }
    
    // 3. Add * between letter and number, i.e. 3b=>3*b
    if(isNumber(lastCharacter) && isVariable(_str[i])){
      _str.insert(i,1,'*');
      i++;
    }
    
    // 4. Add * between constant and sin/ln/cos: Asin(b) => A*sin(b)
    if((isNumber(lastCharacter) || isVariable(lastCharacter)) && isFunction(_str[i])){
      _str.insert(i,1,'*');
      i++;
    }
    
    
    // Update last character
    lastCharacter = _str[i];
  }
  
  return 0;
}


// Recursive method to create an tree from a string
void Expression::toTree(Node* currNode, std::string currStr)
{ 
  int pos = getHighestPrecedence(currStr);
  
  // the currStr doesn't contain any operator
  if(pos==-1){
    currNode->setData(trim(currStr));
  }
  else{
    currNode->setData(std::string(1,currStr[pos]));
    currNode->setRight(new Node());
    currNode->setLeft(new Node());
    
    toTree(currNode->getLeft(), fixParenthesis(currStr.substr(0,pos)));
    toTree(currNode->getRight(), fixParenthesis(currStr.substr(pos+1)));
  }
}


// -----------------------------------------------------------
//                            PUBLIC
// -----------------------------------------------------------

// Creates an tree from the string
Node* Expression::toTree(){
  preProcess();
  Node* central = new Node();
  toTree(central, _str);
  
  return central;
}


// Check string for errors
bool Expression::checkError()
{
  // If the string contains non valid characters
  if(_str.find_first_not_of(_allowedCharacters)!=std::string::npos)
    throw std::invalid_argument("Exp contains illegal character");
  
  return true;
}


// Create a new object
Expression::Expression(std::string str)
{
  _str = str;
  checkError();
}


// Overload << so we can print this class easy!
std::ostream& operator<<(std::ostream& out, const Expression& exp)
{
  // Since operator << is a friend of the Expression class, we can access Expression's members directly.
  std::string str(exp._str); // Make a new copy of string because we don't want to "undo" the preProcessing work of this expression
  
  Expression::replaceString(str,"#","sin");
  Expression::replaceString(str,"%","cos");
  Expression::replaceString(str,"&","ln");
  
  out << str;
  return out;
}


