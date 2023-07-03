
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

int main() {
	gpioInitialise();
	gpioSetMode(17, PI_OUTPUT);
	gpioWrite(17, 1);
	sleep(1);
	gpioWrite(17, 0);

	hardware_PWM(12, 800, 250000) // 800Hz 25% dutycycle

}
