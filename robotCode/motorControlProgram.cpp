#include <stdio.h>
int main(int argc, char const *argv[]) {
	printf("Running motor control program.\n");	

  	char* l = NULL;
  	size_t n;
	while (getline(&l,&n, stdin) != -1 ) {
      		printf("MCtrl, from stdin: %s",l);
    	}
	return 0;
}
