#include "pca9685.h"

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>

#define PIN_BASE 300
#define MAX_PWM 4096


/**
 * Calculate the number of ticks the signal should be high for the required amount of time
 */
int calcTicks(float impulseMs) {
	int pwmHertz = 50;
	float cycleMs = 1000.0f / pwmHertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

/**
 * input is [0..1]
 * output is [min..max]
 */
float map(float input, float min, float max) {
	return (input * max) + (1 - input) * min;
}

void moveServoOld(int port, float milis) {
	printf("position in %d\n", milis);

	float millis = map(milis, 1, 2);
	printf("after map %d\n", millis);
		
	int tick = calcTicks(millis);
	printf("after tick calc %d\n", tick);

	pwmWrite(PIN_BASE+port, tick);
}

int coerceIn(int value, int min, int max) {
	if (value > max) {
		return max;
	} else if (value < min) {
		return min;
	} else {
		return value;
	}
}

void moveServo(int port, float amount) {
	int minPosition = 0;
	int maxPosition = 4096*2;

	int position = coerceIn(amount, minPosition, maxPosition);
	
	pwmWrite(PIN_BASE + port, position);//calcTicks(2));
}

int setupPwmDriver() {
	wiringPiSetup();

	// Setup with pinbase 300 and i2c location 0x40
	int hertz = 50;
	int fd = pca9685Setup(PIN_BASE, 0x40, hertz);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}

	pca9685PWMReset(fd);
	return 0;
}

int main(void) {
	printf("PCA9685 servo example\n");

	setupPwmDriver();

	// Set servo to neutral position at 1.5 milliseconds
	// (View http://en.wikipedia.org/wiki/Servo_control#Pulse_duration)
//	float millis = 1.5;
//	int tick = calcTicks(millis, HERTZ);
//	pwmWrite(PIN_BASE + 16, tick);
	printf("pin base %i\n", PIN_BASE);

	moveServo(0, 0);
	delay(1000);
	moveServo(0, 200);
	delay(1000);
	moveServo(0, 5000);
	delay(1000);

	return 0;
}
