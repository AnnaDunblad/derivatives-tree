#include "Node.h"

// Create a new floating node
Node::Node()
{
  _leftChildren = NULL;
  _rightChildren = NULL;
  _parent = NULL;
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


void Node::setRight(Node* node, Node* parent){
  _rightChildren = node;
  if(node) // node will be NULL when we are copying a node without children to a new node, i.e. this->setRight(other->getRight(),this);
    _rightChildren->setParent(parent);
}
void Node::setLeft(Node* node, Node* parent){
  _leftChildren = node;
  if(node) // node will be NULL when we are copying a node without children to a new node, i.e. this->setLeft(other->getLeft(),this);
    _leftChildren->setParent(parent);
}

Node* Node::getRight(){
  return _rightChildren;
}
Node* Node::getLeft(){
  return _leftChildren;
}



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
      //std::cout << firstSpaces - i << std::endl;
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


  // Derive this tree, recursive
void derive()
{


}




