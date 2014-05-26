#include "Node.h"


// Shorten the expression tree from this node (remove zeroes, remove outermost parenthesis,
void Node::shorten(){
  // TODO:  Sometimes we need to shorten the tree several times 
  shorten(this);
}

// Returns number of shortened steps
int Node::shorten(Node* node){
  
  // Shorten the children first (Note: Since it's a binary tree each node either have both or none children, so just check for one)
  if(node->getRight()){ 
    node->getRight()->shorten();
    node->getLeft()->shorten();
  }else{
    // If this node doesn't have any children we can't shorten it
    return 0;
  }
  
  
  // If a child is 0, delete it
  // If this operator is + or -, move the other child to this node. 
  // If this operator is *, remove the other child as well
  // If this operator is /, if the 0 was to the right, remove node, else error

  // Node containing a child that has data=0
  Node* zeroChild = NULL;
  // The other child (with unknown value)
  Node* otherChild = NULL;
  
  if(node->getRight()->getData()=="0"){
    zeroChild = node->getRight();
    otherChild = node->getLeft();
  }
  else if(node->getLeft()->getData() == "0"){
    zeroChild = node->getLeft();
    otherChild = node->getRight();
  }
  
  
  // If both children are pure numeric we can do the operation, for example 1+2 = 2 and remove the operator 
  if(node->getRight()->isNumeric() && node->getLeft()->isNumeric()){
    node->setData(Node::doOperation(node->getLeft()->getNumber(),node->getOperator(),node->getRight()->getNumber()));
    
    delete node->getRight();
    delete node->getLeft();
    node->setRight(NULL, node);
    node->setLeft(NULL, node);
    
    return 1;
    
    
  // Short a/1 and a*1 to a     
  }else if(node->getRight()->getData()=="1" && (node->getOperator()=='*' || node->getOperator()=='/')){
    
    delete node->getRight();
    Node* tmp = node->getLeft(); // Need to access getLeft() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(node->getLeft()->getData());
    node->setRight(node->getLeft()->getRight(), node);
    node->setLeft(node->getLeft()->getLeft(), node);
    
    delete tmp;
    
    return 1; 
    
  // Short 1*a to a
  }else if(node->getLeft()->getData()=="1" && node->getOperator()=='*'){
    
    delete node->getLeft();    
    Node* tmp = node->getRight(); // Need to access getRight() to fetch its children, but then I have already updated the variable and lost track of this object, so need to save it before I update anything.
    
    node->setData(node->getRight()->getData());
    node->setRight(node->getRight()->getRight(), node);
    node->setLeft(node->getRight()->getLeft(), node);
    
    delete tmp;
    
    return 1; 





  // If a child is 0
 }else if(zeroChild){
    switch(node->getOperator()){
    // If this operator is + or - ...
    case '-'://  Special case with (0-b), because we shoudn't short it (0-b != b)
      if(zeroChild==node->getLeft()){
	return 0; // Use return instead of break because we don't want to remove the zeroChild after the switch 	
      }
    case '+':
      // If this node has an parent (isn't the ancestor too everyone)
      /*if(node->getParent()){
	std::cout << "Byter bort " << node->getData() << " till " << otherChild->getData() << " i " << node->getParent()->getData() << std::endl;
	// ... move the other child to this node. 
	node->getParent()->changeChild(node,otherChild); // Changed 19:14 25 May, from (node,zeroChild) to (node,otherChild) Seems to work!
	}else*/{ // Commented out 21:45 25 May, because we will (I think) never interchange the children... Just move one of the children up a step to its parent
	// Move the other child to this position (without changing the address of this node, i.e. node = otherNode wont work)
	node->setData(otherChild->getData());
	node->setRight(otherChild->getRight(),node);
	node->setLeft(otherChild->getLeft(),node);
	// Delete the other node
	delete otherChild;
      }
      break;
    case '/':
      if(zeroChild==node->getRight())
	throw std::overflow_error("Divide by zero exception");
      // NOTE: Fall through! (the same behaviour as *)
    case '^':
      if(zeroChild==node->getRight()){
	node->setData("1");
	delete otherChild;
	break;
      }
      // NOTE: Fall through! If the base was 0, same behaviour as *
    // If the operator is * ...
    case '*':
      // ... remove the other child as well
      delete otherChild;
      node->setData("0");
      node->setRight(NULL, node);
      node->setLeft(NULL, node);
      break;
    }
    
    // Delete the 0 (must be last because we are using this pointer to get right children in changeChild).
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


// Change the child that pointed to /from/ to /to/
void Node::changeChild(Node* from, Node* to)
{
  std::cout << "from: "<<from<<", to: "<<to<<std::endl;
  if(getLeft()==from)
    setLeft(to,this);
  else if(getRight()==from)
    setRight(to,this);
  else
    std::cout << "ERROR in changeChild, from=" << from->getData() << ", to=" << to->getData() << ", this=" << getData() << std::endl;
}

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

float Node::getNumber(){
  float number;
  std::istringstream(_data) >> number;
  return number;
}


