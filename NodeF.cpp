#include "Node.h"


// Shorten the expression tree from this node (remove zeroes etc.)
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
  if(node->getRight()->isNumeric() && node->getLeft()->isNumeric()){
    node->setData(Node::doOperation(node->getLeft()->getNumber(),node->getOperator(),node->getRight()->getNumber()));
    
    delete node->getRight();
    delete node->getLeft();
    node->setRight(NULL, node);
    node->setLeft(NULL, node);
    
    return 1;
  }
  
  
  // --------------------------------------------------------------
  // Short a/1 and a*1 to a
  // --------------------------------------------------------------
  else if(node->getRight()->getData()=="1" && (node->getOperator()=='*' || node->getOperator()=='/')){
    
    delete node->getRight();
    Node* tmp = node->getLeft(); // Need to access getLeft() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(node->getLeft()->getData());
    node->setRight(node->getLeft()->getRight(), node);
    node->setLeft(node->getLeft()->getLeft(), node);
    
    delete tmp;
    return 1;
  }
  
  
  // --------------------------------------------------------------
  // Short 1*a to a
  // --------------------------------------------------------------
  else if(node->getLeft()->getData()=="1" && node->getOperator()=='*'){
    
    delete node->getLeft();    
    Node* tmp = node->getRight(); // Need to access getRight() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(node->getRight()->getData());
    node->setRight(node->getRight()->getRight(), node);
    node->setLeft(node->getRight()->getLeft(), node);
    
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
      node->setRight(otherChild->getRight(),node);
      node->setLeft(otherChild->getLeft(),node);
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
      node->setRight(NULL, node);
      node->setLeft(NULL, node);
      break;
    }
    
    // Delete the zero node (must be last because we don't want to remove the zero in for example 0-b).
    delete zeroChild; 
    return 1;
  } 
  
  return 0;
}

std::string Node::doOperation(float left, char op, float right)
{
  float result;
  switch(op){
  case '+':
    result = left + right;
    break;
  case '-':
    result = left - right;
    break;
  case '*':
    result = left * right;
    break;
  case '/':
    result = left / right;
    break;
  case '^':
    result = pow(left, right);
    break;
  }

  std::ostringstream o;    
  o << result;
  return o.str();
}

// TODO: Remove this method, if still unused when everything works!
Node* Node::getParent()
{
  return _parent;
}
void Node::setParent(Node* parent)
{
  _parent = parent;
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
