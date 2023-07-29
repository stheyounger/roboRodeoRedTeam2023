#include "pca9685.h"
#include <wiringPi.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;


int coerceIn(int value, int min, int max) {
	if (value > max) {
		return max;
	} else if (value < min) {
		return min;
	} else {
		return value;
	}
}
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


int main(int argc, char const* argv[]) {
	printf("Running motor control program.\n");

	PwmController servoCtrl;
	servoCtrl.setupPwmDriver();
	printf("after setup, before move\n");	
	/*armEsc(0, servoCtrl);
	armEsc(1, servoCtrl);*/
	servoCtrl.moveServo(0, 00);
	usleep(1000);
	servoCtrl.moveServo(0, 1500);


  	char* l = NULL;
  	size_t n;
	while (getline(&l,&n, stdin) != -1 ) {
      		printf("MotorCtrl <- stdin: %s\n",l);

		if (checkForPipeData(l)) {
			printf("Pipe Data: %s\n", l);

			MotorCommand motorCmd = pipeDataToMotorCommand(l);

			printf("motorCmd: %i, %f\n", motorCmd.portNumber, motorCmd.position);

			servoCtrl.moveServo(motorCmd.portNumber, motorCmd.position);
		}

    	}


	return 0;
}
