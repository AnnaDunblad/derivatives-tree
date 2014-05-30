// Author: Anna Dunblad
// Date: 30 May
// Open Source for any use by anyone
#include "Node.h"

// Create a new floating node
Node::Node()
{
  _leftChildren = NULL;
  _rightChildren = NULL;
	}

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

//overload method to return result of differentiate
Node* Node::differentiate(std::string var)
{
	Node* newNode=new Node();

	return differentiate(var,this,newNode);
}




 // differentiate this tree, recursive
Node* Node::differentiate(std::string variable,Node* node, Node* newNode )
 {
		char dataNode=node->getOperator();
		   switch(dataNode)   //test every operator - if number, variable or mult, go to a method
		   {				
			case  '+':	
				addDifferentiate(variable,node, newNode);
				break;
			case '-':
				subDifferentiate(variable,node, newNode);
				break;
			case 'n':
				if(node->getData()==variable){
					newNode->setData("1");
				}
				else
				{
					newNode->setData("0");
				}
				break;
			case '*' :	
				multDifferentiate(variable,node,newNode);			
				break;
			case '/':
				divDifferentiate(variable,node,newNode);
				break;
			case  '^':	
				powDifferentiate(variable,node, newNode);
				break;
			case '%':
				cosDifferentiate(variable,node,newNode);
				break;
			case '#': 
				sinDifferentiate(variable,node,newNode);
				break;
			case '&': 
				lnDifferentiate(variable,node,newNode);
				break;
			default:
				throw std::invalid_argument("Node contains illegal character");
				break;
			}

return newNode;
 }

//copies a tree nodewise recursive 
Node* Node::copyNodeTree(Node* node)
{
	Node* newNode=new Node();
	newNode->setData(node->getData()); //copy first node
	
	if(node->getRight() && node->getLeft())
	{	
		newNode->setRight(copyNodeTree(node->getRight()));
		newNode->setLeft(copyNodeTree(node->getLeft()));

	}
return newNode;
}



std::ostream& operator<<(std::ostream &output,Node* node)
{
if(!node->getLeft() || !node->getRight())	//if tree is only of one node, return only that node
	{
		 return output << node->getData();
	}
	std::string op;	
	switch(node->getOperator())
	{
		case '&':
			op=  "ln";
			break;
		case  '%':
			op=  "cos";
			break;
		case '#':
			op= "sin";
			break;
		default:
			op=node->getData();
			break;	
	}
		
	//put paranthesis around expressions depending on the priority of children compared to parent
	if((Node::getOpPrio(node->getOperator()) > Node::getOpPrio(node->getRight()->getOperator())) && 
		(Node::getOpPrio(node->getOperator()) > Node::getOpPrio(node->getLeft()->getOperator())))
	{
		return output << "(" << node->getLeft() << ")" << op  << "(" << node->getRight() << ")";
	}	
	if(Node::getOpPrio(node->getOperator()) > Node::getOpPrio(node->getRight()->getOperator()))
	{
		return  output <<   node->getLeft()  << op  <<"(" << node->getRight() << ")";
	}	
	
	if(Node::getOpPrio(node->getOperator()) > Node::getOpPrio(node->getLeft()->getOperator()))
	{
		return  output <<  "(" << node->getLeft() << ")" <<  op  <<  node->getRight();
	}
	
	
	return  output << node->getLeft() << op << node->getRight();
	
}

 
 int Node::getOpPrio(char op){
	switch(op) 
	{
		case '+':
		case '-':
			return 1;
		case '*':
		case '/': 
			return 2;
		case '^':
			return 3;
		case '%':
		case '&':
		case '#':
			return 4;
		default:
			return 5;	
	}
 }

 
 //differentiate additions
