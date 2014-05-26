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


void Node::setData(float data){
  std::ostringstream o;    
  o << data;
  _data = o.str();
}
void Node::setData(std::string data){
  _data = data;
}
std::string Node::getData(){
  return _data;
}


void Node::setRight(Node* node, Node* parent){
  _rightChildren = node;
  setParent(parent);
}
void Node::setLeft(Node* node, Node* parent){
  _leftChildren = node;
  setParent(parent);
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

Node* Node::derive(std::string var)
{
	Node* newNode=new Node();
	
	return derive(var,this,newNode);
}

  // Derive this tree, recursive
Node* Node::derive(std::string variable,Node* node, Node* newNode )
 {		 
		char dataNode=node->getOperator();
		   switch(dataNode)   //test every operator - if number, variable or mult, go to a method
		   {
			case '*' :
				if(node->getRight()->getData()== "0" || node->getRight()->getData()=="0")
					{
					newNode->setData("+");
					newNode->setRight((new Node()),newNode);
					newNode->getRight()->setData("0");
					newNode->setLeft((new Node()),newNode);
					newNode->getLeft()->setData("0");
					}	
					else
					{
					std::cout<<" * detected, going to multDer"<<std::endl;
					multDerive(variable,node,newNode);
					}	
				break;
			case '/':
				//divDerive(variable,node);
				break;	
			case  '+':	
				std::cout<<" + detected"<<std::endl;
				addDerive(variable,node, newNode);
				break;
			case '-':
				std::cout<<" - detected"<<std::endl;
				newNode->setData("-");
				subDerive(variable,node, newNode);
				break;
			case 'n':
				if(node->getData()==variable){
					std::cout<<" variable detected"<<std::endl;
					newNode->setData("1");
				}
				else
				{
					std::cout<<" number detected"<<std::endl;
					newNode->setData("0");
				}
				break;
			default:
				newNode->setData("0");
				break;
			}
	
return newNode;;
 }

Node* Node::copyNodeTree(Node* node)
{
	Node* newNode=new Node();
	newNode->setData(node->getData()); //copy first node
	if(node->getRight()!=NULL && node->getRight()!=NULL)
	{
		newNode->setRight(node->getRight(),newNode);
		newNode->setLeft(node->getLeft(),newNode);
		copyNodeTree(node->getLeft());
		copyNodeTree(node->getRight());
	}
return newNode;
}
void Node::addDerive(std::string var,Node* node, Node* newNode)
{
	
	//Copying the values of right and left child node to save for later use 
	 Node* rightTree = copyNodeTree(node->getRight()); 	 
	 Node* leftTree = copyNodeTree(node->getLeft());
	 
	//creating new tree
	newNode->setData("+");
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
 	newNode->setLeft(derive(var,leftTree,newNode->getLeft()),newNode); //putting the left child node to the derived value
	newNode->setRight(derive(var,rightTree,newNode->getRight()),newNode); //putting the right child node to the derived value

}
Expression Node::toExpression(){
  // TODO: Implement!
  return Expression("a+b*sin(d)");
}


/*
 void Node::divDerive(std::string var, Node* node); 
 {
 
 
 } */
  

void Node::subDerive(std::string var,Node* node, Node* newNode)
{

	//Copying the values of right and left child node to save for later use 
	 Node* rightTree = copyNodeTree(node->getRight()); 	 
	 Node* leftTree = copyNodeTree(node->getLeft());
	 
	//creating new tree
	newNode->setData("-");
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
 	newNode->setLeft(derive(var,leftTree,newNode->getLeft()),newNode); //putting the left-right child node to the derived value
	newNode->setRight(derive(var,rightTree,newNode->getRight()),newNode); //putting the left-right child node to the derived value

}
void Node::multDerive(std::string var,Node* node, Node* newNode) 	//rule: D(f*g)=f'*g +g'*f
{ 

	 Node* rightTree = copyNodeTree(node->getRight()); 	//Copying the values of right and left child node to save for later use  
	 Node* leftTree = copyNodeTree(node->getLeft());
	 
	//creating new tree
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
	newNode->getLeft()->setLeft(new Node(),newNode->getLeft());
	newNode->getLeft()->setRight(new Node(),newNode->getLeft());
	newNode->getRight()->setLeft(new Node(),newNode->getRight());
	newNode->getRight()->setRight(new Node(),newNode->getRight());
	
	
	newNode->getLeft()->setLeft(leftTree,newNode->getLeft());   //put left-left child tree to copied tree (the one not derivated)
 	newNode->getRight()->setLeft(derive(var,leftTree,newNode->getRight()->getLeft()),newNode->getRight()); //putting the left-right child node to the derived value
	newNode->getLeft()->setRight(derive(var,rightTree,newNode->getLeft()->getRight()),newNode->getLeft()); //putting the left-right child node to the derived value
	newNode->getRight()->setRight(rightTree,newNode->getRight()); //put right-right child tree to copied tree (the one not derivated)
	

	//putting the parent node to + and the right and left children to * 
	newNode->setData("+");
	newNode->getRight()->setData("*");
	newNode->getLeft()->setData("*");
	

	
}
