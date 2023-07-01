#include <cstdio>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>

#include <wiringPi.h>
#include <softPwm.h>

//#include <node_api.h>

using namespace std;

class PwmController {
        public : 
		int pinNumber;
		PwmController(int gpioNumber) {
                	pinNumber = gpioNumber;
			softPwmCreate(gpioNumber, 0, 100);
        	}
		
};

int main(int argc, char **argv) {
        if (wiringPiSetup() == -1)
                return 1 ;
        const int servoNumber = stoi(argv[1]);
        const int servoPosition = stoi(argv[2]);
        printf("servo %d, position %d hi there bud\n", servoNumber, servoPosition);

        int targetPin;
        if (servoNumber == 1) {
                targetPin = 23;
        } else {
                targetPin = 26;
        }

	const double conversion = 1024.0/100.0;

	pinMode(targetPin, PWM_OUTPUT);
	pwmWrite(targetPin, servoPosition * conversion);
	
}
/*int main(int argc, char **argv) {
        if (wiringPiSetup() == -1)
                return 1 ;

	const int servoNumber = stoi(argv[1]);
        const int servoPosition = stoi(argv[2]);
        printf("servo %d, position %d hi there bud\n", servoNumber, servoPosition);

        int targetPin;
        if (servoNumber == 1) {
                targetPin = 23;
        } else {
                targetPin = 26;
        }

//        softPwmCreate(targetPin, 0, 100);
//        softPwmWrite(targetPin, servoPosition);

	PwmController pc(23);
	
	ifstream myfile ("dataFile.txt");
	for (;;) {
		string mystring;
		myfile >> mystring;
		cout << mystring;
		myfile.clear();
	}
}*/

