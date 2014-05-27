#include <fstream>   // To read/write files
#include <iostream>  // Input/Output
#include <vector>    // vector of trees
#include "Expression.h"

// Compile with  g++ -W -Wall -pedantic program.cpp Expression.cpp Node.cpp NodeF.cpp -o program


// Howto print the expressions (true==trees, false==strings)
bool treeViz = false;

// Function to print the tree in a desired way
void print(Node* tree)
{
  if(treeViz)
    tree->printTree();
  else
    std::cout << tree->toExpression().toString()<<std::endl;
}




int main(int argc, char* argv[])
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
  
  if(argc>1){
    // ------------------------------------------
    //   Read file passed as program parameter
    // ------------------------------------------  
    file.open(argv[1], std::ios::in);
    if(!file.is_open()){
      std::cout << "Input file not found!" << std::endl;
      return -1;
    }
    while(getline(file, line)){
      trees.push_back(Expression(line).toTree());
      std::cout << trees.back()->toExpression().toString() << " added."<<std::endl;
    }
    file.close();
    
  }else{
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
    
  }
  

  if(argc>1){
    // ------------------------------------------
    //     When using command line default is 
    //         the string representation
    // ------------------------------------------
    std::cout << "Will display expressions as strings." << std::endl;
    treeViz = false;
  }else{
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
  }
  
  
  
  // ------------------------------------------
  //  If we are using the command line interface
  // ------------------------------------------
  if(argc>1){
    
    // Where do we want to save the expressions? 
    if(argc>2)
      // Save the data to another file
      file.open(argv[2], std::ios::out | std::ios::trunc);
    else
      // Save the data to same as input (overwrites the data)
      file.open(argv[1], std::ios::out | std::ios::trunc);
    
    
    // Print message if we don't apply shorting!
    if(argc>3 && std::string(argv[3]) == "-ns"){
      std::cout << "Does not short expression" << std::endl;
    }
    
    
    // For every expression currently loaded in the vector
    for(unsigned int i = 0; i < trees.size(); i++) {
      
      // Short expression (if not -ns flag provided)
      if(!(argc>3 && std::string(argv[3]) == "-ns"))
	trees[i]->shorten();
      
      
      std::cout << "Will differentiate " << i+1 << ": " << trees[i]->toExpression().toString()<< std::endl;
      variables.clear();
      // Get all variables existing in the current tree
      trees[i]->getVariables(variables);
      // If the tree didn't contain any variable I just pass x as dummy variable
      if(variables.empty()){
	var="x";
      }
      // If the tree just has one variable we don't need to bother the user with a question, just use the variable
      else if(variables.size()==1){
	std::cout << "Differentiate with respect on " << variables.begin()->first << std::endl;
	var = variables.begin()->first;
      }
      // If the tree has several variables we need to ask which one the user want to derive with respect on
      else{
	do{
	  std::cout << "Differentiate with respect on variable?" << std::endl;
	  for(std::map<std::string,float>::iterator it=variables.begin(); it!=variables.end(); ++it){
	    std::cout << it->first << std::endl;
	  }
	  std::cin >> var;
	  
	  // Loop until the user picks a existing variable
	}while(variables.find(var) == variables.end());
      }
      // Differentiate the expression
      trees[i] = trees[i]->differentiate(var);

      // Short expression (if not -ns flag provided)
      if(!(argc>3 && std::string(argv[3]) == "-ns"))
	trees[i]->shorten();
      

      
      std::string derivada = trees[i]->toExpression().toString();
      file << derivada << std::endl;
      std::cout << "Saved expression " << i+1 << ": " << derivada << std::endl;
    }
    file.close();
  }

  // Reset the cout, like no parameter was passed (because we don't want to enter this if once more)
  argc = 1; 
  
  
  
  
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
      for(unsigned int i = 0; i < trees.size(); i++) {
	variables.clear();
	std::cout << "Expression " << i+1 <<":"<< std::endl;
	print(trees[i]);
	
	// ------------------------------------------
	//      Ask for differentiate variable
	// ------------------------------------------  
	trees[i]->getVariables(variables);
	// If the tree didn't contain any variable I just pass x as dummy variable
	if(variables.empty()){
	  var="x";   
	}
	// If the tree just has one variable we don't need to bother the user with a question, just use the variable
	else if(variables.size()==1){
	  std::cout << "Differentiate with respect on " << variables.begin()->first << std::endl;
	  var = variables.begin()->first;
	}
	// If the tree has several variables we need to ask which one the user want to differentiate with respect on
	else{
	  do{
	    std::cout << "Differentiate with respect on variable?" << std::endl;
	    for(std::map<std::string,float>::iterator it=variables.begin(); it!=variables.end(); ++it){
	      std::cout << it->first << std::endl;
	    }
	    std::cin >> var;
	    
	    // Loop until the user picks a existing variable
	  }while(variables.find(var) == variables.end());
	}
	// Differentiate the expression
	trees[i] = trees[i]->differentiate(var);
	// Print it again (just debug)
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