void Node::addDifferentiate(std::string var,Node* node, Node* newNode)
{
	//Copying the values of right and left child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); 	 
	Node* leftTree = copyNodeTree(node->getLeft());

	//creating new tree
	newNode->setData("+");
	newNode->setLeft(differentiate(var,leftTree,new Node())); //putting the left child node to the differentiated value
	newNode->setRight(differentiate(var,rightTree,new Node())); //putting the right child node to the differentiated value

}
//differentiate subtraction
void Node::subDifferentiate(std::string var,Node* node, Node* newNode)
{
	//Putting top nod to "-"
	newNode->setData("-");

	//Copying the values of right and left child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); 	 
	Node* leftTree = copyNodeTree(node->getLeft());

	//creating new tree with differentiated values
	newNode->setLeft(differentiate(var,leftTree,new Node())); //putting the left-right child node to the differentiated value
	newNode->setRight(differentiate(var,rightTree,new Node())); //putting the left-right child node to the differentiated value

}
//differentiate mutiplication
void Node::multDifferentiate(std::string var,Node* node, Node* newNode) 	//rule: D(f*g)=f'*g +g'*f
{ 
	//Putting top nod to +
	newNode->setData("+");
	//Copying the values of right and left child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); 	 
	Node* leftTree = copyNodeTree(node->getLeft());

	//creating new tree
	newNode->setRight(new Node());
	newNode->setLeft(new Node());

	//putting the right and left children to * 
	newNode->getRight()->setData("*");
	newNode->getLeft()->setData("*");

	newNode->getLeft()->setLeft(leftTree);   //put left-left child tree to copied tree (the one not derivated)
	newNode->getRight()->setLeft(differentiate(var,leftTree,new Node())); //putting the left-right child node to the differentiated value
	newNode->getLeft()->setRight(differentiate(var,rightTree,new Node())); //putting the left-right child node to the differentiated value
	newNode->getRight()->setRight(rightTree); //put right-right child tree to copied tree (the one not derivated)

}
//differentiate divisions
 void Node::divDifferentiate(std::string var, Node* node, Node* newNode) //rule: (f/g)'= (f'g-fg')/(g*g)
 {
	//Putting top nod to /
	newNode->setData("/");
	//Copying the values of right and left child node to save for later use
	Node* leftTree = copyNodeTree(node->getLeft()); //f	
	Node* rightTree1 = copyNodeTree(node->getRight()); //g
	Node* rightTree2 = copyNodeTree(node->getRight()); //g, making a second to later be left-left-right node

	//creating first branches to new tree
	newNode->setRight(new Node());
	newNode->setLeft(new Node());

	//putting the right and left children to - resp. ^ 
	newNode->getLeft()->setData("-");
	newNode->getRight()->setData("^");

	//Creating right branch and asigning the nodes: g and 2 (g^2)
	newNode->getRight()->setRight(new Node());
	newNode->getRight()->getRight()->setData("2"); //2
	newNode->getRight()->setLeft(rightTree1); //g

	//Creating left branch and asigning the nodes: 
	newNode->getLeft()->setLeft(new Node());
	newNode->getLeft()->setRight(new Node());
	Node* leftLeft=newNode->getLeft()->getLeft();
	Node* leftRight= newNode->getLeft()->getRight();

	//putting the left-left and left-right children to * 
	leftLeft->setData("*");
	leftRight->setData("*");
	//putting the left-left-right node and left-right-left node to g and f
	leftLeft->setRight(rightTree2);
	leftRight->setLeft(leftTree);
	//putting the left-left-left node and left-right-right node to f' and g'
	leftLeft->setLeft(differentiate(var,leftTree,new Node()));
	leftRight->setRight(differentiate(var,rightTree1,new Node()));
	
 } 
 //differentiate power
 void Node::powDifferentiate(std::string var,Node* node, Node* newNode)//Rule: (f^g)'=f^g(f'(g/f)+g'
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
	newNode->setRight(new Node());
	newNode->setLeft(new Node());

	//putting the right and left children to ^ resp. + 
	newNode->getLeft()->setData("^");
	newNode->getRight()->setData("+");


	//Creating left branch and asigning the nodes: 
	newNode->getLeft()->setLeft(leftTree1); //LL
	newNode->getLeft()->setRight(rightTree1); //LR



	//Creating right-left branch and asigning the nodes: 
	newNode->getRight()->setLeft(new Node());
	Node* rightLeft=newNode->getRight()->getLeft(); //RL
	rightLeft->setData("*"); //RL
	rightLeft->setLeft(differentiate(var,leftTree1,new Node())); // RLL
	rightLeft->setRight(new Node()); //RLR
	rightLeft->getRight()->setData("/"); //RLR
	rightLeft->getRight()->setLeft(rightTree2); //RLRL
	rightLeft->getRight()->setRight(leftTree2); //RLRR

	//Creating right-right branch and asigning the nodes:
	newNode->getRight()->setRight(new Node());
	Node* rightRight=newNode->getRight()->getRight(); //RR
	rightRight->setData("*"); //RR
	rightRight->setLeft(differentiate(var,rightTree1,new Node())); //RRL
	rightRight->setRight(new Node()); //creating node RRR
	Node* rightRightRight=rightRight->getRight();
	rightRightRight->setData("&"); //& representing ln,  RRR
	rightRightRight->setLeft(new Node());
	rightRightRight->getLeft()->setData(""); // RRRL empty node
	rightRightRight->setRight(leftTree3); // RRRR

 }
 
//differentiate cos
void Node::cosDifferentiate(std::string var, Node* node, Node* newNode){ //rule: (cos(f))'=sin(f)*-f'
	//Putting top nod to *
	newNode->setData("*");
		//Copying the values of right child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); //f
	
	//creating and asigning child nodes
	newNode->setRight(new Node());
	newNode->getRight()->setData("#"); //R
	newNode->getRight()->setRight(rightTree); //RR
	newNode->getRight()->setLeft(new Node());
	newNode->getRight()->getLeft()->setData(""); //RL empty node
	
	newNode->setLeft(new Node());
	newNode->getLeft()->setData("-"); //L
	newNode->getLeft()->setLeft(new Node());	
	newNode->getLeft()->getLeft()->setData("0");
	newNode->getLeft()->setRight(differentiate(var,rightTree,new Node()));
	

}
//differentiate sin
void Node::sinDifferentiate(std::string var, Node* node, Node* newNode){//rule: (sin(f))'=cos(f)*f'
	//Putting top nod to *
	newNode->setData("*");
	
	//Copying the values of right child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); //f	
	//creating and asigning child nodes
	newNode->setRight(new Node());
	newNode->getRight()->setData("%"); //R
	newNode->getRight()->setRight(rightTree); //RR
	newNode->getRight()->setLeft(new Node());
	newNode->getRight()->getLeft()->setData(""); //RL empty node
	
	newNode->setLeft(differentiate(var,rightTree,new Node())); //L
}

//differentiate ln
void Node::lnDifferentiate(std::string var, Node* node, Node* newNode) //(ln(f)'= f'/f
{
	//Putting top nod to /
	newNode->setData("/");
	
	//Copying the values of right child node to save for later use 
	Node* rightTree = copyNodeTree(node->getRight()); //f	
	
	//creating and asigning child nodes
	newNode->setRight(rightTree); //R
	newNode->setLeft(differentiate(var,rightTree,new Node())); //L


}
  
