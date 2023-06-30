#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <softPwm.h>
//#include <softServo.h>

void servoTest(int);
void pwmTest(int);
void pinOnOffTest(int);

int main(int argc, char **argv) {
	const int servoNumber = atoi(argv[1]);
	const int servoPosition = atoi(argv[2]);
	printf("servo %d, position %d \n", servoNumber, servoPosition);

	if (wiringPiSetup() == -1)
    		return 1 ;


	int targetPin;
	if (servoNumber == 1) {
		targetPin = 8;
	} else {
		targetPin = 12;
	}

        softPwmCreate(targetPin, 0, 100);
	softPwmWrite(targetPin, servoPosition);

//	servoTest(targetPin);
//	pwmTest(targetPin);

	return 0;
}

//void servoTest(int targetPin) {
//	softServoSetup(0, 1, 2, 3, 4, 5, 6, 7);
//	softServoWrite(0,  0);
//	for(;;) {
//		delay(10);
//	}
//}

void pwmTest(int targetPin) {
	const int pwmRange = 100;
        softPwmCreate(targetPin, 0, pwmRange);
        
        const int delayMilis = 100;
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
