
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

using namespace std;

int main(int argc, char **argv) {
        const int servoNumber = stoi(argv[1]);
        const int servoPosition = stoi(argv[2]);
	printf("servo %d, position %d\n", servoNumber, servoPosition);

        int targetPin;
        if (servoNumber == 1) {
                targetPin = 23;
        } else {
                targetPin = 26;
        }

	gpioInitialise();
	gpioServo(targetPin, servoPosition);
//	gpioTerminate();
}

