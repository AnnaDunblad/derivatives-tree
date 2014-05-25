#include "Node.h"


// Shorten the expression tree from this node (remove zeroes, remove outermost parenthesis,
void Node::shorten(){
  shorten(this);
}

// Returns number of shortened steps
int Node::shorten(Node* node){
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
    zeroChild = node->getRight();
    otherChild = node->getLeft();
  }
  else if(node->getLeft()->getData() == "0"){
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
      if(node->getParent()){
	// ... move the other child to this node. 
	node->getParent()->changeChild(node,otherChild); // Changed 19:14 25 May, from (node,zeroChild) to (node,otherChild) Seems to work!
      }else{
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
  }
  return 0;
}

// Change the child that pointed to /from/ to /to/
void Node::changeChild(Node* from, Node* to)
{
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
