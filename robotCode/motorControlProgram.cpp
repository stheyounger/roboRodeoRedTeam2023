
#include "pca9685.h"
#include <wiringPi.h>

#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <unistd.h>


using namespace std;


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

void empty() {


			wiringPiSetup();
}

float coerceIn(float value, float min, float max) {
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	} else {
		return value;
	}
}

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
	float zero = 1.1;
	float full = 2.0;
	float travel = full - zero;
	float boundedPosition = coerceIn(position, 0.0, 1.0);
	float millis = zero + (boundedPosition * travel);
	pca9685PWMWrite(fd, port, 0, calcTicks(millis, hertz, maxPwm));
	printf("Sent it\n");
}


void sendUnboundedPwm(int servoNum, float position, int fd){
	int port = servoNum;
	printf("Sending unbounded pwm to port %d\n", port);
	float zero = 1.1;
	float full = 2.0;
	float travel = full - zero;
	//float boundedPosition = coerceIn(position, 0.0, 1.0);
	float millis = zero + (position * travel);
	pca9685PWMWrite(fd, port, 0, calcTicks(millis, hertz, maxPwm));
	printf("Sent unbounded pwm \n");
}

/*void camera() {
	const int fileDescriptor = open("/dev/video0", O_RDWR);
	
	struct v4l2_buffer buffer;
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	const int numberOfBytes = 200;
	const int video = read(fileDescriptor, buffer, numberOfBytes);
}*/

void initServos(int fd) {
	moveServo(0, 0.5, fd);
	moveServo(1, 0.5, fd);
	moveServo(2, 0.5, fd);
	moveServo(3, 0.5, fd);
	moveServo(4, 0.5, fd);
	moveServo(5, 0.5, fd);
		
	moveServo(12, 0, fd);
	moveServo(13, 0, fd);
	moveServo(14, 1, fd);
	moveServo(15, 0, fd);
}

int main(int argc, char const* argv[]) {
	//camera();


	printf("Running motor control program.\n");
	printf("Connecting to the servo board via i2c...\n");	
	int fd = pca9685Setup(300, 0x40, hertz);
	
	printf("Connected - fd: %d\n", fd);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}

	//float target = 0.0;
	pca9685PWMFreq(fd, hertz);
	pca9685PWMReset(fd);
	//float speed  = 0.05;
	initServos(fd);

	/*for (;;) {
		moveServo(14, 1, fd);
		moveServo(15, 0, fd);
		sleep(2);

		moveServo(14, 0, fd);
		moveServo(15, 1, fd);
		sleep(2);
	}*/
	/*for(int port = 0; port <= 3; port++){
		const float power = 0.6;
		moveServo(port, power, fd);
		printf("Moving %d power %f", port, power);
		sleep(1);
	}
	for(int port = 0; port <= 3; port++){
		moveServo(port, 0.5, fd);
	}
	for(int port = 0; port <= 3; port++){
		const float power = 0.6;
		moveServo(port, power, fd);
		printf("Moving %d power %f", port, power);
		sleep(1);
	}
	for(int port = 0; port <= 3; port++){
		moveServo(port, 0.5, fd);
	}*/

	char* l = NULL;
  	size_t n;
	while (getline(&l,&n, stdin) != -1 ) {
      		printf("MotorCtrl <- stdin: %s\n",l);

		if (checkForPipeData(l)) {
			MotorCommand motorCmd = pipeDataToMotorCommand(l);

			printf("motorCmd: %i, %f\n", motorCmd.portNumber, motorCmd.position);

			//moveServo(0, motorCmd.position, fd);
			if (motorCmd.portNumber == 10 || motorCmd.portNumber == 11) {
				sendUnboundedPwm(motorCmd.portNumber, motorCmd.position, fd);
			} else {
				moveServo(motorCmd.portNumber, motorCmd.position, fd);
			}
		}

    }

	return 0;
}

