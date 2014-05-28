#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>  // cin and cout
#include <sstream>   // To convert string to integer
#include <algorithm> // std::max
#include <vector>    // std::vector
#include <cmath>     // pow()
#include <stdexcept> // Exceptions 
#include <map>       // Map used when calculating value of an tree, save all variables and their values in a map
#include "Expression.h"
class Expression;


// Class representing a binary tree
class Node{

 private: 
  Node* _rightChildren;
  Node* _leftChildren;
  std::string _data; 

  int maxLevel(Node* node);
  bool isAllElementsNull(std::vector<Node*>);
  void printTreeInternal(std::vector<Node*>, int, int);
  int shorten(Node*); 
  char getOperator();
  bool isNumeric();
  float getNumber();
  static float doOperation(float,char,float); //calculate value of inserted operators of nodes if not symbolic tree
  Node* differentiate(std::string variable, Node* node, Node* newNode); // differentiate this tree, recursive
  std::string toExpression(Node* newNode); //turns the resulttree into a string

 public: 

  Node(); // Empty constructor that we use when creating nodes to three
  Node* getRight();
  Node* getLeft();
  void setRight(Node*);
  void setLeft(Node*);
  void setData(float);
  void setData(std::string);
  std::string getData();
  
  // Return a printable representation of this tree
  void printTree();
  

  
  // Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
  void shorten();
  


	Node* differentiate(std::string var);   //overload method to return result of differentiate
	
	static Node* copyNodeTree(Node* node); //copies a tree nodewise recursive 
	Expression  toExpression(); //overload method to return result of toExpression
	 std::string expression;
	 int i;

	
	void addDifferentiate(std::string var, Node* node, Node* newNode); //differentiate additions
	void subDifferentiate(std::string var, Node* node, Node* newNode); //differentiate subtractions
	void multDifferentiate(std::string var, Node* node, Node* newNode); //differentiate mutiplication
	void divDifferentiate(std::string var,Node* node, Node* newNode); //differentiate divisions
	void powDifferentiate(std::string var,Node* node, Node* newNode);//differentiate powers	
	void cosDifferentiate(std::string var, Node* node, Node* newNode); //differentate cin	
	void sinDifferentiate(std::string var,Node* node, Node* newNode); //differetiate sin
	void lnDifferentiate(std::string var, Node* node, Node* newNode); //differentiate ln



	

  // Returns a map with all variables used in this tree (and their values set to 0)
  void getVariables(std::map<std::string,float>&);
  // Calculate the value of this tree with the variables set to the values in the map
  float calculate(std::map<std::string,float>&);
  
};

  
#endif  
