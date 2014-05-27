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
 {		 std::cout<<"derive funcion initialized"<<std::endl;
		char dataNode=node->getOperator();
		 std::cout<<"dataNode="<<dataNode<<std::endl;
		   switch(dataNode)   //test every operator - if number, variable or mult, go to a method
		   {
			case '*' :	
				multDerive(variable,node,newNode);			
				break;
			case '/':
				divDerive(variable,node,newNode);
				break;
			case  '^':	
				std::cout<<" ^ detected"<<std::endl;
				powDerive(variable,node, newNode);
				break;
				
			case  '+':	
				std::cout<<" + detected"<<std::endl;
				addDerive(variable,node, newNode);
				break;
			case '-':
				std::cout<<" - detected"<<std::endl;
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
	std::cout<<"switch sentance done"<<std::endl;
	std::cout<<"newNode="<<newNode->getData()<<std::endl;
return newNode;
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
 	newNode->setLeft(derive(var,leftTree,new Node()),newNode); //putting the left child node to the derived value
	newNode->setRight(derive(var,rightTree,new Node()),newNode); //putting the right child node to the derived value

}
Expression Node::toExpression(){
  // TODO: Implement!
  return Expression("a+b*sin(d)");
}

 


void Node::subDerive(std::string var,Node* node, Node* newNode)
{
	//Putting top nod to -
	newNode->setData("-");
	
	//Copying the values of right and left child node to save for later use 
	 Node* rightTree = copyNodeTree(node->getRight()); 	 
	 Node* leftTree = copyNodeTree(node->getLeft());
	 
	//creating new tree with derived values
 	newNode->setLeft(derive(var,leftTree,new Node()),newNode); //putting the left-right child node to the derived value
	newNode->setRight(derive(var,rightTree,new Node()),newNode); //putting the left-right child node to the derived value

}
void Node::multDerive(std::string var,Node* node, Node* newNode) 	//rule: D(f*g)=f'*g +g'*f
{ 
	// if multiplied with cero-> put branch to + 0 to prevent infinite loop
	if(node->getRight()->getData()== "0" || node->getRight()->getData()=="0")
	{
		newNode->setData("+");
		newNode->setRight((new Node()),newNode);
		newNode->getRight()->setData("0");
		newNode->setLeft((new Node()),newNode);
		newNode->getLeft()->setData("0");
	}
	//Putting top nod to +
	newNode->setData("+");
	//Copying the values of right and left child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); 	 
	Node* leftTree = copyNodeTree(node->getLeft());

	//creating new tree
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
	
	//putting the right and left children to * 
	newNode->getRight()->setData("*");
	newNode->getLeft()->setData("*");

	newNode->getLeft()->setLeft(leftTree,newNode->getLeft());   //put left-left child tree to copied tree (the one not derivated)
	newNode->getRight()->setLeft(derive(var,leftTree,new Node()),newNode->getRight()); //putting the left-right child node to the derived value
	newNode->getLeft()->setRight(derive(var,rightTree,new Node()),newNode->getLeft()); //putting the left-right child node to the derived value
	newNode->getRight()->setRight(rightTree,newNode->getRight()); //put right-right child tree to copied tree (the one not derivated)

}

 void Node::divDerive(std::string var, Node* node, Node* newNode) //rule: (f/g)'= (f'g-fg')/(g*g)
 {
 	//Putting top nod to /
	newNode->setData("/");
	//Copying the values of right and left child node to save for later use
	Node* leftTree = copyNodeTree(node->getLeft()); //f	
	Node* rightTree1 = copyNodeTree(node->getRight()); //g
	Node* rightTree2 = copyNodeTree(node->getRight()); //g, making a second to later be left-left-right node

	//creating first branches to new tree
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
	
	//putting the right and left children to - resp. ^ 
	newNode->getLeft()->setData("-");
	newNode->getRight()->setData("^");
	
	//Creating right branch and asigning the nodes: g and 2 (g^2)
	newNode->getRight()->setRight(new Node(),newNode->getRight());
	newNode->getRight()->getRight()->setData("2"); //2
	newNode->getRight()->setLeft(rightTree1,newNode->getRight()); //g

	//Creating left branch and asigning the nodes: 
	 newNode->getLeft()->setLeft(new Node(),newNode->getLeft());
	 newNode->getLeft()->setRight(new Node(),newNode->getLeft());
	 Node* leftLeft=newNode->getLeft()->getLeft();
	 Node* leftRight= newNode->getLeft()->getRight();
	
	//putting the left-left and left-right children to * 
	leftLeft->setData("*");
	leftRight->setData("*");
	//putting the left-left-right node and left-right-left node to g and f
	leftLeft->setRight(rightTree2,leftLeft);
	leftRight->setLeft(leftTree,leftRight);
	//putting the left-left-left node and left-right-right node to f' and g'
	leftLeft->setLeft(derive(var,leftTree,new Node()),leftLeft);
	leftRight->setRight(derive(var,rightTree1,new Node()),leftRight);
	std::cout<<"derivation of div done"<<std::endl;
	
 } 
 
 void Node::powDerive(std::string var,Node* node, Node* newNode)//Rule: (f^g)'=f^g(f'(g/f)+g'
 {
  	//Putting top nod to *
	newNode->setData("*");
	
	//Copying the values of right and left child node to save for later use in serveral new nodes
	Node* leftTree1 = copyNodeTree(node->getLeft()); //f1	
	Node* leftTree2 = copyNodeTree(node->getLeft()); //f2
	Node* leftTree3 = copyNodeTree(node->getLeft()); //f3	
	Node* rightTree1 = copyNodeTree(node->getRight()); //g1
	Node* rightTree2 = copyNodeTree(node->getRight()); //g2
	
	//creating first branches to new tree
	newNode->setRight(new Node(),newNode);
	newNode->setLeft(new Node(),newNode);
	
	//putting the right and left children to ^ resp. + 
	newNode->getLeft()->setData("^");
	newNode->getRight()->setData("+");
	
	
	//Creating left branch and asigning the nodes: 
	 newNode->getLeft()->setLeft(leftTree1,newNode->getLeft()); //LL
	 newNode->getLeft()->setRight(rightTree1,newNode->getLeft()); //LR


	
	//Creating right-left branch and asigning the nodes: 
	 newNode->getRight()->setLeft(new Node(),newNode->getRight());
	 Node* rightLeft=newNode->getRight()->getLeft(); //RL
	 rightLeft->setData("*"); //RL
	 rightLeft->setLeft(derive(var,leftTree1,new Node()),rightLeft); // RLL
	 rightLeft->setRight(new Node(),rightLeft); //RLR
	 rightLeft->getRight()->setData("/"); //RLR
	 rightLeft->getRight()->setLeft(rightTree2,rightLeft->getRight()); //RLRL
	  rightLeft->getRight()->setRight(leftTree2,rightLeft->getRight()); //RLRR
	 
	//Creating right-right branch and asigning the nodes:
		newNode->getRight()->setRight(new Node(),newNode->getRight());
		Node* rightRight=newNode->getRight()->getRight(); //RR
		rightRight->setData("*"); //RR
		rightRight->setLeft(derive(var,rightTree1,new Node()),rightRight); //RRL
		rightRight->setRight(new Node(),rightRight); //creating node RRR
		Node* rightRightRight=rightRight->getRight();
		rightRightRight->setData("&"); //& representing ln,  RRR
		rightRightRight->setLeft(new Node(),rightRightRight);
		rightRightRight->getLeft()->setData(" "); //node filled with " " RRRL
		rightRightRight->setRight(leftTree3,rightRightRight); // RRRR
		
	std::cout<<"derivation of pow done"<<std::endl;
	
	

	
	
 }
  
