#include <stdio.h>
#include <wiringPi.h>

int main() {
	printf("Hey there\n");
	
	if (wiringPiSetup () == -1)
    		return 1 ;
	
	pinMode (0, OUTPUT) ;
	
	for (;;) {
    		digitalWrite (0, 1) ;       // On
    		delay (500) ;               // mS
    		digitalWrite (0, 0) ;       // Off
    		delay (500) ;
  	}
	
	return 0;
}
