#include "pca9685.h"
#include <wiringPi.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;


class PwmController {
	public:
		const int pinBase = 300;
		const int maxPwm = 4096;
		const int hertz = 50;
	private: 
		int calcTicks(float impulseMs) {
			float cycleMs = 1000.0f / hertz;
			return (int)(maxPwm * impulseMs / cycleMs + 0.5f);
		}
		float map(float input, float min, float max) {
			return (input * max) + (1 - input) * min;
		}
	public:
		int setupPwmDriver() {
			wiringPiSetup();

			// Setup with pinbase 300 and i2c location 0x40
			int fd = pca9685Setup(pinBase, 0x40, hertz);
			printf("fd: %d\n", fd);
			if (fd < 0) {
				printf("Error in setup\n");
				return fd;
			}
				

			pca9685PWMReset(fd);
			//pwmSetMode(PWM_MODE_MS);

			return 0;
		}
		void moveServo(int port, float milis) {
			printf("position in %f\n", milis);

			float millis = map(milis, 1, 2);
			//printf("after map %f\n", millis);
		
			int tick = calcTicks(millis);
			printf("after tick calc %d\n", tick);

			pwmWrite(pinBase+port, milis);//coerceIn(tick, 1000, 2000));
		}
};

class MotorCommand {
	public:
		int portNumber;
		float position;
		MotorCommand(int port, float pos) {
			portNumber = port;
			position = pos;
		}
};


MotorCommand pipeDataToMotorCommand(char* data) {

	string sData = data;
	string dataWithoutKey = sData.substr(9, sData.length());
	printf("dataWithoutKey %s\n", dataWithoutKey);
	
	int startOfSecondTerm = dataWithoutKey.find(" ");

	string firstTerm = dataWithoutKey.substr(0, startOfSecondTerm);
	string secondTerm = dataWithoutKey.substr(startOfSecondTerm, dataWithoutKey.length());
	printf("firstTerm %s, secondTerm %s\n", firstTerm.c_str(), secondTerm.c_str());

	int portNumber = stoi(firstTerm);
	float position = stof(secondTerm);
	cout << "portNumber " << portNumber << " position " << position << + "\n";	

	MotorCommand motorCmd(portNumber, position);

	cout << motorCmd.portNumber << " " << motorCmd.position << "\n";

	return motorCmd;
}

bool checkForPipeData(char* data) {
	string key = "pipeData";

	string sData = data;
	string potentialKey = sData.substr(0, key.length());
	if(potentialKey.compare(key) == 0) {
		return true;
	} else {
		return false;
	}
}

void armEsc(int port, PwmController servoCtrl) {
	//servoCtrl.moveServo(port, 0.0f);
	
	pwmWrite(300+port, 500);
	//usleep(2100);
	pwmWrite(300+port, 1000);
	//usleep(100);
	pwmWrite(300+port, 1500);

	//servoCtrl.moveServo(port, 0.5f);
}

const int pinBase = 300;
const int maxPwm = 4096;
const int hertz = 50;

int calcTicks(float impulseMs, int hertz, int MAX_PWM)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

void moveServo(int servoNum, float position, int fd){
	int port = servoNum;
	printf("Sending to port %d\n", port);
		//pca9685FullOn(fd, 0, 1);
	float zero = 1.1;
	float full = 2.0;
	float travel = full - zero;
	float millis = zero + (position * travel);
	pca9685PWMWrite(fd, pinBase + port, 0, calcTicks(millis, hertz, maxPwm));
	printf("Sent it\n");
}

int main(int argc, char const* argv[]) {
	printf("Running motor control program.\n");
			//wiringPiSetup();

			// Setup with pinbase 300 and i2c location 0x40
	printf("Connecting to the servo board via i2c...\n");	
	int fd = pca9685Setup(pinBase, 0x40, hertz);
	printf("Connected - fd: %d\n", fd);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}

	float target = 0.0;
	pca9685PWMFreq(fd, hertz);
	pca9685PWMReset(fd);
	float speed  = 0.05;

	for(int port = 0; port <= 15; port++){
		moveServo(port, 0.5, fd);
	}
	sleep(1);
	for(int port = 0; port <= 15; port++){
		moveServo(port, 0.5 - speed, fd);
	}
	sleep(5);
	for(int port = 0; port <= 15; port++){
		moveServo(port, 0.5, fd);
	}
	sleep(3);
	for(int port = 0; port <= 15; port++){
		moveServo(port, 0.5 + speed, fd);
	}
	sleep(5);
	for(int port = 0; port <= 15; port++){
		moveServo(port, 0.5, fd);
	}

	printf("Exiting\n");
	//pwmWrite(pinBase+port, 0.0);//coerceIn(tick, 1000, 2000));
			
	return 0;
}
