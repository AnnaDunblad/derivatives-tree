// Author: Fredrik Lofgren
// Date: 30 May
// Open Source for any use by anyone

#include "Node.h"

// -----------------------------------------------------------
//                          PUBLIC
// -----------------------------------------------------------

// Shorten the tree below this Node
void Node::shorten(){
  
  // Shorten the children first (Note: Since it's a binary tree each node either have both or none children, so just check for one)
  if(this->getRight()){ 
    this->getRight()->shorten();
    this->getLeft()->shorten();
  }else{
    // If this node doesn't have any children we can't shorten it
    return;
  }
  
  // Child node containing data=0
  Node* zeroChild = NULL;
  // The other child node (with unknown data)
  Node* otherChild = NULL;
  
  if(this->getRight()->getData()=="0"){
    zeroChild = this->getRight();
    otherChild = this->getLeft();
  }else if(this->getLeft()->getData() == "0"){
    zeroChild = this->getLeft();
    otherChild = this->getRight();
  }
  
  // --------------------------------------------------------------
  // If both children are pure numeric we can perform the operation, 
  // for example 1+2 = 2 and remove the operator 
  // --------------------------------------------------------------
  // If both are pure numeric OR right numeric while left empty (then we have a function, like sine or log). 
  if((this->getRight()->isNumeric() && this->getLeft()->isNumeric()) || (this->getRight()->isNumeric() && this->getLeft()->getData().empty())){
    this->setData(Node::doOperation(this->getLeft()->getNumber(), this->getOperator(), this->getRight()->getNumber()));
    
    delete this->getRight();
    delete this->getLeft();
    this->setRight(NULL);
    this->setLeft(NULL);
    
    return;
  }
  
  
  // --------------------------------------------------------------
  // Short a/1 and a*1 to a
  // --------------------------------------------------------------
  else if(this->getRight()->getData()=="1" && (this->getOperator()=='*' || this->getOperator()=='/')){
    
    delete this->getRight();
    Node* tmp = this->getLeft(); // Need to access getLeft() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    this->setData(tmp->getData());
    this->setRight(tmp->getRight());
    this->setLeft(tmp->getLeft());
    
    delete tmp;
    return;
  }
  
  
  // --------------------------------------------------------------
  // Short 1*a to a
  // --------------------------------------------------------------
  else if(this->getLeft()->getData()=="1" && this->getOperator()=='*'){
    
    delete this->getLeft();
    Node* tmp = this->getRight(); // Need to access getRight() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    this->setData(tmp->getData());
    this->setLeft(tmp->getLeft()); 
    this->setRight(tmp->getRight()); 
    
    delete tmp;
    return; 
  }
  
  
  // --------------------------------------------------------------
  // If a child is 0
  // --------------------------------------------------------------
  else if(zeroChild){
    
    switch(this->getOperator()){
      
    case '-':
      // Special case with zero to the left, because we shoudn't short it (0-b != b)
      if(zeroChild==this->getLeft()){
	return; // Use return instead of break because we don't want to remove the zeroChild after the switch 	
      }
      // NOTE: Fall through! b-0 is the same as b+0 or 0+b
      
    case '+':
      // Move the other child to this position (without changing the address of this node, i.e. node = otherNode wont work)
      this->setData(otherChild->getData());
      this->setRight(otherChild->getRight());
      this->setLeft(otherChild->getLeft());
      // Delete the other node
      delete otherChild;
      break;
      
    case '/':
      // Check for illegal operation
      if(zeroChild==this->getRight())
	throw std::overflow_error("Divide by zero exception");
      // NOTE: Fall through! The same as multiplication. 0/b is the same as 0*b or b*0
      
    case '^':// If the operator is ^
      // Check if the exponent was 0
      if(zeroChild==this->getRight()){
	this->setData("1");
	delete otherChild;
	break;
      }
      // NOTE: Fall through! If the base was 0, same behaviour as *
      
    case '*': // If the operator is * ...
      // ... remove the other child as well
      delete otherChild;
      this->setData("0");
      this->setRight(NULL);
      this->setLeft(NULL);
      break;
    }
    
    // Delete the zero node (must be last because we don't want to remove the zero in for example 0-b).
    delete zeroChild; 
    return;
  } 
  
  return;
}

