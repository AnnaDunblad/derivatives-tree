#include "Tree.h"
#include "Node.h"

// Divide and conquer 
int Tree::decode(Expression exp){
	// Search for the operator with highest precedence. 
  //  int pos = getHighestPrecendence(str);
  
  // NOTE:  Remove outer paranthesis. (because the split algorithm will split (b+a) into the two incorrect subexpressions "(b" and "a)" which will produce errors
  
  
	// If no operator found, return this number
	
	// Split the expression in two by this operator and call this function recursively
	
  return 0;
}




int Tree::maxLvl(Node* node){
  if (node == NULL)
    return 0;
  return std::max(maxLvl(node->getLeft()), maxLvl(node->getRight())) + 1;
}

bool Tree::isAllElementsNull(std::vector<Node*> list) {
  for(std::vector<Node*>::iterator it = list.begin(); it != list.end(); ++it) {
    if((*it) != NULL)
      return false;
  }
  return true;
}
void Tree::printNode(Node* root) {
  int maxLevel = maxLvl(root);
  printNodeInternal(std::vector<Node*>(1,root), 1, maxLevel);
}

void Tree::printNodeInternal(std::vector<Node*> nodes, int level, int maxLevel) {
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

  printNodeInternal(newNodes, level + 1, maxLevel);
}







