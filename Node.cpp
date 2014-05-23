#include "Node.h"


Node::Node()
{
  _leftChildren = NULL;
  _rightChildren = NULL;
}

/*bool Node::setData(std::string data)
{
  if (!(std::istringstream(data) >> _data)){
    _data = 0;
    return false;
  }
  return true;
}*/

void Node::setData(std::string data){
  _data = data;
}
std::string Node::getData(){
  return _data;
}


bool Node::setOperator(char op){
  switch(op)
    {
    case '+':
      return true;
      // TODO: Implement!
    }
  return false;
}


void Node::setRight(Node* node){
  _rightChildren = node;
}
void Node::setLeft(Node* node){
  _leftChildren = node;
}

Node* Node::getRight(){
  return _rightChildren;
}
Node* Node::getLeft(){
  return _leftChildren;
}

