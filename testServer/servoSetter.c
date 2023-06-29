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

	softPwmCreate(targetPin, 100, 100);
	softPwmWrite(targetPin, 50);
	int delayTime = 20;
	for (;;) {
    		for (int bright = 0 ; bright < 100 ; ++bright) {
      			softPwmWrite (1, bright) ;
      			delay(delayTime) ;
    		}
		for (int bright = 100 ; bright >= 0 ; --bright) {
      			softPwmWrite(1, bright) ;
      			delay(delayTime);
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
