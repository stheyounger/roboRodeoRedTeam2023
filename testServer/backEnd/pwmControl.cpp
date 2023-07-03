
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

int main() {
	gpioInitialise();
	gpioSetMode(17, PI_OUTPUT);
	gpioWrite(17, 1);
	sleep(1);
	gpioWrite(17, 0);

	gpioServo(12, 500);
	sleep(1);
	gpioServo(12, 2500);
	sleep(2);
}