// Returns the variables used in this String (use set because it contains unique elements)
void Node::getVariables(std::map<std::string,float>& variables){

  // If this is a variable (i.e. has no childs and is not numeric)
  if(this->getRight() == NULL && !this->isNumeric() && !this->getData().empty()){
    variables.insert(std::pair<std::string,float>(this->getData(),0));
  }else if(this->getRight() != NULL){
    this->getRight()->getVariables(variables);
    this->getLeft()->getVariables(variables);
  }
}

// Calculate the value of this tree if all symbolic variables is set to value,with the variables set to the values in the map
float Node::calculate(std::map<std::string,float>& variables){
  // If this is a number literal
  if(this->isNumeric())
    return this->getNumber();
  // If this is a variable
  else if(this->getRight() == NULL)
    return variables[this->getData()];
  // If this is an operation, execute it!
  else
    return doOperation(this->getLeft()->calculate(variables),this->getOperator(),this->getRight()->calculate(variables));
  
  return 0;
}


// Print this tree
void Node::printTree() {
  int maxLevel = this->maxLevel(this);
  printTree(std::vector<Node*>(1,this), 1, maxLevel);
}



// -----------------------------------------------------------
//                          PRIVATE
// -----------------------------------------------------------

// Do the operation op with left operand and right operand
float Node::doOperation(float left, char op, float right)
{
  switch(op){
  case '+':
    return left + right;
    break;
  case '-':
    return left - right;
    break;
  case '*':
    return left * right;
    break;
  case '/':
    return left / right;
    break;
  case '^':
    return pow(left, right);
    break;
  case '#':
    return sin(right);
    break;
  case '%':
    return cos(right);
  case '&':
    return log(right); // Note this is the natural logarithm
  }
  return 0;
}


// Return the operator, and n if the node contains data
char Node::getOperator()
{
  if(getData()=="+")
    return '+';
  else if(getData()=="-")
    return '-';
  else if(getData()=="*")
    return '*';
  else if(getData()=="/")
    return '/';
  else if(getData()=="^")
    return '^';
  else if(getData()=="#")
    return '#';
  else if(getData()=="%")
    return '%';
  else if(getData()=="&")
    return '&';
  return 'n';
}

// returns true if this node contains a number-value
bool Node::isNumeric()
{
  float number;
  return (std::istringstream(_data) >> number);
}

// Returns the mathematical value of this node (0 is its an operator, funcion of parenthesis)
float Node::getNumber(){
  float number;
  std::istringstream(_data) >> number;
  return number;
}

// Used internally to print a tree (recursively)
void Node::printTree(std::vector<Node*> nodes, int level, int maxLevel) {
  if (nodes.empty() || isAllElementsNull(nodes))
    return;
  
  int floor = maxLevel - level;
  int endgeLines = 1<<std::max(floor - 1, 0);
  int firstSpaces = (1<<floor) - 1;
  int betweenSpaces = (1<<(floor + 1)) - 1;
  
  std::cout << std::string(firstSpaces,' ');
  std::vector<Node*> newNodes;
  
  for(std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
    if((*it) != NULL){
      std::cout << (*it)->getData();
      newNodes.push_back((*it)->getLeft());
      newNodes.push_back((*it)->getRight());
    }else{
      newNodes.push_back(NULL);
      newNodes.push_back(NULL);
      std::cout << " ";
    }
    std::cout << std::string(betweenSpaces,' ');
  }
  std::cout << std::endl;
  
  for (int i = 1; i <= endgeLines; i++) {
    for (unsigned int j = 0; j < nodes.size(); j++) {
      if(firstSpaces - i > 0)
	std::cout << std::string(firstSpaces - i,' ');
      
      if (nodes[j] == NULL) {
	std::cout << std::string(endgeLines + endgeLines + i + 1,' ');
	continue;
      }
      if (nodes[j]->getLeft() != NULL)
	std::cout << "/";
      else	
	std::cout << std::string(1,' ');
      std::cout << std::string(i + i - 1,' ');
      
      if (nodes[j]->getRight() != NULL)
	std::cout << "\\";
      else
	std::cout << std::string(1,' ');
      std::cout << std::string(endgeLines + endgeLines - i,' ');
    }
    std::cout << std::endl;
  }
  
  printTree(newNodes, level + 1, maxLevel);
}
int Node::maxLevel(Node* node){
  if (node == NULL)
    return 0;
  return std::max(maxLevel(node->getLeft()), maxLevel(node->getRight())) + 1;
}

bool Node::isAllElementsNull(std::vector<Node*> list) {
  for(std::vector<Node*>::iterator it = list.begin(); it != list.end(); ++it) {
    if((*it) != NULL)
      return false;
  }
  return true;
}

