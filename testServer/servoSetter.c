#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <softPwm.h>

void pinOnOffTest(int);

int main() {
	printf("Pwm output test.\n");
	
	if (wiringPiSetup() == -1)
    		return 1 ;
	
	int targetPin = 23;

//	pinOnOffTest(targetPin);

	softPwmCreate(targetPin, 0, 100);
//	softPwmWrite(targetPin, 50);
	
	int delayMilis = 200;
	for (;;) {
    		for (int bright = 0 ; bright < 100 ; ++bright) {
      			softPwmWrite (targetPin, bright) ;
      			delay(delayMilis) ;
    		}
		for (int bright = 100 ; bright >= 0 ; --bright) {
      			softPwmWrite(targetPin, bright) ;
      			delay(delayMilis);
    		}
  	}
	
	return 0;
}

void pinOnOffTest(int targetPin) {
        pinMode (targetPin, OUTPUT) ;
        for (;;) {
                digitalWrite (targetPin, 1) ;       // On
                delay (500) ;               // mS
                digitalWrite (targetPin, 0) ;       // Off
                delay (500) ;
        }       
	
}
