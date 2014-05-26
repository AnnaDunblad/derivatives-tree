#include "Node.h"

// Create a new floating node
Node::Node()
{
  _leftChildren = NULL;
  _rightChildren = NULL;
  _parent = NULL;
  multDone=false;
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
	return derive(var,this);
}

  // Derive this tree, recursive
Node* Node::derive(std::string variable,Node* node)
 {		  
		 Node* topNode=node;
		char dataNode=node->getOperator();

		   switch(dataNode)   //test every operator - if number, variable or mult, go to a method
		   {
			case '*' :
					node=multDerive(variable,node);		
				break;
			case '/':
				//divDerive(variable,node);
				break;	
			case  '+':	
				break;
			case '-':
				break;
			case 'n':
				if(node->getData()==variable){
					node->setData("1");
				}
				else
				{
					node->setData("0");
				}
				break;
			default:
				node->setData("0");
				break;
			}

		if(node->getRight()!= NULL && node->getRight()->getData()!="0"){
			std::cout<<"Getting right child"<<std::endl;
			derive(variable,node->getRight());

		}
		if(node->getLeft()!= NULL && node->getLeft()->getData()!="0"){
			std::cout<<"Getting left child"<<std::endl;
			derive(variable, node->getLeft());

		}
		if(node->getLeft()== NULL && node->getRight()== NULL)
			std::cout<< "End of tree"<< std::endl;

return topNode;
 }
 
Node* Node::copyNode(Node* node)
{
	Node* newNode = new Node();
	newNode->setData(node->getData());
return newNode;
}

Node* Node::copyNodeTree(Node* node)
{
	Node* newNode=new Node();
	while(node->getRight()!=NULL && node->getRight()!=NULL)
	{
		newNode->setData(node->getData()); //copy first node
		newNode->setRight(node->getRight(),newNode);
		newNode->setLeft(node->getLeft(),newNode);
		copyNodeTree(node->getLeft());
		copyNodeTree(node->getRight());
	}

return newNode;
}

Node* Node::multDerive(std::string var,Node* node) 	//rule: D(f*g)=f'*g +g'*f
{ 
	std::cout<<"Entering multDerive function"<<std::endl;
	std::cout<<"node->getRight()->getData()="<<node->getRight()->getData() <<std::endl;
	std::cout<<"node->getLeft()->getData()="<<node->getLeft()->getData() <<std::endl;

	Node* RRnode;
	Node* LLnode;
	if(node->getRight()->getOperator() == 'n' && node->getLeft()->getOperator() ) {
		 RRnode = copyNode(node->getRight()); 	//Copying the values of right and left child node to save for later use  
		 LLnode = copyNode(node->getLeft());
	}
	else
	{
		if(node->getRight()->getData()!="0")
		{
			std::cout<<"Getting right child in derive from  mult"<<std::endl;
			RRnode=derive(var,node->getRight());
		}
		if(node->getLeft()->getData()!="0")
		{
			std::cout<<"Getting left child from mult"<<std::endl;
			derive(var, node->getLeft());
		}
	}
	
	
	
	node->getLeft()->setLeft(LLnode,node->getLeft()); //put left-left child node to copied value (the one not derivated)
	node->getRight()->setRight(RRnode,node->getRight()); //put right-right child node to copied value (the one not derivated)
	
		std::cout<<"RR="<<RRnode->getData() <<std::endl;
		std::cout<<"LL="<<LLnode->getData() <<std::endl;

	//putting the parent node to + and the right and left children to * 
	node->setData("+");
	node->getRight()->setData("*");
	node->getLeft()->setData("*");

	//starting deriving and asignation of Left-Right child node
	Node* LRnode= new Node(); LRnode->setData(RRnode->getData()); //create new node with the value to be derivated taken from the value saved in right-right node
	node->getLeft()->setRight(LRnode,node->getLeft()); 
    std::string resultLR= derive(var,LRnode)->getData();
	LRnode->setData(resultLR); //putting the left-right child node to the derived value

	//starting deriving and asignation of Right-Left child node	
	Node* RLnode= new Node(); RLnode->setData(LLnode->getData()); //create new node with the value to be derivated taken from the value saved in left-left node
	node->getRight()->setLeft(RLnode,node->getRight()); 
    std::string resultRL= derive(var,RLnode)->getData();
	RLnode->setData(resultRL); //putting the right-left child node to the derived value
	
		std::cout<<"RR="<<RRnode->getData() <<std::endl;
		std::cout<<"RL="<<RLnode->getData() <<std::endl;
		std::cout<<"LL="<<LLnode->getData() <<std::endl;
		std::cout<<"LR="<<LRnode->getData() <<std::endl;
	
	
return RRnode; 

}

  
 
  




