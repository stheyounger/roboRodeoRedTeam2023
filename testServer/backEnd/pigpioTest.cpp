
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>

int main() {
	gpioInitialise();
	gpioSetMode(17, PI_OUTPUT);
	gpioWrite(17, 1);
	sleep(1);
	gpioWrite(17, 0);

	gpioServo(12, 1000);
	sleep(1);
	gpioServo(12, 2000);
	sleep(1);

//	gpioSetMode(12, PI_OUTPUT);
//	gpioHardwarePWM(12, 25000, 0);	
//	sleep(1);
//	gpioHardwarePWM(12, 25000, 500000);	

}
