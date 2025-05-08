#include <stdio.h>

int main(void){

	char str[10] = "Hello Worl";
	printf("%s", str);
	char* h = &str[2];
	*h = 'p';
	printf("%s", str);

	return 0;
	



}
