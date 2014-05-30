// Author: Fredrik Lofgren and Anna Dunblad
// Date: 30 May
// Open Source for any use by anyone

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
		
		void setData(float);
		int maxLevel(Node*);
		bool isAllElementsNull(std::vector<Node*>);
		void printTree(std::vector<Node*>, int, int);
		char getOperator();
		bool isNumeric();
		float getNumber();
		static float doOperation(float,char,float); //calculate value of inserted operators of nodes if not symbolic tree
		Node* differentiate(std::string, Node*, Node*); // differentiate this tree, recursive
		
		static Node* copyNodeTree(Node*); //copies a tree nodewise recursive 
		static int getOpPrio(char);

		//functions used to differentiate different expressions
		void addDifferentiate(std::string, Node*, Node*); //differentiate additions
		void subDifferentiate(std::string, Node*, Node*); //differentiate subtractions
		void multDifferentiate(std::string, Node*, Node*); //differentiate mutiplication
		void divDifferentiate(std::string,Node*, Node*); //differentiate divisions
		void powDifferentiate(std::string,Node*, Node*);//differentiate powers	
		void cosDifferentiate(std::string, Node*, Node*); //differentate cos	
		void sinDifferentiate(std::string,Node* node, Node*); //differetiate sin
		void lnDifferentiate(std::string, Node*, Node*); //differentiate ln


	public: 

		Node(); // Empty constructor that we use when creating nodes to three
		Node* getRight();
		Node* getLeft();
		void setRight(Node*);
		void setLeft(Node*);
		void setData(std::string);
		std::string getData();
		friend std::ostream& operator<<(std::ostream&, Node*);

		// Overload method to the private void printTree(std::vector<Node*>, int, int). 
		void printTree();

		// Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
		void shorten();

		Node* differentiate(std::string);   //overload method to return result of differentiate

		
		// Returns a map with all variables used in this tree (and their values set to 0)
		void getVariables(std::map<std::string,float>&);
		// Calculate the value of this tree with the variables set to the values in the map
		float calculate(std::map<std::string,float>&);

};

  
#endif  
