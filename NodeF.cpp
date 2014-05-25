#include "Node.h"


// Shorten the expression tree from this node (remove zeroes, remove outermost parenthesis,
void Node::shorten(){
  shorten(this);
}

void Node::shorten(Node* node){
  // If a child is 0, delete it
  // If this operator is + or -, move the other child to this node. 
  // If this operator is *, remove the other child as well
  // If this operator is /, if the 0 was to the right, remove node, else error
  
  if(node->getRight()->getData()=='0'){
    if(node->getData() == '+' || node->getData() == '-'){
      delete node->getRight();
      node->getParent()->changeChild(this,node->getLeft());
    }
  }
}

// Change the child that pointed to /from/ to /to/
void Node::changeChild(Node* from, Node* to)
{
  if(getLeft()==from)
    setLeft(to,this);
  else if(getRight()==from)
    setRight(to,this);
  else
    std::cout << "ERROR in changeChild, from="<<from->getData()<<", to="<<to->getData()<<", this="<<getData()<<std::endl;
}

Node* Node::getParent()
{
  return _parent;
}
void Node::setParent(Node* node)
{
  _parent node;
}
