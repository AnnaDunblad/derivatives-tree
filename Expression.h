#include<string>

class Expression
{
 private: 
  std::string str;
  bool isOperator(char);
  bool isNumber(char);
  int preProcess();
  int getHighestPrecendence();
 public:
  
};
