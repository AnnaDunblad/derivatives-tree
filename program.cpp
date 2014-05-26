#include <fstream>   // To read/write files
#include <iostream>  // Input/Output
#include <vector>    // vector of trees
#include "Expression.h"

// Compile with  g++ -W -Wall -pedantic program.cpp Expression.cpp Node.cpp NodeF.cpp -o program


// Howto print the expressions (true==trees, false==strings)
bool treeViz = false;
void print(Node* tree)
{
  if(treeViz)
    tree->printTree();
  else
    std::cout << tree->toExpression().toString()<<std::endl;
}



int main()
{
  // Variable used to make menu selections
  int selection;
  // Variable hold the filename to file
  std::string filename;
  // File handle when reading or writing data
  std::fstream file; 
  // Loaded expressions
  std::vector<Node*> trees;
  // Possible variables
  std::map<std::string,float> variables;
  // Variable to differentiate with respect to
  std::string var;
  // Line when reading files
  std::string line;
 



  std::cout << "Welcome to the automatic derivative calculator" << std::endl;
  

  // ------------------------------------------
  // Ask the user for a (or several) expressions
  // ------------------------------------------  
 newExpression:
  do{
    std::cout << "Where do you have the expressions?"<< std::endl;
    std::cout << "1. File"<<std::endl<<"2. Terminal"<<std::endl;
    std::cin >> selection; 
  }while(selection < 1 || selection>2);
  
  switch(selection){
  case 1:
    // ------------------------------------------
    //        Read file with expressions 
    // ------------------------------------------
    do{
      std::cout << "Enter a valid filename to a file with expressions (one expression/line):"<<std::endl;
      std::cin >> filename;
      file.open(filename.c_str(), std::ios::in);
    }while(!file.is_open());

    while(getline(file, line)){
      trees.push_back(Expression(line).toTree());
      std::cout << trees.back()->toExpression().toString() << " added."<<std::endl;
    }
    file.close();
    break;
    
    
  case 2:
    // ------------------------------------------
    //       Ask the user for an expression
    // ------------------------------------------
    std::cout << "Enter a expression:" <<std::endl;
    std::string exp;
    std::cin >> exp;
    trees.push_back(Expression(exp).toTree());
    break;
  }
  




  // ------------------------------------------
  //  Ask the user how to display expressions
  // ------------------------------------------  
 newViz:
  do{
    std::cout << "How do you prefer the visualizing of expressions?" <<std::endl;
    std::cout << "1. Tree " << std::endl << "2. String" <<std::endl;
    std::cin >> selection;
  }while(selection<1 || selection > 2);
  treeViz = selection==1; 
  





  // NOTE: Exits loop by using goto (This is the last remaining stronghold for the use of goto)
  while(true){ 
  // ------------------------------------------
  // Ask the user how to manipulate expression
  // ------------------------------------------  
    do{
      std::cout << "What do you want to do now?" << std::endl;
      std::cout << "1. Differentiate expression(s)" << std::endl;
      std::cout << "2. Shorten expression(s)" << std::endl;
      std::cout << "3. Calculate expression(s)" << std::endl;
      std::cout << "4. Show expression(s)" << std::endl;
      std::cout << "5. Save expresion(s) to file" << std::endl;
      std::cout << "6. Input new expresion(s)" << std::endl;
      std::cout << "7. Change vizualisation"<<std::endl;
      std::cout << "8. End program"<<std::endl;
      std::cin >> selection;
    }while(selection<1 || selection>8);
    
    
    switch(selection){
    case 1:
      // ------------------------------------------
      //       Differentiate the expressions
      // ------------------------------------------  
      std::cout << "Differentiate with respect on variable? (x, y, z, etc.)" << std::endl;
      std::cin >> var;
      for(unsigned int i = 0; i < trees.size(); i++) {
	std::cout << "Expression " << i+1 <<":"<< std::endl; 
	trees[i] = trees[i]->derive(var);
	print(trees[i]);
      }
      break;


    case 2:
      // ------------------------------------------
      //            Short the expressions
      // ------------------------------------------  
      for(unsigned int i = 0; i < trees.size(); i++) {
	std::cout << "Expression " << i+1 <<":"<< std::endl; 
	trees[i]->shorten();
	print(trees[i]);
      }
      break;


    case 3:
      // ------------------------------------------
      //   Calculate the value of the expression 
      //      (first ask for variable values)
      // ------------------------------------------  
      for(unsigned int i = 0; i < trees.size(); i++) {
	variables.clear();
	std::cout << "Expression " << i+1 <<":"<< std::endl;
	trees[i]->getVariables(variables);
	for(std::map<std::string,float>::iterator it=variables.begin(); it!=variables.end(); ++it){
	  std::cout << "Insert value on variable " << it->first << ":" << std::endl;
	  float value;
	  std::cin >> value;
	  variables[it->first] = value;
	}
	
	std::cout << "The result is " << trees[i]->calculate(variables) << std::endl;
      }      
      break;
    case 4:
      // ------------------------------------------
      //   Display the expression in desired way
      // ------------------------------------------  
      for(unsigned int i = 0; i < trees.size(); i++) {
	std::cout << "Expression " << i+1 <<":"<< std::endl; 
	print(trees[i]);
      }
      break;
    case 5:
      // ------------------------------------------
      //      Save the expressions to a file 
      // ------------------------------------------  
      std::cout << "Enter a filename to save expressions to (will overwrite content if the file exists)" << std::endl;
      std::cin >> filename;
      file.open(filename.c_str(), std::ios::out | std::ios::trunc);
      for(unsigned int i = 0; i < trees.size(); i++) {
	file << trees[i]->toExpression().toString();
	std::cout << "Saved expression " << i+1 <<"."<< std::endl; 
      }
      file.close();

      break;
    case 6: 
      // ------------------------------------------
      //           Start over with a new 
      //        (or add another) expression
      // ------------------------------------------  
      do{
	std::cout << "Do you want to remove the current expression(s) first?" << std::endl;
	std::cout << "1. Yes" << std::endl << "2. No" <<std::endl;
	std::cin >> selection;
      }while(selection < 1 || selection > 2);
      if(selection==1)
	trees.clear();
      goto newExpression;
    case 7:
      // ------------------------------------------
      //    Change how expression are displayed
      // ------------------------------------------  
      goto newViz;
    case 8: 
      // ------------------------------------------
      //              End the program
      // ------------------------------------------  
      goto end;
    }
  }
  
  

  // ------------------------------------------
  //               Exit message
  // ------------------------------------------  
 end:
  std::cout << "Thanks for using our program" << std::endl;
  std::cout << "by Fredrik & Anna" << std::endl;
  std::cout << "Copyleft Creative Commons" << std::endl;
   
  return 0;
}
