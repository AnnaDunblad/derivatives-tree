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
	std::cout<<"Entering switch function"<<std::endl;
	std::cout<<"Node Data="<<node->getData() << std::endl;
		  char dataNode=node->getOperator();
		   switch(dataNode)   //test every operator - if number, variable or mult, go to a method
		   {
			case '*' :
				if(node->getRight()!=NULL && node->getRight()!=NULL)
				{
					if(node->getRight()->getData()== "0"  || node->getLeft()->getData()=="0")
					{

						std::cout<<"A child node to a * is cero"<<std::endl;
						node->setRight(new Node(),node);
						node->setLeft(new Node(),node);
						node->setData("+");
						node->getRight()->setData("0");
						node->getLeft()->setData("0");
					}
					else
					{
						multDerive(variable,node);
					}
				}
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
		std::cout<<"Switch Sentence Done"<<std::endl;
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

return node;
 }
 
Node* Node::copyNode(Node* node)
{
	std::cout<<"Entering copyNode function"<<std::endl;
	Node* newNode = new Node();
	newNode->setData(node->getData());

return newNode;
}

void Node::multDerive(std::string var,Node* node)
{
	std::cout<<"Entering multDerive function"<<std::endl;
	/*
	Node* RRnode = copyNode(node->getRight());
	Node* LLnode = copyNode(node->getLeft());

	//rule: D(f*g)=f'*g +g'*f
	node->setData("+");
	std::cout<<"putting first node in mult to +"<<std::endl;
	node->getRight()->setData("*");
	node->getLeft()->setData("*");
	std::cout<<"putting child nodes to * "<<std::endl;
	
	node->getLeft()->setRight(derive(var,node->getRight()),node->getLeft()); //derive left-right child node 
	node->getRight()->setLeft(derive(var,node->getLeft()),node->getLeft());//derive right-left child node 
	

	node->getLeft()->setLeft(LLnode,node->getLeft()); 						//put left-left child node to copied value (the one not derivated)
	node->getRight()->setRight(RRnode,node->getRight()); //put right-right child node to copied value (the one not derivated)
	std::cout<<"Child nodes copied"<<std::endl;
	
	*/

	std::cout<<"putting first node in mult to +"<<std::endl;
}



Expression Node::toExpression(){
  // TODO: Implement!
  return Expression("a+b*sin(d)");
}


/*
 void Node::divDerive(std::string var, Node* node); 
 {
 
 
 }
  
  void Node::setData(std::string data){
  _data = data;
}
std::string Node::getData(){
  return _data;
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
} */
  
  
 
  




