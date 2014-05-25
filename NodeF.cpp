#include "Node.h"


// Shorten the expression tree from this node (remove zeroes, remove outermost parenthesis,
void Node::shorten(){
  shorten(this);
}

// Returns number of shortened steps
int Node::shorten(Node* node){
  
  std::cout << "Is: "<<node <<"Has right: " << node->getRight() << ", left: " << node->getLeft() << std::endl;
  //std::cout << node->getData() << " is " << node->isNumeric()<<" "<<node->getNumber() << std::endl;
  // Shorten the children first (Note: Since it's a binary tree each node either have both or none children, so just check for one)
  if(node->getRight()){
    
    node->getRight()->shorten();
    node->getLeft()->shorten();
  }
  else{
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
    std::cout<< "Hoger==0:"<<node->getRight()<<std::endl;
    zeroChild = node->getRight();
    otherChild = node->getLeft();
  }
  else if(node->getLeft()->getData() == "0"){
    std::cout <<"Vanster==0: "<<node->getLeft() << std::endl;
    zeroChild = node->getLeft();
    otherChild = node->getRight();
  }
  // If a child is 0
  if(zeroChild){
    switch(node->getOperator()){
    // If this operator is + or - ...
    case '+':
    case '-':
      // If this node has an parent (isn't the ancestor too everyone)
      /*if(node->getParent()){
	std::cout << "Byter bort " << node->getData() << " till " << otherChild->getData() << " i " << node->getParent()->getData() << std::endl;
	// ... move the other child to this node. 
	node->getParent()->changeChild(node,otherChild); // Changed 19:14 25 May, from (node,zeroChild) to (node,otherChild) Seems to work!
	}else*/{ // Commented out 21:45 25 May, because we will (I think) never interchange the children... Just move one of the children up a step to its parent
	std::cout << "Buh!" <<std::endl;
	// Move the other child to this position (without changing the address of this node, i.e. node = otherNode wont work)
	node->setData(otherChild->getData());
	node->setRight(otherChild->getRight(),node);
	node->setLeft(otherChild->getLeft(),node);
	// Delete the other node
	delete otherChild;
      }
      break;
    case '/':
      if(zeroChild==node->getRight()){
	std::cout << "ERROR Division by zero" << std::endl;
	return -1;
      }
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




    /*

  // If both children are pure numeric we can do the operation, for example 1+2 = 2 and remove the operator 
  }else if(node->getRight()->isNumeric() && node->getLeft()->isNumeric()){
    std::cout << "Nummber??" << std::endl;
    node->setData(Node::doOperation(node->getLeft()->getNumber(),node->getOperator(),node->getRight()->getNumber()));
    
    delete node->getRight();
    delete node->getLeft();
    node->setRight(NULL, node);
    node->setLeft(NULL, node);
    */
  }
    
  return 0;
}
/*
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
    */

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


