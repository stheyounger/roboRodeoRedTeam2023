#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
//#include <softServo.h>
#include <softPwm.h>


void pwmTest(int);
void pinOnOffTest(int);

int main() {
	printf("Pwm output test.\n");
	
	if (wiringPiSetup() == -1)
    		return 1 ;
	
	const int targetPin = 0;
	
	pwmTest(targetPin);
	
	return 0;
}

void pwmTest(int targetPin) {
	int pwmRange = 100;
        softPwmCreate(targetPin, 0, pwmRange);
        
        int delayMilis = 100;
        for (;;) {
                for (int bright = 0; bright < pwmRange; ++bright) {
                        softPwmWrite(targetPin, bright);
                        delay(delayMilis);
                }
                for (int bright = pwmRange; bright >= 0; --bright) {
                        softPwmWrite(targetPin, bright);
                        delay(delayMilis);
                }
        }
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
