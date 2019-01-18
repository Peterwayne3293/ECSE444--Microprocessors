#include <common_lib.h>
#include <add.h>

int main(){
	
	/*
	//Hello Wolrd
	printf("Hello world");
	*/
	
	float n1 = 1.1;
	float n2 = 2.2;
	
	float c = add_c(n1, n2);
	float a = add_asm(n1, n2);
	
	printf("C subroutine sum = %f\n", c);
	printf("Assembly subroutine sum = %f\n", a);
	
	return 0;
}
