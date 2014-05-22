#ifndef TREE_H
#define TREE

// Operators in precedence order
enum Operator{
	MULT, DIV, ADD, SUB, NUM
}

class Tree{
	
	public: 
		
		// Takes a string expression and convert it to a tree, recursively, returns 1 if successful
		int decode(std::string expression);
		
		// Return a printable representation of this tree (i.e. convert it to an expression)
		std::string toString();
		
		// Shorten the expression tree (remove zeroes, remove outermost parenthesis, 
		void Short();
		
		// Derive this tree, recursive
		void derive();
		
		class Node{
			private: 
				Tree _rightChildren;
				Tree _leftChildren;
				//Node _parent;
				Operator _operator;
				int _data; // If _operator is NUM we keep the number in _data
			public: 
				Node(Operator _operator, Tree leftChildren = NULL, Tree rightChildren = NULL);
				//Node getParent();
				Tree getRightChildren();
				Tree getLeftChildren();
				void setRightChildren(Tree tree);
				void setLeftChildren(Tree tree);
		}
		
		
}

#endif
