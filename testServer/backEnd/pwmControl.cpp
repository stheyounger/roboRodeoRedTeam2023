
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

int main() {
	gpioInitialise();
	gpioServo(12,100);
	sleep(1);
	gpioServo(12, 2500);
	sleep(2);
	gpioTerminate();
}
