#include "Node.h"

void Node::printTree() {
  int maxLevel = this->maxLevel(this);
  printTreeInternal(std::vector<Node*>(1,this), 1, maxLevel);
}

void Node::printTreeInternal(std::vector<Node*> nodes, int level, int maxLevel) {
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
	
	printTreeInternal(newNodes, level + 1, maxLevel);
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

// Shorten the Expression tree from this node (remove zeroes etc.)
void Node::shorten(){
  // TODO:  Sometimes we may need to shorten the tree several times 
  shorten(this);
}

// Shorten the tree below /node/
int Node::shorten(Node* node){

  // Shorten the children first (Note: Since it's a binary tree each node either have both or none children, so just check for one)
  if(node->getRight()){ 
    node->getRight()->shorten();
    node->getLeft()->shorten();
  }else{
    // If this node doesn't have any children we can't shorten it
    return 0;
  }
  
  // Child node containing data=0
  Node* zeroChild = NULL;
  // The other child node (with unknown data)
  Node* otherChild = NULL;
  
  if(node->getRight()->getData()=="0"){
    zeroChild = node->getRight();
    otherChild = node->getLeft();
  }else if(node->getLeft()->getData() == "0"){
    zeroChild = node->getLeft();
    otherChild = node->getRight();
  }
  
  // --------------------------------------------------------------
  // If both children are pure numeric we can perform the operation, 
  // for example 1+2 = 2 and remove the operator 
  // --------------------------------------------------------------
  // If both are pure numeric OR right numeric while left empty (then we have a function, like sine or log). 
  if((node->getRight()->isNumeric() && node->getLeft()->isNumeric()) || (node->getRight()->isNumeric() && node->getLeft()->getData().empty())){
    node->setData(Node::doOperation(node->getLeft()->getNumber(),node->getOperator(),node->getRight()->getNumber()));
    
    delete node->getRight();
    delete node->getLeft();
    node->setRight(NULL);
    node->setLeft(NULL);
    
    return 1;
  }
  
  
  // --------------------------------------------------------------
  // Short a/1 and a*1 to a
  // --------------------------------------------------------------
  else if(node->getRight()->getData()=="1" && (node->getOperator()=='*' || node->getOperator()=='/')){
    
    delete node->getRight();
    Node* tmp = node->getLeft(); // Need to access getLeft() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(tmp->getData());
    node->setRight(tmp->getRight());
    node->setLeft(tmp->getLeft());
    
    delete tmp;
    return 1;
  }
  
  
  // --------------------------------------------------------------
  // Short 1*a to a
  // --------------------------------------------------------------
  else if(node->getLeft()->getData()=="1" && node->getOperator()=='*'){
    
    delete node->getLeft();
    Node* tmp = node->getRight(); // Need to access getRight() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(tmp->getData());
    node->setLeft(tmp->getLeft()); 
    node->setRight(tmp->getRight()); 
    
    delete tmp;
    return 1; 
  }
  
  
  // --------------------------------------------------------------
  // If a child is 0
  // --------------------------------------------------------------
  else if(zeroChild){
    
    switch(node->getOperator()){
    
    case '-':
      // Special case with zero to the left, because we shoudn't short it (0-b != b)
      if(zeroChild==node->getLeft()){
	return 0; // Use return instead of break because we don't want to remove the zeroChild after the switch 	
      }
      // NOTE: Fall through! b-0 is the same as b+0 or 0+b
      
    case '+':
      // Move the other child to this position (without changing the address of this node, i.e. node = otherNode wont work)
      node->setData(otherChild->getData());
      node->setRight(otherChild->getRight());
      node->setLeft(otherChild->getLeft());
      // Delete the other node
      delete otherChild;
      break;
      
    case '/':
      // Check for illegal operation
      if(zeroChild==node->getRight())
	throw std::overflow_error("Divide by zero exception");
      // NOTE: Fall through! The same as multiplication. 0/b is the same as 0*b or b*0
      
    case '^':// If the operator is ^
      // Check if the exponent was 0
      if(zeroChild==node->getRight()){
	node->setData("1");
	delete otherChild;
	break;
      }
      // NOTE: Fall through! If the base was 0, same behaviour as *
      
    case '*': // If the operator is * ...
      // ... remove the other child as well
      delete otherChild;
      node->setData("0");
      node->setRight(NULL);
      node->setLeft(NULL);
      break;
    }
    
    // Delete the zero node (must be last because we don't want to remove the zero in for example 0-b).
    delete zeroChild; 
    return 1;
  } 
  
  return 0;
}

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

// Calculate the value of this tree with the variables set to the values in the map
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
