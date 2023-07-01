#include <cstdio>

#include <wiringPi.h>
#include <softPwm.h>

class PwmController {
        public : void sayHi() {
                printf("hi");
        }
};

int main() {
	const int servoNumber = atoi(argv[1]);
        const int servoPosition = atoi(argv[2]);
        printf("servo %d, position %d \n", servoNumber, servoPosition);

        if (wiringPiSetup() == -1)
                return 1 ;


        int targetPin;
        if (servoNumber == 1) {
                targetPin = 23;
        } else {
                targetPin = 26;
        }

        softPwmCreate(targetPin, 0, 100);
        softPwmWrite(targetPin, servoPosition);



	printf("My age is ");
	PwmController pc;
	pc.sayHi();
}
