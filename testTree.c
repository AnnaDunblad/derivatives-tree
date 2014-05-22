#include <stdio.h>

int main()
{
	char str[] = "(b+c+b)+1";
	printf("%s\n",str);
	// Search for the operator with highest precedence. 
	int numberOfParantesis = 0;
	int deepness = 0;
	int groundest = 255;
	int groundestPosition = -1;
	char lastOperador = 0;
	
	for(int i = 0; str[i] != 0; ++i) {
		switch(str[i])
		{
			case '(':
				/*if(lastOperador=='n')
				{
					printf("%d: explicit * detected\n", i);
					deepness++;
					printf("%d: deepness increased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}*/
				printf("%d: ( detected\n", i);
				numberOfParantesis++;
				deepness++;
				break;
			case ')':
				printf("%d: ) detected\n", i);
				if(numberOfParantesis==0)
					return -1;
				numberOfParantesis--;
				deepness--;
				break;
			case '*':
			case '/':
				printf("%d: */ detected\n", i);
				if(lastOperador=='+')
				{
					deepness++;
					printf("%d: deepness increased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}
				lastOperador = '*';
				break;
			case '+':
			case '-':
				printf("%d: +- detected\n", i);
				// If this is a new operator
				if(lastOperador=='*')
				{
					deepness--;
					printf("%d: deepness decreased to %d\n", i, deepness);
				}
				// If this is the operator with the highest precedence up to now
				if(deepness < groundest)
				{
					groundest = deepness;
					groundestPosition = i;
					printf("%d: groundestPosition=%d\n", i, i);
				}
				lastOperador = '+';
			default: 
				// Number (or variable detected)
				// TODO: Concat if several caracters, i.e.  8132
				//lastOperador='n';
				break;
		}
	}
}