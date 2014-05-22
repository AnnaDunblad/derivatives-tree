a+b*c


// Divide and conquer 
int Tree::decode(std::string str){
	// Search for the operator with highest precedence. 
	int numberOfParantesis = 0;
	int deepness = 0;
	int groundest = 255;
	int groundestPosition = -1;
	
	for(int i = 0; i < str.size(); ++i) {
		switch(str[i])
		case '(':
			numberOfParantesis++;
			deepness++;
			break;
		case ')':
			if(numberOfParantesis==0)
				return -1;
			numberOfParantesis--;
			deepness--;
		case '*':
		case '/':
			
			break;
		case '+':
		case '-':
			deepness++;
			// If this is the operator with the highest precedence up to now
			if(deepness < groundest)
			{
				groundest = deepness;
				groundestPosition = i;
			}
	}
	// If no operator found, return this number
	
	// Split the expression in two by this operator and call this function recursively
	
	
}