
#include "pca9685.h"
#include <wiringPi.h>

#include <linux/videodev2.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <sys/ioctl.h>

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

void camera() {

	int fdCam;
    	if((fdCam = open("/dev/video0", O_RDWR)) < 0){
        	perror("open");
		printf("error happened fd: %d\n", fdCam);
        	//exit(1);
    	}
	printf("nah fd: %d\n", fdCam);

	struct v4l2_format format;
  	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	format.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
    	format.fmt.pix.width = 2592;
    	format.fmt.pix.height = 1944;

	struct v4l2_requestbuffers bufrequest;
    	bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	bufrequest.memory = V4L2_MEMORY_MMAP;
    	bufrequest.count = 1;

	struct v4l2_buffer bufferinfo;
    	memset(&bufferinfo, 0, sizeof(bufferinfo));

    	bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	bufferinfo.memory = V4L2_MEMORY_MMAP;
    	bufferinfo.index = 0;

	void* buffer_start = mmap(
                NULL,
                bufferinfo.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fdCam,
                bufferinfo.m.offset
                );

    	if(buffer_start == MAP_FAILED){
        	perror("mmap");
		printf("looks like map failed\n");
        	//exit(1);
    	}

    	memset(buffer_start, 0, bufferinfo.length);

    	// Activate streaming
    	int type = bufferinfo.type;
    	if(ioctl(fdCam, VIDIOC_STREAMON, &type) < 0){
        	perror("VIDIOC_STREAMON");
		printf("streaming phone\n");
        	//exit(1);
    	}


}


int main(int argc, char const* argv[]) {
	camera();


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

			printf("hi hi hi motorCmd: %i, %f\n", motorCmd.portNumber, motorCmd.position);

			//moveServo(0, motorCmd.position, fd);
			moveServo(motorCmd.portNumber, motorCmd.position, fd);
		}

    	}

	return 0;
}

